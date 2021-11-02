//
//  Text.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Text.hpp"

const std::string TextStyle::toStr() const {
    std::string str = " ";

    switch (font_style) {
        case Font::Regular:
            str+="\\regular";
            break;
        case Font::Bold:
            str+="\\bold";
            break;
        case Font::ExtraLight:
            str+="\\extraLight";
            break;
        case Font::BoldOblique:
            str+="\\boldOblique";
            break;
        case Font::Oblique:
            str+="\\oblique";
            break;
        case Font::Condensed_Bold:
            str+="\\condensed_Bold";
            break;
        case Font::Condensed_BoldOblique:
            str+="\\condensed_BoldOblique";
            break;
        case Font::Condensed_Oblique:
            str+="\\condensed_Oblique";
            break;
        case Font::Condensed:
            str+="\\condensed";
            break;
        default:break;
    }
    
    str += " \\size:" + std::to_string(size) + " ";
    return str;
}

#include <iostream>
Text::Text(const std::string source, Font& font): withBackground(false), source(source), ttf_api_style(TTF_api_style::Normal), fontVirtualSize(12), font(nullptr), surfc(nullptr){
    this->font = new Font(font);
    foregroundColor.set(Colors::Black);
    backgroundColor.set(Colors::Transparent);
    if(source.size())
        from_string(source);
}

Text::~Text(){
    _delete_custom_data();
    Object::~Object();
}

void Text::_delete_custom_data(){
    source.data.clear();
    _free_font();
}

void Text::_free_font(){
    if(font)
        delete font;
    font = nullptr;
}

Text& Text::from_string(const std::string source, Renderer * renderer){
    this->renderer.set(renderer);
    this->source.set(source);
    wasCompiled = false;
    if(renderer) compile_canvas(renderer);
    return (*this);
}

void Text::tryCompile(){
    DPIConstant.set(1);
    wasCompiled.set(false);
}

void Text::_adjustTextDPI(){
    font->size = fontVirtualSize.get();
    font->set_scaling_factor(DPIConstant.get());
    DPIConstant.leave();
    fontVirtualSize.leave();
}

void Text::_set_font_style(const Font::Style style, const unsigned int size){
    if(font){
        font->set_style(style, (size ? (fontVirtualSize.get() = size) : fontVirtualSize.get()));
        fontVirtualSize.leave();
        DPIConstant.leave();
    } else {
        Core::log(Core::Warning, "Failed to set Text::Font* (nullptr)");
    }
}

bool Text::compile_canvas(SDL_Renderer * renderer){
    if(renderer && !source.get().empty()){
        
        _free_canvas();

        if(!font->ttfFont.get()){
            font->ttfFont.leave();
            source.leave();
            Core::log(Core::Warning, "Text compilation failed  (Invalid font style).");
            return false;
        } else {
            if(((TTF_api_style)TTF_GetFontStyle(font->ttfFont.data)) != ttf_api_style.get()){
                TTF_SetFontStyle(font->ttfFont.data, (int)ttf_api_style.data);
            } ttf_api_style.leave();
            font->ttfFont.leave();
        }
        
        if(surfc) SDL_FreeSurface(surfc);
        surfc = nullptr;
        if(!compatibilityMode) {
            if(!(surfc=TTF_RenderUTF8_Blended(font->ttfFont.data, source.data.data(), (SDL_Color)foregroundColor.get()))) {
#ifdef LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
                Core::log(Core::Warning, "Text: Render failed (invalid surfc).");
#endif
                foregroundColor.leave();
                source.leave();
                font->ttfFont.leave();
                return false;
            }
        } else {
            switch (compatibilityMode) {
                case RenderShadedMode:
                    if(!(surfc=TTF_RenderUTF8_Shaded(font->ttfFont.data, source.data.data(), (SDL_Color)foregroundColor.get(), (SDL_Color)backgroundColor.get()))){
#ifdef LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
                        Core::log(Core::Warning, "Text (CompatibilityMode[RenderShadedMode]): Render failed.");
#endif
                        foregroundColor.leave();
                        backgroundColor.leave();
                        source.leave();
                        font->ttfFont.leave();
                        return false;
                    } else
                        backgroundColor.leave();
                    break;
                case RenderSolidMode:
                    if(!(surfc=TTF_RenderUTF8_Solid(font->ttfFont.data, source.data.data(), (SDL_Color)foregroundColor.get()))){
#ifdef LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
                        Core::log(Core::Warning, "Text (CompatibilityMode[RenderSolidMode]): Render failed.");
#endif
                        foregroundColor.leave();
                        source.leave();
                        font->ttfFont.leave();
                        return false;
                    }
                    break;
                default:break;
            }
        }
        
        font->ttfFont.leave();
        source.leave();
        foregroundColor.leave();
        this->canvas.set(SDL_CreateTextureFromSurface(renderer, surfc));
        SDL_FreeSurface(surfc);
        surfc = nullptr;
        drawMode.set(DrawMode::ImageMode);
        return true;
    } else if (source.data.empty()) {
#ifdef LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
        Core::log(Core::Warning, "Text compilation ignored (invalid string).");
#endif
        source.leave();
        return true;
    } drawMode.set(DrawMode::DefaultMode);
    source.leave();
#ifdef LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
    Core::log(Core::Warning, "Text compilation failed (invalid renderer or string).");
#endif
    _free_canvas();
    return false;
}

