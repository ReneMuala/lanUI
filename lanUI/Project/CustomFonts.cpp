//
//  CustomFonts.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 02/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "CustomFonts.hpp"

namespace CustomFonts
{
    /*
     Here you can declare your custom fonts.
     */
Font Lobster;

/*!
 In lanUI your custom fonts must to be loaded manually, insert the statements in this funcion.
 */
void _loadCustomFonts(){
    Lobster.fromFile("lanUi.Bundle/System/Library/Fonts/Lobster-Regular.ttf", Font::Style::Regular);
}

}
