
/*
 
 SDL2_gfxPrimitives.h: graphics primitives for SDL
 
 Copyright (C) 2012-2014  Andreas Schiffler
 
 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source
 distribution.
 
 Andreas Schiffler -- aschiffler at ferzkopp dot net
 
 */

#ifndef _SDL2_gfxPrimitives_h
#define _SDL2_gfxPrimitives_h

#include <math.h>
#ifndef M_PI
#define M_PI	3.1415926535897932384626433832795
#endif

//#include "SDL.h"
#include <SDL2/SDL.h>
#include "../../Interface/Color/Color.hpp"

/* ----- Versioning */

#define SDL2_GFXPRIMITIVES_MAJOR	1
#define SDL2_GFXPRIMITIVES_MINOR	0
#define SDL2_GFXPRIMITIVES_MICRO	2


namespace primitives {

/* Note: all ___Color routines expect the color to be in format 0xRRGGBBAA */

/* Pixel */

int pixelColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Color color);
int pixelColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint32 color);
int pixelRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Horizontal line */

int hlineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, Color color);
int hlineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color);
int hlineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Vertical line */

int vlineColor(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2, Color color);
int vlineColor(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color);
int vlineRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Rectangle */

int rectangleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 w, Sint16 h, Color color);
int rectangleColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
int rectangleRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Rounded-Corner Rectangle */

int roundedRectangleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 w, Sint16 h, Sint16 radius, Color color);
int roundedRectangleColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 radius, Uint32 color);
int roundedRectangleRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1,
                         Sint16 x2, Sint16 y2, Sint16 radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled rectangle (Box) */

int boxColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 w, Sint16 h, Color color);
int boxColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
int boxRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Rounded-Corner Filled rectangle (Box) */
int roundedBoxColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 w, Sint16 h, Sint16 radius, Color color);
int roundedBoxColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 radius, Uint32 color);
int roundedBoxRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Line */

int lineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Color color);
int lineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
int lineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Line */
int aalineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Color color);
int aalineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
int aalineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Thick Line */
int thickLineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Color color);
int thickLineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint32 color);
int thickLineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Circle */
int circleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Color color);
int circleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Uint32 color);
int circleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Arc */
int arcColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Color color);
int arcColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Uint32 color);
int arcRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Circle */

int aacircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Color color);
int aacircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Uint32 color);
int aacircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Circle */

int filledCircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 r, Color color);
int filledCircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
int filledCircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Ellipse */

int ellipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Color color);
int ellipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
int ellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Ellipse */

int aaellipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Color color);
int aaellipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
int aaellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Ellipse */
int filledEllipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Color color);
int filledEllipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
int filledEllipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Pie */
int pieColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Color color);
int pieColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Uint32 color);
int pieRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Pie */

int filledPieColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Color color);
int filledPieColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Uint32 color);
int filledPieRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 radius, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Trigon */
int trigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Color color);
int trigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
int trigonRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA-Trigon */
int aatrigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Color color);
int aatrigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
int aatrigonRGBA(SDL_Renderer * renderer,  Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
                 Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Trigon */
int filledTrigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Color color);
int filledTrigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
int filledTrigonRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Polygon */
int polygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Color color);
int polygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
int polygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA-Polygon */
int aapolygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Color color);
int aapolygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
int aapolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Polygon */
int filledPolygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Color color);
int filledPolygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color);
int filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Textured Polygon */

int texturedPolygon(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, SDL_Surface * texture,int texture_dx,int texture_dy);

/* Bezier */
int bezierColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, int s, Color color);
int bezierColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, int s, Uint32 color);
int bezierRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
}

#endif				/* _SDL2_gfxPrimitives_h */

