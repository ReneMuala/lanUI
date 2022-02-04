//
//  Font.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Font.hpp"
#include <unordered_map>
#include "../../Utilities/PathResolver.hpp"

namespace FontsSharedData
{
BSemaphore TTF_Fcall;
Font DejaVuSans, WorkSans, OpenSans, DefaultFonts;
Semaphore<std::unordered_map<unsigned long /* FONT_ID */, TTF_Font*>> allFonts;
unsigned long allFontsCount;

Semaphore<std::unordered_map<Font*,std::string const>> globalFonts;
}

Font::Font(const std::string name) {
    _init(name);
}

void Font::_init(const std::string name){
    style = Regular;
    ttfFont = nullptr;
    scalingFactorConstant = 1;
    FontsSharedData::allFonts.hold();
    FontsSharedData::allFontsCount++;
    id_inAllFonts = FontsSharedData::allFontsCount-1;
    FontsSharedData::allFonts.leave();
    for(int i = 0 ; i < totalStyles ; i++)
         path_copy[i].clear();
    if(!name.empty()) set_global_name(name);
}

Font::~Font(){
    if(TTF_WasInit())
        free();
    for(int i = 0 ; i < totalStyles ; i++)
        path_copy[i].clear();
}

void Font::free(){
    if(ttfFont.get()){
        FontsSharedData::allFonts.hold();
        FontsSharedData::allFonts.data[id_inAllFonts] = nullptr;
        FontsSharedData::allFonts.leave();
        FontsSharedData::TTF_Fcall.hold();
        TTF_CloseFont(ttfFont.data);
        ttfFont.data = nullptr;
        FontsSharedData::TTF_Fcall.leave();
    }
    ttfFont.leave();
}

bool Font::_test(const char *path){
    static TTF_Font * buffer (nullptr);
    bool sucess = (buffer = TTF_OpenFont(path, 12));
    if(buffer) TTF_CloseFont(buffer);
    buffer = nullptr;
    return sucess;
}

bool Font::_load(const char *path, const int size){
    free();
    FontsSharedData::TTF_Fcall.hold();
    if(!(ttfFont.get() = TTF_OpenFont(path, size*scalingFactorConstant)))
        Core::log(Core::Error, ("Unable to open font file: " + std::string(path) + "(" + TTF_GetError() +")").c_str());
    else {
        FontsSharedData::TTF_Fcall.leave();
        FontsSharedData::allFonts.hold();
        FontsSharedData::allFonts.data[id_inAllFonts] = ttfFont.data;
//        std::cout << ">>\n";
//        for(auto test : Fonts::allFonts.data){
//            std::cout << test.first << "\t" << test.second << std::endl;
//        }
        FontsSharedData::allFonts.leave();
    }
    ttfFont.leave();
    return true;
}

const bool Font::isValid() const {
    return path_copy[Regular].length();
}

const void Font::operator=(const Font &other){
    free();
    for(int i = 0 ; i < Style::totalStyles ; i++){
        if(other.path_copy[i].length()) {
            path_copy[i] = other.path_copy[i];
        } else path_copy[i].clear();
    } set_style(Regular);
}

Font& Font::set_style(const Style new_style, const int new_size) {
    size = new_size;
    if(!path_copy[new_style].empty()) {
        style = new_style;
        _load(path_copy[new_style].c_str(), size);
    } else if(!path_copy[Regular].empty()) {
        Core::log(Core::Warning, "set_style(...) must to be used with a valid font style. Using Regular...");
        _load(path_copy[Regular].c_str(), size);
    } else {
        Core::log(Core::Error, "Using set_style(...) without any valid font style loaded.");
    } return (*this);
}

Font& Font::fromFile(std::string path, Style style){
    path = PathResolver::resolve(path);
    if(_test(path.c_str())) {
        path_copy[style] = path;
#ifdef LANUI_DEBUG_MODE
        std::string message = path;
        message+=" (Font style successful loaded).";
        Core::log(Core::Message, message.c_str());
#endif
    } else {
#ifdef LANUI_DEBUG_MODE
        std::string message = path;
        message+=" (Unable to load font style file).";
        Core::log(Core::Error, message.c_str());
#endif
        Core::log(Core::Error, "Unable to load font style.");
    }
    return (*this);
}

Font& Font::set_scaling_factor(const int constant){
    scalingFactorConstant = constant;
    if(!path_copy[style].empty()) {
        _load(path_copy[style].c_str(), size);
    } else {
        Core::log(Core::Warning, "Using Font::set_scaling_factor(...) without a valid style.");
    }
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
    for(auto font : FontsSharedData::globalFonts.data){
        if(strcasecmp(font.second.c_str(), name.c_str()) == 0)
            target_font = font.first;
    } if(!target_font && strncasecmp(name.c_str(), "default", 7) == 0){
        target_font = &FontsSharedData::DefaultFonts;
    }
    FontsSharedData::globalFonts.leave();
    return target_font;
}

void Font::print_avaliable_styles() const {
    for(auto index = 0; index < totalStyles ; index ++){
        printf("%2d \t %s\n", index, path_copy[index].c_str());
    }
}
