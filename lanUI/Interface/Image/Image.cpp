//
//  Image.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 04/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Image.hpp"
#include "../../Core/Core.hpp"
#include <thread>
#include <math.h>

const Color Image::_getGradientFrameColor(GradientElement first, GradientElement second, double y, double height){
    // Special thanks to: Bill_Kendrick (https://discourse.libsdl.org/u/Bill_Kendrick)
    Uint8 r = (second.second * (second.first.r * y / height)) + (first.second * (first.first.r * (height - y) / height));
    Uint8 g = (second.second * (second.first.g * y / height)) + (first.second * (first.first.g * (height - y) / height));
    Uint8 b = (second.second * (second.first.b * y / height)) + (first.second * (first.first.b * (height - y) / height));
    return {r,g,b,0xff};
}

Image& Image::fromLinearGradient(LGOrietantion orientation, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window){
    _freeImage();
    image.set(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, size.get().w, size.data.h));
    size.leave();
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, image.data);
    int _w = size.get().w, _h = size.data.h; size.leave();
    Rect linearFill = {0,0, (orientation == Vertical) ? (float)(_w) : 1, (orientation == Horizontal) ? (float)(_h) : 1};
    Color temp;
    
    for(int i = 0 ; i < ((orientation == Vertical) ? _h : _w) ; i++){
        temp = _getGradientFrameColor(first, second, i, ((orientation == Vertical) ? _h : _w));
        SDL_SetRenderDrawColor(renderer, temp.r, temp.g, temp.b, 255);
        if(orientation == Vertical)
            linearFill.y = i;
        else
            linearFill.x = i;
        SDL_RenderFillRectF(renderer, &linearFill);
    }
    drawMode.set(DrawMode::ImageMode);
    
    backgroundColor.set(Colors::Transparent);
    this->renderer.set(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    
    return (*this);
}

const double Image::_getDistance(Point point1, Point point2){
    return  pow(
                pow((point1.x - point2.x), 2) +
                pow((point1.y - point2.y), 2)
                , 0.5);
}

Image& Image::fromRadialGradient(Point center, double radius, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window){
    _freeImage();
    image.set(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, size.get().w, size.data.h));
    size.leave();
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, image.data);
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
            SDL_SetRenderDrawColor(renderer, temp.r, temp.g, temp.b, 255);
            SDL_RenderDrawPointF(renderer, x, y);
        }
    } drawMode.set(DrawMode::ImageMode);
    
    backgroundColor.set(Colors::Transparent);
    this->renderer.set(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    return (*this);
}

Image& Image::set_angle(const Angle angle){
    this->angle.set(angle);
    return (*this);
}
