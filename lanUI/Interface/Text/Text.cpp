//
//  Text.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Text.hpp"

Text::Text(const std::string source, Font& font): source(source), fontVirtualSize(12){
    DrawableObject();
    this->font = font;
    primaryColor.set(Colors::Black);
    secondaryColor.set(Colors::White);
    wasCompiled = false;
    dpiK = 1;
}

Text& Text::from_string(const std::string source, Renderer * renderer){
    this->renderer.set(renderer);
    this->source.set(source);
    wasCompiled = false;
    if(renderer) compile(renderer);
    return (*this);
}

void Text::tryCompile(){
    renderer.get();
    wasCompiled = compile(renderer.data, true);
    renderer.leave();
}

void Text::_adjustTextDPI(){
    font.set_style(font.style, fontVirtualSize.get()*(dpiK));
    //TTF_SetFontOutline(font.children[font.style], 1);
    // TTF_SetFontHinting(font.children[font.style], TTF_HINTING_NORMAL);
    fontVirtualSize.leave();
}

bool Text::compile(SDL_Renderer * renderer, bool internCall, bool fixCall){
    _freeImage();
    if(renderer && source.get().length()){
        source.leave();
        static float wDif(0);
        static float hDif(0);
        
        wDif = size.get().w;
        hDif = size.data.h;
        size.leave();
        
        drawMode.set(DrawMode::ImageMode);
        
        if(!font.child)
            Core::log(Core::Error, "Text: Invalid font style.");
        
        SDL_Surface * surfc = nullptr;
        
        if(!(surfc = TTF_RenderUTF8_Blended(font.child, source.get().data(), (SDL_Color)primaryColor.get()))){
            Core::log(Core::Warning, "Text: Renderization failed.");
        }
        
        source.leave();
        primaryColor.leave();
        
        if(internCall){
            this->image.set(SDL_CreateTextureFromSurface(renderer, surfc));
            if(!fixCall)
                set_size(surfc->w, surfc->h);
        } else {
            fromSurface(surfc, renderer);
        }
        
        if(!fixCall) {
            // get the difference bettwen the actual with, and the last one
            wDif = size.get().w - wDif;
            hDif = size.data.h - hDif;
            size.leave();
            if(rootType.get()){
                rootType.leave();
                _sync_root_size(wDif, hDif);
            } rootType.leave();
        }
        
        SDL_FreeSurface(surfc);
        source.leave();
        return true;
    } drawMode.set(DrawMode::DefaultMode);
    source.leave();
    Core::log(Core::Warning, "Text compilation failed (invalid renderer).");
    return false;
}

void Text::_render_background(SDL_Renderer * renderer, Rect * rect){
    secondaryColor.hold();
    SDL_SetRenderDrawColor(renderer, secondaryColor.data.r, secondaryColor.data.g, secondaryColor.data.b, secondaryColor.data.a);
    secondaryColor.leave();
    SDL_RenderFillRectF(renderer, rect);
}

void Text::_render(SDL_Renderer * renderer, float x, float y, const float dpiK){
    if(_inRootBounds(x, y)){
        
        _align(x, y);
        
        rect_buffer.hold();
        if(!wasCompiled) {
            wasCompiled = compile(renderer);
        }

        if(this->dpiK != dpiK){
            this->dpiK = dpiK;
            _adjustTextDPI();
            wasCompiled = compile(renderer, true, true);
        }

        rect_buffer.data = {
            (x + padding.get().left)*dpiK,
            (y + padding.data.top)*dpiK,
            ((size.data.w)*dpiK),
            ((size.data.h)*dpiK)
        };
        
        padding.leave();
        size.leave();

        if(withBackground)
            _render_background(renderer, &rect_buffer.data);
        
        SDL_RenderCopyF(renderer, image.get(), NULL, &rect_buffer.data);
        image.leave();
        rect_buffer.leave();
        _renderEmbedded(renderer, x, y, dpiK);
    } else {
        Core::log(Core::Warning, ("Text outside root bounds (" + std::to_string(x) + "VS"
                                   + std::to_string(root.get()->size.get().x) +":" + std::to_string(y) + ")").c_str());
        root.data->size.leave();
        root.leave();
    }
};



Text& Text::set_style(TextStyle preset){
    fontVirtualSize = preset.size;
    font.set_style(preset.font_style, preset.size);
    tryCompile();
    return (*this);
}

Text& Text::set_font(Font & new_font){
    font = new_font;
    font.style = Font::Regular;
    font.set_style(Font::Regular, 12);
    tryCompile();
    return (*this);
}

Text& Text::set_primary_color(const Color color){
    primaryColor.set(color);
    tryCompile();
    return (*this);
}

Text& Text::set_background_color(const Color color){
    secondaryColor.set(color);
    withBackground = true;
    return (*this);
}

Text& Text::regular(const int size){
    fontVirtualSize = size;
    font.set_style(Font::Regular, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::bold(const int size){
    fontVirtualSize = size;
    font.set_style(Font::Bold, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::boldOblique(const int size){
    fontVirtualSize = size;
    font.set_style(Font::BoldOblique, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::extraLight(const int size){
    fontVirtualSize = size;
    font.set_style(Font::ExtraLight, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::oblique(const int size){
    fontVirtualSize = size;
    font.set_style(Font::Oblique, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::condensed_Bold(const int size){
    fontVirtualSize = size;
    font.set_style(Font::Condensed_Bold, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::condensed_BoldOblique(const int size){
    fontVirtualSize = size;
    font.set_style(Font::Condensed_BoldOblique, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::condensed_Oblique(const int size){
    fontVirtualSize = size;
    font.set_style(Font::Condensed_Oblique, size*dpiK);
    tryCompile();
    return (*this);
}

Text& Text::condensed(const int size){
    fontVirtualSize = size;
    font.set_style(Font::Condensed, size*dpiK);
    tryCompile();
    return (*this);
}
