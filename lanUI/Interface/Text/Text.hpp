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
#include "../Font/ProgramFonts.hpp"
#include "TextStyle/TextStyle.hpp"

class Text: public Object {
public:
    
    typedef enum {
        Normal,
        Underline,
        StrikeThrough,
    } TextEffect;
    
protected:
    
    BSemaphore globalSemaphore;
    TextEffect textEffect;
    std::string source;
    Font font;
    int32_t windowId;
    FontFace fontFace;
    CairoFontExtents fontExtents;
    CairoTextExtents textExtents;
    CairoFontFace * cairoFontFace;
    
    Text& set_size(const float size_w, const float size_h) override;
    
    virtual void default_composer_callback();
    
public:

    void _compute_text_size();
    
    bool _allocate_cairo_font_face();
    
    void _free_cairo_font_face();
        
    void _compose(Renderer*, const float dpiK, const int32_t windowId) override;
    
    Text(const std::string source, Font& font = Fonts::DefaultFonts);
    
    Text& from_string(const std::string);
    
    Text& set_text_effect(const TextEffect);
    
    Text& set_style(const TextStyle);
    
    Text& set_font(Font &);
    
    /// Sets the height of the font
    Text& set_font_size(const double size);
    
    Text& set_font_style(const Font::Style FontStyle, const Font::Style alternative_style = Font::Regular);
    
    Text& set_foreground_color(const Color) override;
    
    Text& regular();
    
    Text& bold();
    
    Text& boldOblique();
    
    Text& extraLight();
    
    Text& oblique();
    
    Text& condensed_Bold();
    
    Text& condensed_BoldOblique();
    
    Text& condensed_Oblique();
    
    Text& condensed();
    
    Text& black();
    
    Text& blackItalic();

    Text& boldItalic();
    
    Text& extraLightItalic();
    
    Text& extraBold();
    
    Text& extraBoldItalic();
        
    Text& italic();
    
    Text& light();
    
    Text& lightItalic();
    
    Text& medium();
    
    Text& mediumItalic();
    
    Text& semiBold();
    
    Text& semiBoldItalic();
    
    Text& thin();
    
    Text& thinItalic();
    
};

#endif /* Text_hpp */
