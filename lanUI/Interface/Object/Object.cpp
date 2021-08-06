//
//  Object.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 26/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"
#include "../../Core/Core.hpp"
#include <iostream>

void Object::__align_center(float &x, float &y){
    root.get()->size.get();
    size.hold();
    padding.hold();
    x += (root.data->size.data.w / 2 - ((size.data.w + padding.data.left + padding.data.right)/2));
    y += (root.data->size.data.h / 2 - ((size.data.h + padding.data.top + padding.data.bottom)/2));
    padding.leave();
    size.leave();
    root.data->size.leave();
    root.leave();
}

void Object::__align_left(float &x, float &y){
    static float initial_x(0);
    initial_x = x;
    __align_center(x, y);
    x = initial_x;
}

void Object::__align_right(float &x, float &y){
    static float initial_x(0);
    padding.hold();
    initial_x = x + (root.get()->size.get().w - size.get().w) - (padding.data.right + padding.data.left);
    padding.leave();
    root.data->size.leave();
    root.leave();
    size.leave();
    __align_center(x, y);
    x = initial_x;
}

void Object::__align_top(float &x, float &y){
    static float initial_y(0);
    initial_y = y;
    padding.leave();
    __align_center(x, y);
    y = initial_y;
}

void Object::__align_bottom(float &x, float &y){
    static float initial_y(0);
    padding.hold();
    initial_y = y + (root.get()->size.get().h - size.get().h) - (padding.data.bottom + padding.data.top);
    padding.leave();
    root.data->size.leave();
    root.leave();
    size.leave();
    __align_center(x, y);
    y = initial_y;
}

Object& Object::operator=(Object & other){
    for(int i = 0 ; i < Properties::totalProperties ; i++)
        properties[i] = other.properties[i];
    size = other.size.get(); other.size.leave();
    padding = other.padding.get(); other.padding.leave();
    return (*this);
}

void Object::_sync_root_size(const float &wDif, const float & hDif){
    // update the root size to avoid errors with _inRootBounds(...)
    switch (rootType.get()){
        case VStackRoot:
            if(root.get() && hDif) {
                root.data->_fix_size(0, hDif);
            } root.leave();
            break;
        case HStackRoot:
            if(root.get()) {
                root.data->_fix_size(wDif, 0);
            } root.leave();
            break;
        case ZStackRoot:
            if(root.get()) {
                if(size.get().w >= root.data->size.get().w || size.data.h >= root.data->size.data.w) {
                    root.data->size.leave();
                    root.data->_fix_size(wDif, hDif);
                } root.data->size.leave();
                size.leave();
            } root.leave();
            break;
        default:break;
    }
}

Object& Object::set_size(const float size_w, const float size_h){
    static float wDif(0);
    static float hDif(0);
    
    wDif = size.get().w;
    hDif = size.data.h;

    size.data = {0,0, size_w, size_h};
    
    /// get the difference bettwen the actual with, and the last one
    wDif = size.data.w - wDif;
    hDif = size.data.h - hDif;
    
    size.leave();
    
    if(rootType.get()){
        rootType.leave();
        _sync_root_size(wDif, hDif);
    } rootType.leave();
    
    return (*this);
}

void Object::_fix_size(const float w, const float h){
    size.get();
    size.data.w += w;
    size.data.h += h;
    size.leave();
}

Object& Object::set_relative_size(const float size_w, const float size_h, const float w_correction, const float h_correction){
    if(!root.get()) Core::log(Core::Error, "set_relative_size(...) must to be used in embedded objects.");
    root.data->size.hold();
    static float w(0), h(0);
    w = root.data->size.data.w * size_w;
    h = root.data->size.data.h * size_h;
    root.data->size.leave();
    root.leave();
    set_size((w) + w_correction,
             (h) + h_correction);
    return (*this);
}

Object& Object::set_padding(Padding padding){
    this->padding.set(padding);
    return (*this);
}

Object& Object::set_alignment(Alignment alignment){
    this->aligment.set(alignment);
    return (*this);
}

Object& Object::updateRoot(Object * newRoot){
    root.set(newRoot);
    if(nextInX.get())
        nextInX.data->updateRoot(newRoot);
    nextInX.leave();
    if(nextInY.get())
        nextInY.data->updateRoot(newRoot);
    nextInY.leave();
    return (*this);
}

