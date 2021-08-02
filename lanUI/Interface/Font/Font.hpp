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
    // all childrens are nullptrs
public:
    
    int size;
    
    Style style;

    TTF_Font* child;
    
    Font();
    
    void free();
    
    bool _test(const char * path);
    
    bool _load(const char * path, const int size);
        
    const void operator=(Font&other);
    
    Font& set_style(const Style, const int size = 0);
    
    Font& fromFile(const char * path, Style style = Style::Regular);
};

namespace Fonts {
    extern Font DejaVuSans;
}

#endif /* Font_hpp */
