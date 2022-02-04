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
#include "../Text/Text.hpp"
#include "../Theme/ThemeTextStyles.hpp"

namespace TextStyles {
    extern TextStyle Display;
    extern TextStyle Headline1;
    extern TextStyle Headline2;
    extern TextStyle Headline3;
    extern TextStyle Headline4;
    extern TextStyle Headline5;
    extern TextStyle CaptionCaps;
    extern TextStyle Caption;
    extern TextStyle BodyLarge;
    extern TextStyle BodyMedium;
    extern TextStyle BodySmall;
    extern TextStyle BodyArticle;
    extern TextStyle Default;
}

class Text: public Object {
public:
    
    enum class TTF_api_style : int {
        Normal = TTF_STYLE_NORMAL,
        Light = TTF_STYLE_ITALIC,
        Bold = TTF_STYLE_BOLD,
        Underline = TTF_STYLE_UNDERLINE,
        StrikeThrough = TTF_STYLE_STRIKETHROUGH,
    };
    
private:
    bool withBackground;
    Semaphore<TTF_api_style> ttf_api_style;
    SDL_Surface * surfc;
public:
    Semaphore<std::string> source;
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
    
    Text(const std::string source = "", Font& font = FontsSharedData::DefaultFonts);
    
    ~Text();
    
    void _delete_custom_data() override;
    
    void _free_font();
    
    /// Creates a text image from std::string, set renderer when an imediate text output is required
    Text& from_string(const std::string source , Renderer * renderer = nullptr);
    
    void tryCompile();
        
    void _adjustTextDPI();
    
    Text& set_font_style(const Font::Style, unsigned int const = 12);
    
    bool _compile_canvas_prepare_font();
    
    bool _compile_canvas_render_text(Renderer*);

    void _compile_canvas_free_surfc();
    
    bool compile_canvas(Renderer*);
    
    void _compute_text_size();
    
    void _compile(Renderer*, const float dpiK) override;
    
    void _render_background(SDL_Renderer*, Rect*);
    
    void _render(const unsigned int, SDL_Renderer*, float x, float y, const float dpiK, bool isComposition) override;
    
    // may be useful when using RenderShadedMode
    /// use root primary color as this->background_color
    Text& inherit_background_color();
    
    Text& set_ttf_api_style(const TTF_api_style);
    
    Text& set_style(const TextStyle);
    
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
    
    Text& condensed(const unsigned int = 12);
    
    Text& black(const unsigned int = 12);
    
    Text& blackItalic(const unsigned int = 12);

    Text& boldItalic(const unsigned int = 12);
    
    Text& extraLightItalic(const unsigned int = 12);
    
    Text& extraBold(const unsigned int = 12);
    
    Text& extraBoldItalic(const unsigned int = 12);
        
    Text& italic(const unsigned int = 12);
    
    Text& light(const unsigned int = 12);
    
    Text& lightItalic(const unsigned int = 12);
    
    Text& medium(const unsigned int = 12);
    
    Text& mediumItalic(const unsigned int = 12);
    
    Text& semiBold(const unsigned int = 12);
    
    Text& semiBoldItalic(const unsigned int = 12);
    
    Text& thin(const unsigned int = 12);
    
    Text& thinItalic(const unsigned int = 12);
    
};

#endif /* Text_hpp */
