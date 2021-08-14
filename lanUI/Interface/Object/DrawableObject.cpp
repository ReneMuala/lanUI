//
//  DrawableObject.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"
#include "Core.hpp"
#include <SDL2/SDL_image.h>
#include <map>

namespace DrawableObjectsData {
Semaphore<std::map<const char *, SDL_Surface*>> surfaces;
}

SDL_Surface * get_surface(const char * source){
    SDL_Surface * surfc = nullptr;
    if((surfc = DrawableObjectsData::surfaces.get()[source])){
        DrawableObjectsData::surfaces.leave();
    } else if((surfc = IMG_Load(source))){
        DrawableObjectsData::surfaces.data[source] = surfc;
    } else {
        Core::log(Core::Warning, (std::string("Unable to load image source from path: ") + source).c_str());
    } DrawableObjectsData::surfaces.leave();
    return surfc;
}

void DrawableObject::_render__image(SDL_Renderer * renderer, float x, float y, const float dpiK){
    size.hold();
    rect_buffer.hold();
    rect_buffer.data = {
        (size.data.x)*dpiK,
        (size.data.y)*dpiK,
        (size.data.w)*dpiK,
        (size.data.h)*dpiK
    };
    padding.leave();
    size.leave();
    SDL_RenderCopyExF(renderer, image.get(), NULL, &rect_buffer.data, angle.get(), NULL, SDL_FLIP_NONE);
    //SDL_RenderCopyF(renderer, image.get(), NULL, &rect_buffer.data);
    image.leave();
    angle.leave();
    if(withBorder)
        _render__border(renderer, (Rect*)&rect_buffer.data);
    rect_buffer.leave();
}

void DrawableObject::_render__colorScheme(SDL_Renderer * renderer, float x, float y, const float dpiK){
    size.hold();
    Rect rect =
    {
        (size.data.x)*dpiK,
        (size.data.y)*dpiK,
        (size.data.w)*dpiK,
        (size.data.h)*dpiK,
    };
    size.leave(); padding.leave();
    primaryColor.hold();
    SDL_SetRenderDrawColor(renderer, primaryColor.data.r, primaryColor.data.g, primaryColor.data.b, primaryColor.data.a);
    primaryColor.leave();
    SDL_RenderFillRectF(renderer, &rect);
    _render__border(renderer, &rect);
}

void DrawableObject::_render__default(SDL_Renderer * renderer, float x, float y, const float dpiK) {
    size.hold(); padding.hold();
    Rect rect =
    {
        (size.data.x)*dpiK,
        (size.data.y)*dpiK,
        (size.data.w)*dpiK,
        (size.data.h)*dpiK,
    };
    size.leave(); padding.leave();
    _render__colorScheme(renderer, x, y, dpiK);
    SDL_RenderDrawLineF(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
    SDL_RenderDrawLineF(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y);
}

DrawableObject::DrawableObject(): image(nullptr), withBorder(false), primaryColor(Colors::Primary), secondaryColor(Colors::Secondary), angle(0), drawMode(DrawMode::DefaultMode) {
    Object();
    _clear_properties();
    properties[Properties::isDrawable].set(true);
    default_animation.get()._using = false;
    default_animation.data.delay = 0;
    default_animation.leave();
}

DrawableObject::~DrawableObject(){
    _freeImage();
}

void DrawableObject::_freeImage(){
    if (image.get())
        SDL_DestroyTexture(image.data);
    image.data = nullptr;
    image.leave();
}

DrawableObject& DrawableObject::fromFile(const char *filename, Renderer * renderer){
    fromSurface(get_surface(filename), renderer);
    return (*this);
}

DrawableObject& DrawableObject::fromSurface(Surface * surfc, Renderer * renderer, const bool reset_secondaryColor){
    if(surfc) {
        _freeImage();
        this->image.set(SDL_CreateTextureFromSurface(renderer, surfc));
        this->renderer.set(renderer);
        size.set({0,0,(float)surfc->w, (float)surfc->h});
        drawMode.set(DrawMode::ImageMode);
        // ocultes the border / secondary color
        if(reset_secondaryColor)
            secondaryColor.set(Colors::Transparent);
    } else {
        Core::log(Core::Warning, "Unable to load image from image source (nullptr)");
    }
    return (*this);
}

DrawableObject& DrawableObject::fromColorScheme(const Color color, const Color border){
    primaryColor.set(color);
    secondaryColor.set(border);
    drawMode.set(DrawMode::ColorSchemeMode);
    return (*this);
}

DrawableObject& DrawableObject::set_primary_color(const Color color){
    primaryColor.set(color);
    return (*this);
}

DrawableObject& DrawableObject::set_secondary_color(const Color color){
    secondaryColor.set(color);
    return (*this);
}

DrawableObject& DrawableObject::set_border_color(const Color color){
    secondaryColor.set(color);
    withBorder = true;
    return (*this);
}

void DrawableObject::_render__border(SDL_Renderer * renderer, Rect * rect){
    secondaryColor.hold();
    SDL_SetRenderDrawColor(renderer, secondaryColor.data.r, secondaryColor.data.g, secondaryColor.data.b, secondaryColor.data.a);
    secondaryColor.leave();
    SDL_RenderDrawRectF(renderer, rect);
}

void DrawableObject::_render(SDL_Renderer * renderer, float x, float y, const float dpiK){
    if(_inRootBounds(x, y)){
        _align(x, y);
        size.hold(); padding.hold(); size.data.x = x + padding.data.left; size.data.y = y + padding.data.top; size.leave(); padding.leave();
        switch (drawMode.get()) {
            case DrawMode::ImageMode:
                if(renderer == this->renderer.get()){
                    this->renderer.leave();
                    _render__image(renderer, x, y, dpiK);
                } else {
                    this->renderer.leave();
                    _render__default(renderer, x, y, dpiK);
                } break;
            case DrawMode::ColorSchemeMode:
                _render__colorScheme(renderer, x, y, dpiK);
                break;
            default:
                _render__default(renderer, x, y, dpiK);
                break;
        } drawMode.leave();
        _renderEmbedded(renderer, x, y, dpiK, _RenderEmbeddedMode::_renderOnlyNextInZ);
    } _renderEmbedded(renderer, x, y, dpiK, _RenderEmbeddedMode::_renderOnlyNextInX_Y);
}

void DrawableObject::_run_default_animation(){
    static FrameCount delay(0);
    default_animation.hold();
    
    if(default_animation.data._using){
        if(delay >= default_animation.data.delay){
            delay = 0;
            //default_animation.data._using =
            default_animation.data.callback();
        } delay++;
    }
    default_animation.leave();
    _run_others_default_animation();
}

DrawableObject& DrawableObject::set_default_animation(const FrameCount delay, BoolCallback callback){
    default_animation.hold();
    default_animation.data._using = true;
    default_animation.data.delay = delay;
    default_animation.data.callback = callback;
    default_animation.leave();
    return (*this);
}
