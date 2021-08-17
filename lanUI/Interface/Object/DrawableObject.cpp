//
//  DrawableObject.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"
#include "../../Core/Core.hpp"
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
    _render_routine(dpiK);
    sizeBuffer.hold();
    if(withBackground)
        _render__background(renderer, &sizeBuffer.data);
    SDL_RenderCopyExF(renderer, image.get(), NULL, &sizeBuffer.data, angle.get(), NULL, SDL_FLIP_NONE);
    //SDL_RenderCopyF(renderer, image.get(), NULL, &sizeBuffer.data);
    image.leave();
    angle.leave();
    if(withBorder)
        _render__border(renderer, &sizeBuffer.data);
    sizeBuffer.leave();
}

void DrawableObject::_render__colorScheme(SDL_Renderer * renderer, float x, float y, const float dpiK){
    _render_routine(dpiK);
    foregroundColor.hold();
    SDL_SetRenderDrawColor(renderer, foregroundColor.data.r, foregroundColor.data.g, foregroundColor.data.b, foregroundColor.data.a);
    foregroundColor.leave();
    SDL_RenderFillRectF(renderer, &sizeBuffer.get());
    if(withBorder) _render__border(renderer, &sizeBuffer.data);
    sizeBuffer.leave();
}

void DrawableObject::_render__default(SDL_Renderer * renderer, float x, float y, const float dpiK) {
    _render__colorScheme(renderer, x, y, dpiK);
    sizeBuffer.hold();
    SDL_RenderDrawLineF(renderer, sizeBuffer.data.x, sizeBuffer.data.y, sizeBuffer.data.x + sizeBuffer.data.w, sizeBuffer.data.y + sizeBuffer.data.h);
    SDL_RenderDrawLineF(renderer, sizeBuffer.data.x, sizeBuffer.data.y + sizeBuffer.data.h, sizeBuffer.data.x + sizeBuffer.data.w, sizeBuffer.data.y);
    sizeBuffer.leave();
}

DrawableObject::DrawableObject(): image(nullptr), withBackground(false), withBorder(false), foregroundColor(Colors::Primary), backgroundColor(Colors::Secondary), borderColor(Colors::Transparent), angle(0), drawMode(DrawMode::DefaultMode) {
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

DrawableObject& DrawableObject::fromSurface(Surface * surfc, Renderer * renderer, const bool reset_backgroundColor){
    if(surfc) {
        _freeImage();
        this->image.set(SDL_CreateTextureFromSurface(renderer, surfc));
        this->renderer.set(renderer);
        size.set({0,0,(float)surfc->w, (float)surfc->h});
        drawMode.set(DrawMode::ImageMode);
        // ocultes the border / secondary color
        if(reset_backgroundColor)
            backgroundColor.set(Colors::Transparent);
    } else {
        Core::log(Core::Warning, "Unable to load image from image source (nullptr)");
    }
    return (*this);
}

DrawableObject& DrawableObject::fromColorScheme(const Color color, const Color color2){
    foregroundColor.set(color);
    backgroundColor.set(color2);
    withBackground = true;
    drawMode.set(DrawMode::ColorSchemeMode);
    return (*this);
}

DrawableObject& DrawableObject::set_foreground_color(const Color color){
    foregroundColor.set(color);
    return (*this);
}

DrawableObject& DrawableObject::set_background_color(const Color color){
    backgroundColor.set(color);
    withBackground = true;
    return (*this);
}

DrawableObject& DrawableObject::set_border_color(const Color color){
    borderColor.set(color);
    withBorder = true;
    return (*this);
}

void DrawableObject::_render__background(SDL_Renderer * renderer, Rect * rect){
    backgroundColor.hold();
    SDL_SetRenderDrawColor(renderer, backgroundColor.data.r, backgroundColor.data.g, backgroundColor.data.b, backgroundColor.data.a);
    backgroundColor.leave();
    SDL_RenderFillRectF(renderer, rect);
}

void DrawableObject::_render__border(SDL_Renderer * renderer, Rect * rect){
    borderColor.hold();
    SDL_SetRenderDrawColor(renderer, borderColor.data.r, borderColor.data.g, borderColor.data.b, borderColor.data.a);
    borderColor.leave();
    SDL_RenderDrawRectF(renderer, rect);
}

void DrawableObject::_render_routine(float dpiK){
    sizeBuffer.hold();
    sizeBuffer.data = {
        ((size.get().x+scrollingFactor.get().horizontal)*dpiK),
        ((size.data.y+scrollingFactor.data.vertical)*dpiK),
        (size.data.w)*dpiK,
        (size.data.h)*dpiK
    };
    scrollingFactor.leave();
    size.leave();
    sizeBuffer.leave();
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
