//
//  TextParams.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 05/03/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include "TextParams.hpp"

TextParams::TextParams(){
    surface = cairo_image_surface_create(CAIRO_FORMAT_A1, 1, 1);
    cr = cairo_create(surface);
}

void TextParams::compute_params(CairoFontFace * crface, CairoTextExtents & te, CairoFontExtents & fe, const double size ,const char *source){
    // ?? beg
    cairo_set_font_face(cr, crface);
    cairo_set_font_size(cr, size);
    cairo_font_extents(cr, &fe);
    cairo_text_extents(cr, source, &te);
    // end ??
}

TextParams::~TextParams(){
    if(surface){
        cairo_surface_destroy(surface);
        surface = nullptr;
    } if(cr){
        cairo_destroy(cr);
        cr = nullptr;
    }
}

