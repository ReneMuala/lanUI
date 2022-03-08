//
//  Text.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Text.hpp"
#include "../Theme/Theme.hpp"
#include "../Window/WindowManager/WindowManager.hpp"
#include <iostream>

const std::string TextStyle::toStr() const {
    std::string str = " ";

    switch (font_style) {
        case Font::Regular:
            str+="%regular";
            break;
        case Font::Bold:
            str+="%bold";
            break;
        case Font::ExtraLight:
            str+="%extraLight";
            break;
        case Font::BoldOblique:
            str+="%boldOblique";
            break;
        case Font::Oblique:
            str+="%oblique";
            break;
        case Font::Condensed_Bold:
            str+="%condensed_Bold";
            break;
        case Font::Condensed_BoldOblique:
            str+="%condensed_BoldOblique";
            break;
        case Font::Condensed_Oblique:
            str+="%condensed_Oblique";
            break;
        case Font::Condensed:
            str+="%condensed";
            break;
        default:break;
    }
    
    str += " %size:" + std::to_string(size) + " ";
    return str;
}

namespace WMSSharedData
{
extern std::vector<Window *> programWindows;
}

void Text::default_composer_callback()
{
    CairoContext * cr = composerParams.context;
    cairo_rectangle(cr, 0, 0, textExtents.x_advance, fontExtents.height);
    cairo_set_source_rgba(cr, 0, 0, 0, 0.3);
    cairo_fill(cr);
        
    printf("%s\n", font.get_name().c_str());
    
    cairo_set_font_face(cr, cairoFontFace);
    
    // (size-(size-0.88010350017162*size))
    cairo_set_font_size(cr, font.get_size()/(composerParams.greaterSide));
    
    cairo_move_to(cr, 0, fontExtents.ascent/(composerParams.greaterSide));
    if(!composer_set_foreground_pattern_as_source(cr)){
        foregroundColor.hold();
        cairo_set_source_rgba(cr, foregroundColor.data.get_premultiplied_r(), foregroundColor.data.get_premultiplied_g(), foregroundColor.data.get_premultiplied_b(), foregroundColor.data.get_premultiplied_a());
        foregroundColor.leave();
    } cairo_show_text(cr, source.c_str());
}

void Text::_compute_text_size(){
//    WMSSharedData::programWindows[windowId]->textParams.compute_params(cairoFontFace, textExtents, fontExtents, font.get_size(), source.c_str());
    set_size(textExtents.x_advance, fontExtents.height);
}

bool Text::_allocate_cairo_font_face(){
    if(font.generate_ft_face(fontFace,windowId)){
        cairoFontFace = fontFace.new_cairo_font_face();
        return true;
    } return false;
}

void Text::_free_cairo_font_face(){
    fontFace.done_face();
    if(cairoFontFace){
        cairo_font_face_destroy(cairoFontFace);
        cairoFontFace = nullptr;
    }
}

Text& Text::set_size(const float size_w, const float size_h){
    Object::set_size(size_w, size_h);
    return (*this);
}

void Text::_compose(Renderer * renderer, const float dpiK, const int32_t windowId){
    if(wasCompiled.get()){
        if(!(wasCompiled.data = dpiK == obj_dpiK.get_copy()))
            obj_dpiK.set(dpiK);
    } else
        obj_dpiK.set(dpiK);
        wasCompiled.leave();
    
    this->windowId = windowId;
        
    if(!wasCompiled.get()){
        _allocate_cairo_font_face();
        wasCompiled.leave();
        _compute_text_size();
        _composer_routine(renderer);
        _free_cairo_font_face();
//        TODO: FIX TEXT COMPILATION INCONSISTENCES
//        wasCompiled.data = true;
    } wasCompiled.leave();
    _compose_embedded(renderer, dpiK, windowId);
}

Text::Text(const std::string source, Font& font): source(source), textEffect(Normal), font(font) {
    Object();
    globalSemaphore.leave();
    foregroundColor.set(Themes::_default.colors.get_text_color<Primary>());
    backgroundColor.set(Colors::Transparent);
    this->font.set_size(12);
    set_composer_callback(CallbackExpr(default_composer_callback();));
}

