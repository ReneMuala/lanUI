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
    const std::string toStr() const;
} TextStyle;

namespace TextStyles {
    const TextStyle BigTitle = {Font::Style::Bold, 36};
    const TextStyle Header = {Font::Style::Bold, 18};
    const TextStyle Default = {Font::Style::Regular, 12};
    const TextStyle Caption = {Font::Style::Oblique, 11};
    const TextStyle Footer = {Font::Style::Condensed, 10};
}

class Text: public Object {
    bool withBackground;
    Semaphore<std::string> source;
    int dpiK;
    SDL_Surface * surfc;
public:
    Semaphore<unsigned int> fontVirtualSize;
    Font * font;
    
    typedef enum {
        /// TTF_RenderUTF8_Blended(...)
        NormalMode,
        /// TTF_RenderUTF8_Shaded(...)
        RenderShadedMode,
        /// TTF_RenderUTF8_Solid(...)
        RenderSolidMode,
    } CompatibilityRenderMode;
    
    const CompatibilityRenderMode compatibilityMode = NormalMode;
    
    Text(const std::string source = "", Font& font = Fonts::DejaVuSans);
    
    ~Text();
    
    void _delete_custom_data() override;
    
    void _free_font();
    
    /// Creates a text image from std::string, set renderer when an imediate text output is required
    Text& from_string(const std::string source , Renderer * renderer = nullptr);
    
    void tryCompile();
    
    void _resetDPI();
    
    void _adjustTextDPI();
    
    bool compile(Renderer*, bool internCall = false, bool fixCall = false);
    
    void _compile(Renderer*, const float dpiK) override;
    
    void _render_background(SDL_Renderer*, Rect*);
    
    void _render(SDL_Renderer*, float x, float y, const float dpiK, bool isComposition) override;
    
    // may be useful when using RenderShadedMode
    /// use root primary color as this->background_color
    Text& inherit_background_color();
    
    Text& set_style(TextStyle);
    
    Text& set_font(Font &);
    
    Text& set_foreground_color(const Color) override;
        
    Text& regular(const unsigned int = 12);
    
    Text& bold(const unsigned int = 12);
    
    Text& boldOblique(const unsigned int = 12);
    
    Text& extraLight(const unsigned int = 12);
    
    Text& oblique(const unsigned int = 12);
    
    Text& condensed_Bold(const unsigned int = 12);
    
    Text& condensed_BoldOblique(const unsigned int = 12);
    
    Text& condensed_Oblique(const unsigned int = 12);
    
    Text& condensed(const unsigned int = 0);
    
};

#endif /* Text_hpp */
