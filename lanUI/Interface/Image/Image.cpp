//
//  Image.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 04/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Image.hpp"
#include "../Window/WindowManager/WindowManager.hpp"
#include <thread>
#include <math.h>

const Color BSImage::_getGradientFrameColor(GradientElement first, GradientElement second, double y, double height){
    // Special thanks to: Bill_Kendrick (https://discourse.libsdl.org/u/Bill_Kendrick)
    Uint8 r = (second.second * (second.first.r * y / height)) + (first.second * (first.first.r * (height - y) / height));
    Uint8 g = (second.second * (second.first.g * y / height)) + (first.second * (first.first.g * (height - y) / height));
    Uint8 b = (second.second * (second.first.b * y / height)) + (first.second * (first.first.b * (height - y) / height));
    Uint8 a = (second.second * (second.first.a * y / height)) + (first.second * (first.first.a * (height - y) / height));
    return {r,g,b,a};
}

BSImage& BSImage::fromLinearGradient(LGOrietantion orientation, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window, uint32_t weight, uint32_t gap){
    _free_canvas();
    canvas.set(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.get().w, size.data.h));
    size.leave();
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, canvas.data);
    int _w = size.get().w, _h = size.data.h; size.leave();
    Rect linearFill = {0,0, (orientation == Vertical) ? (float)(_w) : weight, (orientation == Horizontal) ? (float)(_h) : weight};
    Color temp;
    SDL_SetTextureBlendMode(canvas.data, SDL_BLENDMODE_BLEND);
    if(!weight)
        WindowManager::log(WindowManager::Error, "Gradient weight must to be >= 1");
    
    for(int i = 0 ; i < ((orientation == Vertical) ? _h : _w) ; i+=(weight+gap)){
        temp = _getGradientFrameColor(first, second, i, ((orientation == Vertical) ? _h : _w));
        
        SDL_SetRenderDrawColor(renderer, temp.r, temp.g, temp.b, temp.a);
        if(orientation == Vertical)
            linearFill.y = i;
        else
            linearFill.x = i;
        SDL_RenderFillRectF(renderer, &linearFill);
    }
    renderMode.set(RenderMode::ImageMode);
    
    backgroundColor.set(Colors::Transparent);
    this->renderer.set(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    
    return (*this);
}

const double BSImage::_getDistance(Point point1, Point point2){
    return  pow(
                pow((point1.x - point2.x), 2) +
                pow((point1.y - point2.y), 2)
                , 0.5);
}

BSImage& BSImage::fromRadialGradient(Point center, double radius, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window){
    _free_canvas();
    canvas.set(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.get().w, size.data.h));
    size.leave();
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, canvas.data);
    SDL_SetTextureBlendMode(canvas.data, SDL_BLENDMODE_BLEND);
    int _w = size.get().w, _h = size.data.h; size.leave();
    double distance = 0;
    Color temp;
    Point temp_point;
    
    for(int y = 0 ; y < _h ; y++){
        for(int x = 0; x < _w ; x++){
            temp_point.x = x;
            temp_point.y = y;
            distance = _getDistance(center, temp_point);
            
            temp = _getGradientFrameColor(first, second, (distance < radius) ? distance : radius , radius);
            
            SDL_SetRenderDrawColor(renderer, temp.r, temp.g, temp.b, temp.a);
            SDL_RenderDrawPointF(renderer, x, y);
        }
    } renderMode.set(RenderMode::ImageMode);
    
    backgroundColor.set(Colors::Transparent);
    this->renderer.set(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    return (*this);
}

BSImage& BSImage::set_angle(const Angle angle){
    this->angle.set(angle);
    return (*this);
}

void BSImage::_text_surface_operation(){
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif
    
    size.hold();
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, size.data.w, size.data.h, 32, rmask , gmask, bmask, amask);
    SDL_RenderReadPixels(renderer.get(), NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    renderer.leave();
    SDL_SaveBMP(sshot, "screenshot.bmp");
    SDL_FreeSurface(sshot);
}
