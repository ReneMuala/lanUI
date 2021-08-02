//
//  Text.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Text_hpp
#define Text_hpp

#include <string>
#include "../Object/Object.hpp"
#include "../Font/Font.hpp"


typedef struct {
    Font::Style font_style;
    int size;
} TextStyle;

namespace TextStyles {
    const TextStyle Header = {Font::Style::Bold, 18};
    const TextStyle Default = {Font::Style::Regular, 12};
    const TextStyle Caption = {Font::Style::Oblique, 11};
    const TextStyle Footer = {Font::Style::Condensed, 10};
}

class Text: public DrawableObject {
    bool wasCompiled, withBackground;
    Semaphore<std::string> source;
    int dpiK;
    Semaphore<int> fontVirtualSize;
    Font font;
public:
    
    Text(const std::string source = "", Font& font = Fonts::DejaVuSans);
    
    /// Creates a text image from std::string, set renderer when an imediate text output is required
    Text& from_string(const std::string source , Renderer * renderer = nullptr);
        
    void tryCompile();
    
    void _adjustTextDPI();
    
    bool compile(Renderer*, bool internCall = false, bool fixCall = false);
    
    void _render_background(SDL_Renderer*, Rect*);
    
    void _render(SDL_Renderer*, float x, float y, const float dpiK) override;
        
    Text& set_style(TextStyle);
    
    Text& set_font(Font &);
    
    Text& set_primary_color(const Color) override;
    
    Text& set_background_color(const Color);
    
    Text& regular(const int = 0);
    
    Text& bold(const int = 0);
    
    Text& boldOblique(const int = 0);
    
    Text& extraLight(const int = 0);
    
    Text& oblique(const int = 0);
    
    Text& condensed_Bold(const int = 0);
    
    Text& condensed_BoldOblique(const int = 0);
    
    Text& condensed_Oblique(const int = 0);
    
    Text& condensed(const int = 0);
        
};

#endif /* Text_hpp */
