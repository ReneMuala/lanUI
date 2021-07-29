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

namespace CoreData {
extern Semaphore<std::map<const char *, SDL_Surface*>> surfaces;
}

SDL_Surface * get_surface(const char * source){
    SDL_Surface * surfc = nullptr;
    if((surfc = CoreData::surfaces.get()[source])){
        CoreData::surfaces.leave();
    } else if((surfc = IMG_Load(source))){
        CoreData::surfaces.data[source] = surfc;
    } else {
        Core::log(Core::Warning, (std::string("Unable to load image source from path: ") + source).c_str());
    } CoreData::surfaces.leave();
    return surfc;
}

DrawableObject& DrawableObject::fromFile(const char *filename, Renderer * renderer){
    fromSurface(get_surface(filename), renderer);
    return (*this);
}

DrawableObject& DrawableObject::fromSurface(Surface * surfc, Renderer * renderer){
    if(surfc) {
        this->image.set(SDL_CreateTextureFromSurface(renderer, surfc));
        this->renderer.set(renderer);
        size.set({0,0,(float)surfc->w, (float)surfc->h});
        drawMode.set(DrawMode::ImageMode);
        // ocultes the border / secondary color
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

void DrawableObject::render__colorScheme(SDL_Renderer * renderer, float x, float y){
    size.hold(); padding.hold();
    Rect rect =
    {
        (x + padding.data.left),
        (y + padding.data.top),
        size.data.w,
        size.data.h,
    };
    size.leave(); padding.leave();
    primaryColor.hold();
    SDL_SetRenderDrawColor(renderer, primaryColor.data.r, primaryColor.data.g, primaryColor.data.b, primaryColor.data.a);
    primaryColor.leave();
    SDL_RenderFillRectF(renderer, &rect);
    _render__border(renderer, &rect);
}

void DrawableObject::render__image(SDL_Renderer * renderer, float x, float y){
    rect_buffer.hold();
    rect_buffer.data = {x + padding.get().left, y + padding.data.top, size.get().w, size.data.h};
    padding.leave();
    size.leave();
    SDL_RenderCopyF(renderer, image.get(), NULL, &rect_buffer.data);
    image.leave();
    if(withBorder)
        _render__border(renderer, (Rect*)&rect_buffer.data);
    rect_buffer.leave();
}

void DrawableObject::render__default(SDL_Renderer * renderer, float x, float y) {
    size.hold(); padding.hold();
    Rect rect =
    {
        (x + padding.data.left),
        (y + padding.data.top),
        size.data.w,
        size.data.h,
    };
    size.leave(); padding.leave();
    render__colorScheme(renderer, x, y);
    SDL_RenderDrawLineF(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
    SDL_RenderDrawLineF(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y);
}

void DrawableObject::render(SDL_Renderer * renderer, float x, float y){
    if(inRootBounds(x, y)){
        align(x, y);
        size.hold(); size.data.x = x; size.data.y = y; size.leave();
        switch (drawMode.get()) {
            case DrawMode::ImageMode:
                if(renderer == this->renderer.get()){
                    this->renderer.leave();
                    render__image(renderer, x, y);
                } else {
                    this->renderer.leave();
                    render__default(renderer, x, y);
                } break;
            case DrawMode::ColorSchemeMode:
                render__colorScheme(renderer, x, y);
                break;
            default:
                render__default(renderer, x, y);
                break;
        } drawMode.leave();
        renderEmbedded(renderer, x, y);
    }
}
