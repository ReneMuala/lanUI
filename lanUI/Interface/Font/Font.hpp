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
        Bold,
        BoldOblique,
        ExtraLight,
        Oblique,
        Condensed_Bold,
        Condensed_BoldOblique,
        Condensed_Oblique,
        Condensed,
        totalStyles,
    } Style;

private:
    std::string path_copy[Style::totalStyles];
public:
    
    bool ready;
    
    int size, scalingFactorConstant;
    
    size_t id_inAllFonts;
    
    Style style;
    
    Semaphore<TTF_Font*> ttfFont;
    
    Font();
    
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
    Font& fromFile(const char * path, Style style = Style::Regular);
    
    Font& set_scaling_factor(const int constant);
};

/** LanUI defaulf fonts
 */
namespace Fonts {
    extern Font DejaVuSans;
}

#endif /* Font_hpp */
