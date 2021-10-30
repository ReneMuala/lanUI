//
//  Font.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Font.hpp"
#include <list>

namespace Fonts {
Font DejaVuSans;
}

Font::Font(): style(Regular), ready(false), ttfFont(nullptr), scalingFactorConstant(1){
    for(int i = 0 ; i < totalStyles ; i++)
        path_copy[i].clear();
}

Font::~Font(){
    free();
    for(int i = 0 ; i < totalStyles ; i++)
        path_copy[i].clear();
}

void Font::free(){
    ready = false;
    if(ttfFont.get()) TTF_CloseFont(ttfFont.data); ttfFont.data = nullptr;
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
    if(!(ttfFont.get() = TTF_OpenFont(path, size)))
        Core::log(Core::Error, ("Unable to open font file: " + std::string(path)).c_str());
    ttfFont.leave();
    ready = true;
    return true;
}

const void Font::operator=(Font &other){
    free();
    for(int i = 0 ; i < Style::totalStyles ; i++){
        if(other.path_copy[i].length()) {
            path_copy[i] = other.path_copy[i];
        } else path_copy[i].clear();
    }
    set_style(Regular);
}

Font& Font::set_style(const Style new_style, const int new_size){
    size = new_size;
    if(!path_copy[new_style].empty()) {
        _load(path_copy[new_style].c_str(), size*scalingFactorConstant);
    } else if(!path_copy[Regular].empty()) {
        Core::log(Core::Warning, "set_style(...) must to be used with a valid font style. Using Regular...");
        _load(path_copy[Regular].c_str(), size*scalingFactorConstant);
    } else {
        Core::log(Core::Error, "Using set_style(...) without any valid font style loaded.");
    } return (*this);
}

Font& Font::fromFile(const char * path, Style style){
    if(_test(path)) {
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
        _load(path_copy[style].c_str(), size*scalingFactorConstant);
    } else {
        Core::log(Core::Warning, "Using Font::set_scaling_factor(...) without a valid style.");
    }
    return (*this);
}
