//
//  Font.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Font.hpp"
#include "ProgramFonts.hpp"
#include <unordered_map>
#include "../../Utilities/PathResolver.hpp"
#include "../Window/WindowManager/WindowManager.hpp"
#include <vector>

namespace FontsSharedData
{
Semaphore<std::unordered_map<Font*,std::string const>> globalFonts;
}

namespace WMSSharedData
{
extern std::vector<FT_Library> WindowFTLibraries;
}

Font::Font(const std::string name, std::list<std::pair<const std::string,const Style>> init_styles) {
    style = Regular;
    for(int i = 0 ; i < totalStyles ; i++)
         filenames[i].clear();
    for (auto style : init_styles) {
        load_style(style.first, style.second);
    } if(!name.empty()) set_global_name(name);
}

Font::Font(const Font& other){
    this->operator=(other);
}

Font::~Font(){
    for(int i = 0 ; i < totalStyles ; i++)
        filenames[i].clear();
}

bool Font::is_valid() const {
    return is_style_loaded(Regular);
}

bool Font::is_style_loaded(const Style style) const {
    return !filenames[style].empty();
}

const void Font::operator=(const Font &other){
    this->name = other.name;
    for(int i = 0 ; i < Style::totalStyles ; i++){
        if(!other.filenames[i].empty()) {
            filenames[i] = other.filenames[i];
        } else filenames[i].clear();
    } set_style(Regular);
}

const double Font::get_size(){
    return size;
}

Font& Font::set_size(const double size){
    this->size = size;
    return (*this);
}

const Font::Style Font::get_style(){
    return style;
}

const std::string Font::get_name(){
    return name;
}

Font& Font::set_style(const Style style, const Style alternative_style) {
    if(is_style_loaded(style)) {
        this->style = style;
    } else if(is_style_loaded(alternative_style)) {
        this->style = alternative_style;
    }  else if(is_style_loaded(Regular)) {
        WindowManager::log(WindowManager::Warning, "set_style(...) must to be used with a valid font style. Using Regular...");
        this->style = Regular;
    } else {
        WindowManager::log(WindowManager::Error, "Using set_style(...) without any valid font style loaded.");
    } return (*this);
}

Font& Font::load_style(const std::string filename, const Style style){
    filenames[style] = PathResolver::resolve(filename);
    return (*this);
}

Font& Font::set_global_name(const std::string name){
    this->name = name;
    FontsSharedData::globalFonts.get().insert(std::pair<Font*,std::string const>(this,name));
    FontsSharedData::globalFonts.leave();
    return (*this);
}

Font* Font::_get_font_by_global_name(const std::string name){
    Font* target_font = nullptr;
    FontsSharedData::globalFonts.hold();
    
    // if default fonts where requested
    if(!target_font && strncasecmp(name.c_str(), "default", 7) == 0){
        target_font = &Fonts::DefaultFonts;
    } else {
        for(auto font : FontsSharedData::globalFonts.data){
            if(strcasecmp(font.second.c_str(), name.c_str()) == 0)
                target_font = font.first;
        }
    }
    
    FontsSharedData::globalFonts.leave();
    return target_font;
}

bool Font::generate_ft_face(FontFace & ft_face, const int32_t windowId){
    if(is_style_loaded(style))
        if(ft_face.open_face(WMSSharedData::WindowFTLibraries[windowId], filenames[style].c_str())){
//            ft_face.set_size((size+(size-0.88010350017162*size)));
            return true;
        }
    WindowManager::log(WindowManager::Error, "generate_ft_face(...) must to be used with a valid font style. Unafe operation...");
    return false;
}

void Font::print_avaliable_styles() const {
    for(auto index = 0; index < totalStyles ; index ++){
        printf("%2d \t %s\n", index, filenames[index].c_str());
    }
}
