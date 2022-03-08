//
//  ProgramFonts.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 05/03/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include "ProgramFonts.hpp"

namespace Fonts
{
Font DejaVuSans
 ("DejavuSans",{
     {"Fonts:DejaVuSans.ttf", Font::Style::Regular},
     {"Fonts:DejaVuSans-Bold.ttf", Font::Style::Bold},
     {"Fonts:DejaVuSans-BoldOblique.ttf", Font::Style::BoldOblique},
     {"Fonts:DejaVuSans-ExtraLight.ttf", Font::Style::ExtraLight},
     {"Fonts:DejaVuSans-Oblique.ttf", Font::Style::Oblique},
     {"Fonts:DejaVuSansCondensed-Bold.ttf", Font::Style::Condensed_Bold},
     {"Fonts:DejaVuSansCondensed-BoldOblique.ttf", Font::Condensed_BoldOblique},
     {"Fonts:DejaVuSansCondensed.ttf", Font::Style::Condensed}
 }), WorkSans
("WorkSans",{
    {"Fonts:WorkSans-Black.ttf", Font::Black},
    {"Fonts:WorkSans-BlackItalic.ttf", Font::BlackItalic},
    {"Fonts:WorkSans-Bold.ttf", Font::Bold},
    {"Fonts:WorkSans-BoldItalic.ttf", Font::BoldItalic},
    {"Fonts:WorkSans-ExtraBold.ttf", Font::ExtraBold},
    {"Fonts:WorkSans-ExtraBoldItalic.ttf", Font::ExtraBoldItalic},
    {"Fonts:WorkSans-ExtraLight.ttf", Font::ExtraLight},
    {"Fonts:WorkSans-ExtraLightItalic.ttf", Font::ExtraLightItalic},
    {"Fonts:WorkSans-Italic.ttf", Font::Italic},
    {"Fonts:WorkSans-Light.ttf", Font::Light},
    {"Fonts:WorkSans-LightItalic.ttf", Font::LightItalic},
    {"Fonts:WorkSans-Medium.ttf", Font::Medium},
    {"Fonts:WorkSans-MediumItalic.ttf", Font::MediumItalic},
    {"Fonts:WorkSans-Regular.ttf", Font::Regular},
    {"Fonts:WorkSans-SemiBold.ttf", Font::SemiBold},
    {"Fonts:WorkSans-SemiBoldItalic.ttf", Font::SemiBoldItalic},
    {"Fonts:WorkSans-Thin.ttf", Font::Thin},
    {"Fonts:WorkSans-ThinItalic.ttf", Font::ThinItalic},
}), OpenSans
("OpenSans",{
    {"Fonts:OpenSans-Bold.ttf", Font::Bold},
    {"Fonts:OpenSans-BoldItalic.ttf", Font::BoldItalic},
    {"Fonts:OpenSans-ExtraBold.ttf", Font::ExtraBold},
    {"Fonts:OpenSans-ExtraBoldItalic.ttf", Font::ExtraBoldItalic},
    {"Fonts:OpenSans-Italic.ttf", Font::Italic},
    {"Fonts:OpenSans-Light.ttf", Font::Light},
    {"Fonts:OpenSans-LightItalic.ttf", Font::LightItalic},
    {"Fonts:OpenSans-Medium.ttf", Font::Medium},
    {"Fonts:OpenSans-MediumItalic.ttf", Font::MediumItalic},
    {"Fonts:OpenSans-Regular.ttf", Font::Regular},
    {"Fonts:OpenSans-SemiBold.ttf", Font::SemiBold},
    {"Fonts:OpenSans-SemiBoldItalic.ttf", Font::SemiBoldItalic},
}),

/*
 Here you can declare your custom fonts has extern variables
 to access them from your code.
 */

Lobster
("Lobster",{
    {"Fonts:Lobster-Regular.ttf", Font::Style::Regular},
}),


DefaultFonts = WorkSans;
}
