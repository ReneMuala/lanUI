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
#include <string>

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

//void Object::_sync_root_size_for_padding(const float &wDif, const float &hDif){
//    // update the root size to avoid errors with _inRootBounds(...)
//    switch (rootType.get()){
//        case VStackRoot:
//            if(root.get() && hDif) {
//                root.data->_fix_size(0, hDif);
//            } root.leave();
//            break;
//        case HStackRoot:
//            if(root.get()) {
//                root.data->_fix_size(wDif, 0);
//            } root.leave();
//            break;
//        case ZStackRoot:
//            if(root.get()) {
//                if((size.get().w+padding.get().left+padding.data.right) >= root.data->size.get().w
//                   || (size.data.h+padding.data.top+padding.data.bottom) >= root.data->size.data.w) {
//                    root.data->size.leave();
//                    root.data->_fix_size(wDif, hDif);
//                } root.data->size.leave();
//                padding.leave();
//                size.leave();
//            } root.leave();
//            break;
//        default:break;
//    }
//}
//
//void Object::_sync_root_size(const float &wDif, const float & hDif){
//    // update the root size to avoid errors with _inRootBounds(...)
//    switch (rootType.get()){
//        case VStackRoot:
//            if(root.get() && hDif) {
//                root.data->_fix_size(0, hDif);
//            } root.leave();
//            break;
//        case HStackRoot:
//            if(root.get()) {
//                root.data->_fix_size(wDif, 0);
//            } root.leave();
//            break;
//        case ZStackRoot:
//            if(root.get()) {
//                if((size.get().w+padding.get().left+padding.data.right) >= root.data->size.get().w
//                   || (size.data.h+padding.data.top+padding.data.bottom) >= root.data->size.data.w) {
//                    root.data->size.leave();
//                    root.data->_fix_size(wDif, hDif);
//                } root.data->size.leave();
//                padding.leave();
//                size.leave();
//            } root.leave();
//            break;
//        default:break;
//    }
//}


#include "../Container/Container.hpp"
#include "../Stack/Stack.hpp"
#include "../List/List.hpp"

Object& Object::set_size(const float size_w, const float size_h){
    size.set({0,0, size_w, size_h});
    if(root.get()) {
        root.leave();
        root.data->reload();
    } root.leave();
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
    if(root.get()) {
        root.leave();
        root.data->reload();
    } root.leave();
    return (*this);
}

Object& Object::set_alignment(Alignment alignment){
    this->aligment.set(alignment);
    return (*this);
}

Object& Object::updateRoot(Object * newRoot){
    if(root.get()) Core::log(Core::Error, ("Object ["+ std::to_string((long)this)+"] already has a root, this operation is unsafe. (Cast to continue).").c_str());
    root.leave();
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
        inRootBounds_buffer.set(true);
        return true;
    } usingRootBounds.leave();
    
    bool inBounds = true;
    
    if(root.get()){
        root.data->size.get();
        size.get(); padding.get();
        inBounds = ((x + padding.data.left >= root.data->size.data.x)
                    && (y + padding.data.top >= root.data->size.data.y)
                    && (x+(padding.data.left + size.data.w + padding.data.right)) <= (root.data->size.data.x + root.data->size.data.w)
                    && (y+(padding.data.top + size.data.h + padding.data.bottom)) <= (root.data->size.data.y + root.data->size.data.h));
        size.leave(); padding.leave();
        root.data->size.leave();
    } root.leave();
    inRootBounds_buffer.set(inBounds);
    return inBounds;
}

void Object::_useRootBounds(){
    usingRootBounds.set(true);
}

void Object::__renderEmbedded_routine(SDL_Renderer * renderer, Object * embedded, const float x, const float y, float dpiK){
    if(embedded->properties[Properties::isDrawable].get())
        ((DrawableObject*)embedded)->_render(renderer, x, y, dpiK);
    else
        embedded->_render(renderer, x, y, dpiK);
    embedded->properties[Properties::isDrawable].leave();
}

