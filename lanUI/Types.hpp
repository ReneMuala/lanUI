//
//  Types.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 19/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Types_hpp
#define Types_hpp

#include <SDL2/SDL.h>
#include <functional>
#include "CairoSDL/cairosdl.h"

typedef SDL_FRect Rect;
typedef SDL_Event Event;
typedef SDL_Texture Texture;
typedef SDL_Surface Surface;
typedef SDL_Renderer Renderer;
typedef cairo_t CairoContext;
typedef cairo_surface_t CairoSurface;
typedef cairo_pattern_t CairoPattern;
typedef cairo_font_face_t CairoFontFace;
typedef cairo_font_extents_t CairoFontExtents;
typedef cairo_text_extents_t CairoTextExtents;
typedef double Angle;
typedef long FrameCount;
typedef std::function<void()> VoidCallback;
typedef std::function<bool()> BoolCallback;

#endif /* Types_h */
