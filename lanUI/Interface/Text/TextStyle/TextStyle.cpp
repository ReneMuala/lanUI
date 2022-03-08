//
//  TextStyle.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 05/03/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include "TextStyle.hpp"

namespace TextStyles {
    TextStyle Display     = {Font::Style::Bold, 60};
    TextStyle Headline1   = {Font::Style::Bold, 48};
    TextStyle Headline2   = {Font::Style::SemiBold, 36};
    TextStyle Headline3   = {Font::Style::SemiBold, 30};
    TextStyle Headline4   = {Font::Style::SemiBold, 24};
    TextStyle Headline5   = {Font::Style::SemiBold, 20};
    TextStyle CaptionCaps = {Font::Style::Regular, 10};
    TextStyle Caption     = {Font::Style::Regular, 12};
    TextStyle BodyLarge   = {Font::Style::Regular, 20};
    TextStyle BodyMedium  = {Font::Style::Regular, 16};
    TextStyle BodySmall   = {Font::Style::Regular, 14};
    TextStyle BodyArticle = {Font::Style::Oblique, 16};
    TextStyle Default     = BodyMedium;
}
