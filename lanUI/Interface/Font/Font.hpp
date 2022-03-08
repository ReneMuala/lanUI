//
//  Font.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Font_hpp
#define Font_hpp

#include <string>
#include "../../Semaphore/Semaphore.hpp"
#include "FontFace.hpp"
#include <list>
#include <utility>

/** Text Font.
 Stores font configurations and static font styles that can later result in a Freetype2 font face.
 Setup your custum fonts in Project/CustomFonts/ (BEFORE TEMPLATE CREATION)
 */
class Font {
public:
    typedef enum {
        Regular,
        Black,
        BlackItalic,
        Bold,
        BoldOblique,
        BoldItalic,
        Condensed_Bold,
        Condensed_BoldOblique,
        Condensed_Oblique,
        Condensed,
        ExtraLight,
        ExtraLightItalic,
        ExtraBold,
        ExtraBoldItalic,
        Oblique,
        Italic,
        Light,
        LightItalic,
        Medium,
        MediumItalic,
        SemiBold,
        SemiBoldItalic,
        Thin,
        ThinItalic,
        totalStyles,
    } Style;
    
private:
    
    std::string filenames[Style::totalStyles];
    
    std::string name;
    
    double size;
    
    Style style;

public:
    
    Font(const std::string name = "", std::list<std::pair<const std::string,const Style>> = {});
    
    Font(const Font& other);
    
    ~Font();
    
    /// a font is valid and can be used when its Regular style is loaded.
    bool is_valid() const;
    
    bool is_style_loaded(const Style style) const;
    
    const void operator=(const Font&other);
    
    const double get_size();
    
    Font& set_size(const double size);
    
    const Style get_style();
    
    const std::string get_name();
    
    /**Sets font style and size.
     Invalid font styles may generate warnings.
     */
    Font& set_style(const Style style , const Style alternative_style = Regular);
    
    /**Loads font data from a file.
    DON'T FORGET to load the Regular style, it will be necessary to handle erros.
     */
    Font& load_style(const std::string filename, const Style style);
        
    /** Sets the global name for this font, allowing it to be explicitly used in Paragrafs.
     */
    Font& set_global_name(const std::string);
    
    static Font* _get_font_by_global_name(const std::string);
    
    /**
     Generate freetype2 font face.
     @throws runtime_error: if Freetype2 can't open the specific face or if the current style is invalid.
     */
    bool generate_ft_face(FontFace & ft_face, const int32_t windowId);
    
    void print_avaliable_styles() const;
    
};

#endif /* Font_hpp */
