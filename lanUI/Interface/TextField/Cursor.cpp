//
//  Cursor.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 23/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include <stdio.h>
#include "TextField.hpp"

TextField::TextSurface::Cursor::Cursor(): line(0), colummn(0), color(Colors::Dark_green), empty(true), hidden(true),active(false), size({0,0,0,0}), weight(2) {}

void TextField::TextSurface::Cursor::_render(SDL_Renderer * renderer, float x, float y, const float dpiK){
    if(active && !hidden){
        size.x = ((x+xBuffer) * dpiK);
        size.y = (y * dpiK);
        size.w = (weight * dpiK);
        size.h = (hBuffer * dpiK);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRectF(renderer, &size);
    }
}