Text& Text::from_string(const std::string source){
    globalSemaphore.hold();
    this->source = source;
    wasCompiled.set(false);
    globalSemaphore.leave();
    return (*this);
}

Text& Text::set_font_size(const double size){
    globalSemaphore.hold();
    font.set_size(size);
    wasCompiled.set(false);
    globalSemaphore.leave();
    return (*this);
}

Text& Text::set_font_style(const Font::Style style, const Font::Style alternative_style){
    globalSemaphore.hold();
    font.set_style(style, alternative_style);
    wasCompiled.set(false);
    globalSemaphore.leave();
    return (*this);
}

Text& Text::set_text_effect(const TextEffect effect){
    globalSemaphore.hold();
    textEffect = effect;
    wasCompiled.set(false);
    globalSemaphore.leave();
    return (*this);
}

Text& Text::set_style(const TextStyle preset){
    globalSemaphore.hold();
    font.set_size(preset.size);
    font.set_style(preset.font_style);
    wasCompiled.set(false);
    globalSemaphore.leave();
    return (*this);
}

Text& Text::set_font(Font & new_font){
    globalSemaphore.hold();
    Font::Style last_font_style = font.get_style();
    font = new_font;
    wasCompiled.set(false);
    globalSemaphore.leave();
    set_font_style(last_font_style);
    return (*this);
}

Text& Text::set_foreground_color(const Color color){
    globalSemaphore.hold();
    foregroundColor.set(color);
    wasCompiled.set(false);
    globalSemaphore.leave();
    return (*this);
}

Text& Text::regular(){
    return set_font_style(Font::Regular);
}

Text& Text::bold(){
    return set_font_style(Font::Bold);
}

Text& Text::boldOblique(){
   return set_font_style(Font::BoldOblique, Font::Bold);
}

Text& Text::extraLight(){
    return set_font_style(Font::ExtraLight, Font::Light);
}

Text& Text::oblique(){
    return set_font_style(Font::Oblique, Font::Italic);
}

Text& Text::condensed_Bold(){
    return set_font_style(Font::Condensed_Bold, Font::Bold);
}

Text& Text::condensed_BoldOblique(){
    return set_font_style(Font::Condensed_BoldOblique, Font::BoldOblique);
}

Text& Text::condensed_Oblique(){
    return set_font_style(Font::Condensed_Oblique, Font::Oblique);
}

Text& Text::condensed(){
    return set_font_style(Font::Condensed);
}

Text& Text::black(){
    return set_font_style(Font::Black, Font::Bold);
}

Text& Text::blackItalic(){
    return set_font_style(Font::BlackItalic, Font::BoldItalic);
}

Text& Text::boldItalic(){
    return set_font_style(Font::BoldItalic, Font::Bold);
}

Text& Text::extraLightItalic(){
    return set_font_style(Font::ExtraLightItalic, Font::ExtraLight);
}

Text& Text::extraBold(){
    return set_font_style(Font::ExtraBold, Font::Bold);
}

Text& Text::extraBoldItalic(){
    return set_font_style(Font::ExtraBoldItalic, Font::ExtraBold);
}

Text& Text::italic(){
    return set_font_style(Font::Italic, Font::Oblique);
}

Text& Text::light(){
    return set_font_style(Font::Light);
}

Text& Text::lightItalic(){
    return set_font_style(Font::LightItalic, Font::Light);
}

Text& Text::medium(){
    return set_font_style(Font::Medium, Font::Regular);
}

Text& Text::mediumItalic(){
    return set_font_style(Font::MediumItalic, Font::Italic);
}

Text& Text::semiBold(){
    return set_font_style(Font::SemiBold, Font::Bold);
}

Text& Text::semiBoldItalic(){
    return set_font_style(Font::SemiBoldItalic, Font::BoldItalic);
}

Text& Text::thin(){
    return set_font_style(Font::Thin, Font::Light);
}

Text& Text::thinItalic(){
    return set_font_style(Font::ThinItalic, Font::LightItalic);
}
