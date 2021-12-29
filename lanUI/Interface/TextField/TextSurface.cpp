//
//  TextSurface.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 23/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include <stdio.h>
#include "TextField.hpp"

BSTextField::TextSurface::TextSurface(){
    
    set_alignment(Left);
    set_padding({0,5});
    
    set_default_animation
    (60,CallbackExpr({
        cursor.get().hidden = !cursor.data.hidden;
        cursor.leave();
        return true;
    }));
    
}

void BSTextField::TextSurface::_renderEmbedded(SDL_Renderer * renderer, const float x, const float y, float dpiK, _RenderEmbeddedMode _renderEmbeddedMode){
    
    const Padding padding_buffer = padding.get();
    const Rect size_buffer = size.get();
    const ScrollingFactor scrollingFactor_buffer = scrollingFactor.get();
    
    padding.leave();
    size.leave();
    scrollingFactor.leave();
    
    const float nextInZ_x = x + padding_buffer.left + scrollingFactor_buffer.horizontal,
    nextInZ_y = y + padding_buffer.top + scrollingFactor_buffer.vertical;
    
    _lock_renderer_in_bounds(renderer, dpiK);

    if(!(_renderEmbeddedMode & _renderOnlyNextInX_Y))
    {
        if(nextInZ.get())
            __renderEmbedded_routine(renderer, nextInZ.data,
                                     nextInZ_x,
                                     nextInZ_y,
                                     dpiK);
        nextInZ.leave();
    }
    _unlock_renderer_from_bounds(renderer);

    cursor.get()._render(renderer, nextInZ_x, y, dpiK);
    cursor.leave();
    
    if(!(_renderEmbeddedMode & _renderOnlyNextInZ))
    {
        if(nextInX.get())
            __renderEmbedded_routine(renderer, nextInX.data,
                                     x + size_buffer.w + padding_buffer.left + padding_buffer.right,
                                     y,
                                     dpiK);
        
        nextInX.leave();
        
        if(nextInY.get())
            __renderEmbedded_routine(renderer, nextInY.data,
                                     x,
                                     y + size_buffer.h + padding_buffer.top + padding_buffer.bottom,
                                     dpiK);
        nextInY.leave();
    }
}

void BSTextField::TextSurface::_render(SDL_Renderer * renderer, float x, float y, const float dpiK, bool isComposition){
    Text::_render(renderer, x, y, dpiK, isComposition);
}