void Object::_renderEmbedded(SDL_Renderer * renderer, const float x, const float y, float dpiK, _RenderEmbeddedMode _renderEmbeddedMode){
    
    static Padding padding_buffer;
    static Rect size_buffer;
    
    padding_buffer = padding.get();
    size_buffer = size.get();
    
    padding.leave();
    size.leave();
    
    if(!(_renderEmbeddedMode & _renderOnlyNextInX_Y))
    {
        if(nextInZ.get())
            __renderEmbedded_routine(renderer, nextInZ.data,
                                     x + padding_buffer.left,
                                     y + padding_buffer.top,
                                     dpiK);
        
        
        nextInZ.leave();
    }
    
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

void Object::_render(SDL_Renderer * renderer, float x, float y, const float dpiK) {
    if(_inRootBounds(x, y)){
        _align(x, y);
        size.hold(); padding.hold(); size.data.x = x + padding.data.left; size.data.y = y + padding.data.top; size.leave(); padding.leave();
        _renderEmbedded(renderer, x, y, dpiK, _renderOnlyNextInZ);
    } _renderEmbedded(renderer, x, y, dpiK, _renderOnlyNextInX_Y);
}

void Object::_handle_others_routine(Event& event, Object * next, const float dpiK){
    if(next->properties[Properties::isInteractive].get()){
        next->properties[Properties::isInteractive].leave();
        ((InterativeObject*)next)->_handle(event, dpiK);
    } else {
        next->properties[Properties::isInteractive].leave();
        (next)->_handle(event, dpiK);
    }
}

void Object::_handle_others(Event& event, const float dpiK){
    if(nextInX.get()){
        nextInX.leave();
        _handle_others_routine(event, nextInX.data, dpiK);
    } nextInX.leave();
    if(nextInY.get()) {
        nextInY.leave();
        _handle_others_routine(event, nextInY.data, dpiK);
    } nextInY.leave();
}

using namespace InteractiveObjecsData;

bool Object::_has_focus(const float dpiK){
    static bool focus(false);
    (focus =
     (cursor.get().x >= ((size.get().x)*dpiK))
     && (cursor.data.y >= ((size.data.y)*dpiK))
     && (cursor.data.x <= (size.data.x + size.data.w)*dpiK)
     && (cursor.data.y <= (size.data.y + size.data.h)*dpiK)
     );
    padding.leave();
    size.leave();
    cursor.leave();
    return focus;
}

void Object::_handle(Event & event, const float dpiK){
    if(inRootBounds_buffer.get()){
        inRootBounds_buffer.leave();
        size.leave();
        if(this->_has_focus(dpiK)){
            // ignore this object if the nextInZ has focus
            if(nextInZ.get()){
                nextInZ.leave();
                _handle_others_routine(event, nextInZ.data, dpiK);
            } nextInZ.leave();
        } else {
            if(nextInZ.get()){
                nextInZ.leave();
                _handle_others_routine(event, nextInZ.data, dpiK);
            } nextInZ.leave();
            _handle_others(event, dpiK);
        }
    } inRootBounds_buffer.leave();
}

void Object::_run_others_default_animation(){
    if(nextInZ.data)
        nextInZ.data->_run_default_animation();
    //nextInZ.leave();
    
    if(nextInX.data)
        nextInX.data->_run_default_animation();
    //nextInX.leave();
    
    if(nextInY.data)
        nextInY.data->_run_default_animation();
    //nextInY.leave();
}

void Object::_run_default_animation(){
    if(properties[Properties::isDrawable].get()){
        properties[Properties::isDrawable].leave();
        ((DrawableObject*)this)->_run_default_animation();
    }  properties[Properties::isDrawable].leave();
    _run_others_default_animation();
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

void Object::_clear_properties(){
    for(int i = 0 ; i < Properties::totalProperties ; i++){
        properties[i].leave();
        properties[i].set(false);
    }
}

Object::Object(): size({0,0,50,50}), padding({5.0,5.0,5.0,5.0}), root(nullptr), nextInX(nullptr), nextInY(nullptr),nextInZ(nullptr), usingRootBounds(false), inRootBounds_buffer(true) /*rootType(OtherRoot)*/ {
    aligment.set(Alignment::None);
    for(int i = 0 ; i < Requests::totalRequests ; i++){
        requests[i].leave();
        requests[i].set(false);
    }
}