Object& Object::embedInX(Object & other){
    nextInX.hold();
    nextInX.data = &other;
    nextInX.data->updateRoot(root.get());
    root.leave();
    nextInX.leave();
    return (*this);
}

Object& Object::embedInY(Object & other){
    nextInY.hold();
    nextInY.data = &other;
    nextInY.data->updateRoot(root.get());
    root.leave();
    nextInY.leave();
    return (*this);
}

Object& Object::embedInZ(Object & other){
    nextInZ.hold();
    nextInZ.data = &other;
    // nextInZ.data->root = this;
    nextInZ.data->updateRoot(this);
    nextInZ.leave();
    return (*this);
}

Object& Object::switchE(Object & other){
    Object buffer;
    buffer = other;
    other = *this;
    *this = buffer;
    return (*this);
}

Object& Object::replaceE(Object & other){
    *this = other;
    return (*this);
}

bool Object::_inRootBounds(float x, float y){
    // ignore root bounds
    if(!usingRootBounds.get()) {
        usingRootBounds.leave();
        return true;
    } usingRootBounds.leave();
    
    bool inBounds = true;
    if(root.get()){
        root.data->size.get();
        size.get(); padding.get();
        inBounds = ((x + size.data.w + padding.data.left) <= (root.data->size.data.x + root.data->size.data.w) &&
                    (y + size.data.h + padding.data.top) <= (root.data->size.data.y + root.data->size.data.h)) ? true : false;
        size.leave(); padding.leave();
        root.data->size.leave();
    } root.leave();
    return inBounds;
}

void Object::_useRootBounds(){
    usingRootBounds.set(true);
}

void Object::_render(SDL_Renderer * renderer, float x, float y, const float dpiK) {
    if(_inRootBounds(x, y)){
        _align(x, y);
        size.hold(); size.data.x = x; size.data.y = y; size.leave();
        _renderEmbedded(renderer, x, y, dpiK);
    }
}

void Object::_align(float &x, float &y){
    if(root.get()){
        root.leave();
        switch (aligment.get()){
            case Alignment::Center: __align_center(x, y); break;
            case Alignment::Top: __align_top(x, y); break;
            case Alignment::Bottom: __align_bottom(x, y); break;
            case Alignment::Left: __align_left(x, y); break;
            case Alignment::Right: __align_right(x, y); break;
            default: break;
        }
        aligment.leave();
    } root.leave();
}

void Object::__renderEmbedded_routine(SDL_Renderer * renderer, Object * embedded, const float x, const float y, float dpiK){
    if(embedded->properties[Properties::isDrawable].get())
        ((DrawableObject*)embedded)->_render(renderer, x, y, dpiK);
    else
        embedded->_renderEmbedded(renderer, x, y, dpiK);
    embedded->properties[Properties::isDrawable].leave();
}

void Object::_renderEmbedded(SDL_Renderer * renderer, const float x, const float y, float dpiK){
    padding.hold();
    if(nextInZ.get())
        __renderEmbedded_routine(renderer, nextInZ.data,
                                 x + padding.data.left,
                                 y + padding.data.top,
                                 dpiK);
    nextInZ.leave();
    size.hold();
    if(nextInX.get())
        __renderEmbedded_routine(renderer, nextInX.data,
                                 x + size.data.w + padding.data.left + padding.data.right,
                                 y,
                                 dpiK);
    nextInX.leave();
    if(nextInY.get())
        __renderEmbedded_routine(renderer, nextInY.data, x,
                                 y + size.data.h + padding.data.top + padding.data.bottom,
                                 dpiK);
    nextInY.leave();
    size.leave();
    padding.leave();
}

Object::Object(): size({0,0,50,50}), padding({5.0,5.0,5.0,5.0}), root(nullptr), nextInX(nullptr), nextInY(nullptr),nextInZ(nullptr), usingRootBounds(false), rootType(OtherRoot) {
    aligment.set(Alignment::None);
    for(int i = 0 ; i < Requests::totalRequests ; i++){
        requests[i].leave();
        requests[i].set(false);
    }
}
