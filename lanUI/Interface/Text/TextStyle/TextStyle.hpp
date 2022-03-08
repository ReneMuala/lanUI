//
//  TextStyle.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 05/03/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef TextStyle_hpp
#define TextStyle_hpp

#include <stdio.h>
#include "../../Font/Font.hpp"
#include <string>

typedef struct {
    Font::Style font_style;
    int size;
    const std::string toStr() const;
} TextStyle;

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

#endif /* TextStyle_hpp */
