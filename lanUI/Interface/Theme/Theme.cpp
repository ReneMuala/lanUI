//
//  Theme.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 26/11/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Theme.hpp"
#include "ThemeTextStyles.hpp"

void Theme::Button::default_init(){
    child = nullptr;
    heap_child = false;
}

Theme::Button::Button(Object & child){
    default_init();
    child.set_alignment(Center);
    set_content(child);
}

Theme::Button::Button(Text & child){
    default_init();
    child.set_foreground_color(Themes::_default.colors.get_accent_color<Primary>());
    child.set_ttf_api_style(Text::TTF_api_style::Underline);
    child.set_alignment(Center);
    set_content(child);
}

Theme::Button::Button(std::string const str){
    default_init();
    heap_child = (child = new Text(str));
    child->set_foreground_color(Themes::_default.colors.get_accent_color<Primary>());
    child->set_ttf_api_style(Text::TTF_api_style::Underline);
    child->set_alignment(Center);
    set_content(*child);
}

Theme::Button::~Button(){
    if(heap_child && child){
        delete child;
        child = nullptr;
        heap_child = false;
    }
}

void Theme::_set_theme_colors(){
    
    colors.surface.set_element(PalleteElement::Primary,
                                   PalleteMode::Light,
                                   Colors::Deep_sky_blue);
    colors.surface.set_element(PalleteElement::Secondary,
                                   PalleteMode::Light,
                                   Colors::White);
    colors.surface.set_element(PalleteElement::Tertiary,
                                   PalleteMode::Light,
                                   Colors::Light_gray);
    
    colors.surface.set_all(Dark, Colors::Black);
    
    colors.text.set_element(PalleteElement::Primary,
                                PalleteMode::Light,
                                Colors::Black);
    
    colors.text.set_element(PalleteElement::Secondary,
                                PalleteMode::All,
                                Colors::Gray);
    
    colors.text.set_element(PalleteElement::Tertiary,
                                PalleteMode::Light,
                                Colors::Light_gray);
    
    colors.text.set_element(PalleteElement::Primary,
                                PalleteMode::Dark,
                                Colors::White);
    
    colors.text.set_element(PalleteElement::Tertiary,
                                PalleteMode::Light,
                                Colors::Light_gray);
    
    colors.text.set_all(PalleteMode::Dark, Colors::White);
    
    colors.accent.set_element(PalleteElement::Primary,
                                  PalleteMode::All,
                                  Colors::Green);
    colors.accent.set_element(PalleteElement::Secondary,
                                  PalleteMode::All,
                                  Colors::Yellow);
    colors.accent.set_element(PalleteElement::Tertiary,
                                  PalleteMode::All,
                                  Colors::Blue);
}

void Theme::_set_default_font() const {
    Fonts::DefaultFonts = Fonts::WorkSans;
}
