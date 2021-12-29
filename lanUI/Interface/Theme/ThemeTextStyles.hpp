//
//  ThemeTextStyles.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 11/12/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef ThemeTextStyles_h
#define ThemeTextStyles_h

#include "../Text/Text.hpp"

typedef struct {
    Font::Style font_style;
    int size;
    const std::string toStr() const;
} TextStyle;

// THemeStyles
namespace ThemeTextStyles {
    const TextStyle THSDisplay     = {Font::Style::Bold, 60};
    const TextStyle THSHeadline1   = {Font::Style::Bold, 48};
    const TextStyle THSHeadline2   = {Font::Style::SemiBold, 36};
    const TextStyle THSHeadline3   = {Font::Style::SemiBold, 30};
    const TextStyle THSHeadline4   = {Font::Style::SemiBold, 24};
    const TextStyle THSHeadline5   = {Font::Style::SemiBold, 20};
    const TextStyle THSCaptionCaps = {Font::Style::Regular, 10};
    const TextStyle THSCaption     = {Font::Style::Regular, 12};
    const TextStyle THSBodyLarge   = {Font::Style::Regular, 20};
    const TextStyle THSBodyMedium  = {Font::Style::Regular, 16};
    const TextStyle THSBodySmall   = {Font::Style::Regular, 14};
    const TextStyle THSBodyArticle = {Font::Style::Oblique, 16};
    const TextStyle THSDefault     = THSBodyMedium;
};

#endif /* ThemeTextStyles_h */
