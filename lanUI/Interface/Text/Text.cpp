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
    foregroundColor.set(Colors::Black);
    backgroundColor.set(Colors::Red);
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

bool Text::compile(SDL_Renderer * renderer, bool internCall, bool fixCall){
    if(renderer){
        source.leave();
        
        drawMode.set(DrawMode::ImageMode);
        
        if(!font.child.get())
            Core::log(Core::Error, "Text: Invalid font style.");
        
        static SDL_Surface * surfc = nullptr;
        
        surfc = nullptr;
        
        _freeImage();
        perror("before compilation");
        if(!compatibilityMode) {
             if(!(surfc=TTF_RenderUTF8_Blended(font.child.data, source.get().data(), (SDL_Color)foregroundColor.get())))
                 Core::log(Core::Warning, "Text: Render failed.");
        } else {
            switch (compatibilityMode) {
                case RenderShadedMode:
                    if(!(surfc=TTF_RenderUTF8_Shaded(font.child.data, source.get().data(), (SDL_Color)foregroundColor.get(), (SDL_Color)backgroundColor.get())))
                        Core::log(Core::Warning, "Text (CompatibilityMode[RenderShadedMode]): Render failed.");
                    break;
                case RenderSolidMode:
                    if(!(surfc=TTF_RenderUTF8_Solid(font.child.data, source.get().data(), (SDL_Color)foregroundColor.get())))
                        Core::log(Core::Warning, "Text (CompatibilityMode[RenderSolidMode]): Render failed.");
                    break;
                default:break;
            }
        }
        
        font.child.leave();
        
        source.leave();
        foregroundColor.leave();
        backgroundColor.leave();
        if(internCall){
            this->image.set(SDL_CreateTextureFromSurface(renderer, surfc));
            if(!fixCall)
                set_size(surfc->w, surfc->h);
        } else {
            fromSurface(surfc, renderer, false);
            size.leave();
        }
        
        if(!fixCall) {
            if(root.get()){
                root.data->reload();
            } root.leave();
        }
                
        SDL_FreeSurface(surfc);
        source.leave();
        perror("compilation done");
        return true;
    } drawMode.set(DrawMode::DefaultMode);
    source.leave();
    Core::log(Core::Warning, "Text compilation failed (invalid renderer).");
    return false;
}

void Text::_render_background(SDL_Renderer * renderer, Rect * rect){
    backgroundColor.hold();
    SDL_SetRenderDrawColor(renderer, backgroundColor.data.r, backgroundColor.data.g, backgroundColor.data.b, backgroundColor.data.a);
    backgroundColor.leave();
    SDL_RenderFillRectF(renderer, rect);
}

void Text::_render(SDL_Renderer * renderer, float x, float y, const float dpiK){
    
    // prepare texts
    if(!wasCompiled) {
        wasCompiled = compile(renderer);
    } if(this->dpiK != dpiK){
        this->dpiK = dpiK;
        _adjustTextDPI();
        wasCompiled = compile(renderer, true, true);
    }
    
    if(_inRootBounds(x, y)){
        _align(x, y);
        size.hold(); padding.hold(); size.data.x = x + padding.data.left; size.data.y = y + padding.data.top; size.leave(); padding.leave();
        _render__image(renderer, x, y, dpiK);
    } else {
        //Core::log(Core::Warning, "Text outside root bounds.");
        size.leave();
        root.data->size.leave();
        root.leave();
    } _renderEmbedded(renderer, x, y, dpiK, _renderOnlyNextInX_Y);
};

Text& Text::inherit_background_color(){
    if(root.get()){
        if(root.data->properties[Properties::isDrawable].get()){
            backgroundColor.set(((DrawableObject*)root.data)->foregroundColor.get());
            ((DrawableObject*)root.data)->foregroundColor.leave();
        } else {
            Core::log(Core::Warning, "Using inherit_backgroundColor(...) without a Drawable root.");
        } root.data->properties[Properties::isDrawable].leave();
    } else
        Core::log(Core::Warning, "Using inherit_backgroundColor(...) without a root. (nullptr)");
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

Text& Text::set_foreground_color(const Color color){
    foregroundColor.set(color);
    tryCompile();
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
