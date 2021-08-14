//
//  Text.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Text.hpp"

Text::Text(const std::string source, Font& font): wasCompiled(false), withBackground(false), source(source), fontVirtualSize(12){
    DrawableObject();
    this->font = font;
    primaryColor.set(Colors::Black);
    secondaryColor.set(Colors::Red);
    dpiK = 1;
    if(source.size())
    from_string(source);
}

Text& Text::from_string(const std::string source, Renderer * renderer){
    this->renderer.set(renderer);
    this->source.set(source);
    wasCompiled = false;
    _resetDPI();
    if(renderer) compile(renderer);
    return (*this);
}

void Text::tryCompile(){
    renderer.get();
    wasCompiled = compile(renderer.data, true);
    renderer.leave();
}

void Text::_resetDPI(){
    dpiK = 1;
    font.set_style(font.style, fontVirtualSize.get()*(1));
    fontVirtualSize.leave();
}

void Text::_adjustTextDPI(){
    font.set_style(font.style, fontVirtualSize.get()*(dpiK));
    //TTF_SetFontOutline(font.children[font.style], 1);
    // TTF_SetFontHinting(font.children[font.style], TTF_HINTING_NORMAL);
    fontVirtualSize.leave();
}
#include <iostream>
bool Text::compile(SDL_Renderer * renderer, bool internCall, bool fixCall){
    if(renderer){
        source.leave();
        
        drawMode.set(DrawMode::ImageMode);
        
        if(!font.child.get())
            Core::log(Core::Error, "Text: Invalid font style.");
        
        static SDL_Surface * surfc = nullptr;
        
        surfc = nullptr;
        
        _freeImage();
        
        if(!compatibilityMode) {
             if(!(surfc=TTF_RenderUTF8_Blended(font.child.data, source.get().data(), (SDL_Color)primaryColor.get())))
                 Core::log(Core::Warning, "Text: Render failed.");
        } else {
            switch (compatibilityMode) {
                case RenderShadedMode:
                    if(!(surfc=TTF_RenderUTF8_Shaded(font.child.data, source.get().data(), (SDL_Color)primaryColor.get(), (SDL_Color)secondaryColor.get())))
                        Core::log(Core::Warning, "Text (CompatibilityMode[RenderShadedMode]): Render failed.");
                    break;
                case RenderSolidMode:
                    if(!(surfc=TTF_RenderUTF8_Solid(font.child.data, source.get().data(), (SDL_Color)primaryColor.get())))
                        Core::log(Core::Warning, "Text (CompatibilityMode[RenderSolidMode]): Render failed.");
                    break;
                default:break;
            }
        }
        
        font.child.leave();
        
        source.leave();
        primaryColor.leave();
        secondaryColor.leave();
        if(internCall){
            this->image.set(SDL_CreateTextureFromSurface(renderer, surfc));
            if(!fixCall)
                set_size(surfc->w, surfc->h);
        } else {
            fromSurface(surfc, renderer, false);
            std::cout << size.get().w << " * " << size.data.h << std::endl;
            size.leave();
        }
        
        if(!fixCall) {
            if(root.get()){
                root.data->reload();
            } root.leave();
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

        size.hold();
        rect_buffer.data = {
            (x + padding.get().left)*dpiK,
            (y + padding.data.top)*dpiK,
            (size.data.w)*dpiK,
            (size.data.h) *dpiK
        };
        size.leave();
        
        padding.leave();

        if(withBackground)
            _render_background(renderer, &rect_buffer.data);
        
        SDL_RenderCopyExF(renderer, image.get(), NULL, &rect_buffer.data, angle.get(), NULL, SDL_FLIP_NONE);
        //SDL_RenderCopyF(renderer, image.get(), NULL, );
        image.leave();
        angle.leave();
        rect_buffer.leave();
        _renderEmbedded(renderer, x, y, dpiK);
    } else {
        Core::log(Core::Warning, "Text outside root bounds.");
        size.leave();
        root.data->size.leave();
        root.leave();
    }
};

Text& Text::inherit_secondaryColor(){
    if(root.get()){
        if(root.data->properties[Properties::isDrawable].get()){
            secondaryColor.set(((DrawableObject*)root.data)->primaryColor.get());
            ((DrawableObject*)root.data)->primaryColor.leave();
        } else {
            Core::log(Core::Warning, "Using inherit_secondaryColor(...) without a Drawable root.");
        } root.data->properties[Properties::isDrawable].leave();
    } else
        Core::log(Core::Warning, "Using inherit_secondaryColor(...) without a root. (nullptr)");
    root.leave();
    return (*this);
}

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