void Text::_compute_text_size(){
    if(font && !source.get().empty()){
        if(font->ttfFont.get() && font->scalingFactorConstant > 1){
            const int constantBuffer = font->scalingFactorConstant;
            font->ttfFont.leave();
            font->set_scaling_factor(1);
            int w, h;
            TTF_SizeUTF8(font->ttfFont.data, source.data.c_str(), &w, &h);
            size.hold();
            size.data.w = w;
            size.data.h = h;
            size.leave();
            font->set_scaling_factor(constantBuffer);
        } else
            font->ttfFont.leave();
    } source.leave();
    source.leave();
}

void Text::_compile(Renderer * renderer, const float dpiK){
    // prepare text
    if(dpiK != DPIConstant.get()){
        DPIConstant.data = (dpiK);
        DPIConstant.leave();
        _adjustTextDPI();
        wasCompiled.set(false);
    } else
        DPIConstant.leave();
    if(!wasCompiled.get()) {
        do {
#ifdef LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
            Core::log(Core::Warning, "Text was not compiled, compiling... (MAIN CASE)");
#endif
            wasCompiled.data = compile_canvas(renderer);
        } while(!wasCompiled.data);
        
        _compute_text_size();
        
        if(root.get()){
            root.data->reload();
        } root.leave();
    } wasCompiled.leave();
    
    _compile_embedded(renderer, dpiK);
}

void Text::_render_background(SDL_Renderer * renderer, Rect * rect){
    backgroundColor.hold();
    SDL_SetRenderDrawColor(renderer, backgroundColor.data.r, backgroundColor.data.g, backgroundColor.data.b, backgroundColor.data.a);
    backgroundColor.leave();
    SDL_RenderFillRectF(renderer, rect);
}

void Text::_render(SDL_Renderer * renderer, float x, float y, const float dpiK, bool inComposition){
        if(_inRootBounds(x, y)){
            _align(x, y);
            _set_position(x, y);
            _render_routine(dpiK);
            _render_image(renderer, x, y, dpiK);
        }
    #ifdef LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
        else {
            Core::log(Core::Warning, "Text outside root bounds.");
        }
    #endif

#ifdef LANUI_DEBUG_MODE
    if(inComposition)
        SDL_SetRenderDrawColor(renderer, 200, 200, 255, 50);
    else
        SDL_SetRenderDrawColor(renderer, 255, 200, 200, 50);
    SDL_RenderFillRectF(renderer, &sizeBuffer.get());
    SDL_SetRenderDrawColor(renderer, 20, 255, 200, 200);
    SDL_RenderDrawRectF(renderer, &sizeBuffer.data);
    sizeBuffer.leave();
#endif
    
    if(!inComposition)
        _renderEmbedded(renderer, x, y, dpiK, _renderOnlyNextInX_Y);
};

Text& Text::inherit_background_color(){
    if(root.get()){
            backgroundColor.set((root.data)->foregroundColor.get());
            (root.data)->foregroundColor.leave();
    } else
        Core::log(Core::Warning, "Using inherit_backgroundColor(...) without a root. (nullptr)");
    root.leave();
    return (*this);
}

Text& Text::set_ttf_api_style(const TTF_api_style style){
    ttf_api_style.set(style);
    return (*this);
}

Text& Text::set_style(const TextStyle preset){
    fontVirtualSize = preset.size;
    font->set_style(preset.font_style, preset.size);
    tryCompile();
    return (*this);
}

Text& Text::set_font(Font & new_font){
    Font::Style last_font_style = font->style;
    _free_font();
    font = new Font(new_font);
    _set_font_style(last_font_style);
    tryCompile();
    return (*this);
}

Text& Text::set_foreground_color(const Color color){
    foregroundColor.set(color);
    tryCompile();
    return (*this);
}

Text& Text::regular(const unsigned int size){
    _set_font_style(Font::Regular, size);
    tryCompile();
    return (*this);
}

Text& Text::bold(const unsigned int size){
    _set_font_style(Font::Bold, size);
    tryCompile();
    return (*this);
}

Text& Text::boldOblique(const unsigned int size){
    _set_font_style(Font::BoldOblique, size);
    tryCompile();
    return (*this);
}

Text& Text::extraLight(const unsigned int size){
    _set_font_style(Font::ExtraLight, size);
    tryCompile();
    return (*this);
}

Text& Text::oblique(const unsigned int size){
    _set_font_style(Font::Oblique, size);
    tryCompile();
    return (*this);
}

Text& Text::condensed_Bold(const unsigned int size){
    _set_font_style(Font::Condensed_Bold, size);
    tryCompile();
    return (*this);
}

Text& Text::condensed_BoldOblique(const unsigned int size){
    _set_font_style(Font::Condensed_BoldOblique, size);
    tryCompile();
    return (*this);
}

Text& Text::condensed_Oblique(const unsigned int size){
    _set_font_style(Font::Condensed_Oblique, size);
    tryCompile();
    return (*this);
}

Text& Text::condensed(const unsigned int size){
    _set_font_style(Font::Condensed, size);
    tryCompile();
    return (*this);
}
