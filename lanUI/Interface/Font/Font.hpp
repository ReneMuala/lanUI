//
//  Font.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Font_hpp
#define Font_hpp

#include <SDL2/SDL_ttf.h>
#include <string>
#include "../../Core/Core.hpp"


/** Text Font.
 Can be loaded from standard font formats (TTF, OTF,...).
 By default lanUI uses Dejavu sans, witch are loaded in the Core/Core.cpp file.
 You can setup your custum fonts in Project/CurstomFonts/ (BEFORE TEMPLATE CREATION)
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
    std::string path_copy[Style::totalStyles];
public:
    
    std::string name;
        
    int size, scalingFactorConstant;
    
    size_t id_inAllFonts;
    
    Style style;
    
    Semaphore<TTF_Font*> ttfFont;
    
    Font(Font& other) = delete;
    
    Font(const std::string name = "");
    
    void _init(const std::string name = "");
    
    ~Font();
    
    /**Free font.
     Clears font data.
     */
    void free();
    
    bool _test(const char * path);
    
    bool _load(const char * path, const int size);
        
    const void operator=(Font&other);
    
    /**Sets font style and size.
     Invalid font styles may generate warnings.
     */
    Font& set_style(const Style style, const int size = 0);
    
    /**Loads font data from a file.
    DON'T FORGET to load the Regular style, it will be necessary to handle erros.
     */
    Font& fromFile(std::string path, Style style = Style::Regular);
    
    Font& set_scaling_factor(const int constant);
    
    /** Sets the global name for this font, allowing it to be explicitly used in Paragrafs.
     */
    Font& set_global_name(const std::string);
    
    static Font* _get_font_by_global_name(const std::string);
    
    void print_avaliable_styles() const;
    
    
};

/** LanUI defaulf fonts
 */
namespace Fonts {
    extern Font DejaVuSans, WorkSans, OpenSans, DefaultFonts;
    extern void set_default_font();
}

#endif /* Font_hpp */
