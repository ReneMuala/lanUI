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
#include <stack>
#include <map>

std::stack<SDL_Rect> rendererClips;

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
    const float initial_x = x;
    __align_center(x, y);
    x = initial_x;
}

void Object::__align_right(float &x, float &y){
    padding.hold();
    const float initial_x = x + (root.get()->size.get().w - size.get().w) - (padding.data.right + padding.data.left);
    padding.leave();
    root.data->size.leave();
    root.leave();
    size.leave();
    __align_center(x, y);
    x = initial_x;
}

void Object::__align_top(float &x, float &y){
    const float initial_y = y;
    __align_center(x, y);
    y = initial_y;
}

void Object::__align_bottom(float &x, float &y){
    padding.hold();
    size.hold();
    root.hold();
    root.data->size.get();
    const float initial_y = y + (root.data->size.data.h - size.data.h) - (padding.data.bottom + padding.data.top);
    padding.leave();
    root.data->size.leave();
    root.leave();
    size.leave();
    __align_center(x, y);
    y = initial_y;
}

Object& Object::operator=(Object & other){
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

Object& Object::enable_reloading(){
    reloadingDisabled.set(false);
    return (*this);
}

Object& Object::disable_reloading(){
    reloadingDisabled.set(true);
    return (*this);
}

Object& Object::set_size(const float size_w, const float size_h){
    size.hold();
    size.data.w = (size_w >= 0) ? size_w : size.data.w;
    size.data.h = (size_h >= 0) ? size_h : size.data.h;
    size.leave();
    if(root.get()) {
        root.leave();
        root.data->reload();
    } else
        root.leave();
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
    const float w = root.data->size.data.w * size_w;
    const float h = root.data->size.data.h * size_h;
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
    } else
        root.leave();
    return (*this);
}

Object& Object::set_scrollingFactor(ScrollingFactor scrollingFactor){
    this->scrollingFactor.set(scrollingFactor);
    
    if(nextInX.get()) {
        nextInX.leave();
        nextInX.data->set_scrollingFactor(scrollingFactor);
    } else
        nextInX.leave();
    
    if(nextInY.get()) {
        nextInY.leave();
        nextInY.data->set_scrollingFactor(scrollingFactor);
    } else
        nextInY.leave();
    
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
    other.set_alignment(None);
    nextInX.hold();
    nextInX.data = &other;
    nextInX.data->updateRoot(root.get());
    root.leave();
    nextInX.leave();
    return (*this);
}

Object& Object::embedInY(Object & other){
    other.set_alignment(None);
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

//Object& Object::switchE(Object & other){
//    Object buffer;
//    buffer = other;
//    other = *this;
//    *this = buffer;
//    return (*this);
//}
//
//Object& Object::replaceE(Object & other){
//    *this = other;
//    return (*this);
//}

bool Object::_inRootBounds(float x, float y){
    // ignore root bounds
    if(!usingRootBounds.get()) {
        usingRootBounds.leave();
        inRootBoundsBuffer.set(true);
        return true;
    } usingRootBounds.leave();
    
    bool inBounds = true;
    
    root.hold();
    const SDL_Rect rootRect =
    {
        (int)(root.data->size.data.x),
        (int)(root.data->size.data.y),
        (int)(root.data->size.data.w),
        (int)(root.data->size.data.h),
    };
    root.leave();
    
    size.hold();
    padding.hold();
    scrollingFactor.hold();
    const SDL_Rect thisRect =
    {
        (int)(x+scrollingFactor.data.horizontal),
        (int)(y+scrollingFactor.data.vertical),
        (int)(padding.data.left+size.data.w+padding.data.right),
        (int)(padding.data.top+size.data.h+padding.data.bottom),
    };
    size.leave();
    padding.leave();
    scrollingFactor.leave();
    
    if((inBounds = SDL_HasIntersection(&thisRect, &rootRect))){
        if(thisRect.y >= rootRect.y)
            isVericallyAfterRootBeginning.set(true);
        else
            isVericallyAfterRootBeginning.set(false);
        
        if(thisRect.y + thisRect.h <= rootRect.y+rootRect.h)
            isVericallyBeforeRootEnding.set(true);
        else
            isVericallyBeforeRootEnding.set(false);
    } inRootBoundsBuffer.set(inBounds);
    return inBounds;
}

void Object::_useRootBounds(){
    usingRootBounds.set(true);
}

void Object::__renderEmbedded_routine(SDL_Renderer * renderer, Object * embedded, const float x, const float y, float dpiK){
    embedded->_render(renderer, x, y, dpiK);
}

void Object::_renderEmbedded(SDL_Renderer * renderer, const float x, const float y, float dpiK, _RenderEmbeddedMode _renderEmbeddedMode){
    
    const Padding padding_buffer = padding.get();
    const Rect size_buffer = size.get();
    const ScrollingFactor scrollingFactor_buffer = scrollingFactor.get();
    
    padding.leave();
    size.leave();
    scrollingFactor.leave();
    
    if(!(_renderEmbeddedMode & _renderOnlyNextInX_Y))
    {
        _lock_renderer_in_bounds(renderer, dpiK);
        if(nextInZ.get())
            __renderEmbedded_routine(renderer, nextInZ.data,
                                     x + padding_buffer.left + scrollingFactor_buffer.horizontal,
                                     y + padding_buffer.top + scrollingFactor_buffer.vertical,
                                     dpiK);
        
        nextInZ.leave();
        _unlock_renderer_from_bounds(renderer);
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

void Object::_lock_renderer_in_bounds(SDL_Renderer * renderer, float dpiK){
    SDL_Rect inter;
    sizeBuffer.hold();
    const SDL_Rect bounds =
    {
        (int)(sizeBuffer.data.x),
        (int)(sizeBuffer.data.y),
        (int)(sizeBuffer.data.w),
        (int)(sizeBuffer.data.h),
    };
    sizeBuffer.leave();

    if(rendererClips.empty()){
        SDL_RenderSetClipRect(renderer, &bounds);
        rendererClips.push(bounds);
#ifdef LANUI_DEBUG_PRINT_RENDERER_LOCK_AND_UNLOCK_EVENTS
        printf("(1) renderer lock: [%d %d %d %d]\n", rendererClips.top().x, rendererClips.top().y, rendererClips.top().w, rendererClips.top().h);
#endif

        
    } else {
        SDL_IntersectRect(&bounds,&rendererClips.top(), &inter);
        if(SDL_RectEmpty(&inter)){
            SDL_RenderSetClipRect(renderer, &rendererClips.top());
            rendererClips.push(bounds);
#ifdef LANUI_DEBUG_PRINT_RENDERER_LOCK_AND_UNLOCK_EVENTS
            printf("(2) renderer lock: [%d %d %d %d]\n", rendererClips.top().x, rendererClips.top().y, rendererClips.top().w, rendererClips.top().h);
#endif
        } else {
            SDL_RenderSetClipRect(renderer, &inter);
            rendererClips.push(inter);
#ifdef LANUI_DEBUG_PRINT_RENDERER_LOCK_AND_UNLOCK_EVENTS
            printf("(3) renderer lock: [%d %d %d %d]\n", rendererClips.top().x, rendererClips.top().y, rendererClips.top().w, rendererClips.top().h);
#endif
        }
    }
    
#ifdef LANUI_DEBUG_MODE
    SDL_SetRenderDrawColor(renderer, 255, 20, 20, 20);
    SDL_RenderFillRect(renderer, &rendererClips.top());
    SDL_SetRenderDrawColor(renderer, 20, 255, 200, 200);
    SDL_RenderDrawRect(renderer, &rendererClips.top());
#endif
}

void Object::_unlock_renderer_from_bounds(SDL_Renderer * renderer){
    rendererClips.pop();
    if(!rendererClips.empty()){
#ifdef LANUI_DEBUG_PRINT_RENDERER_LOCK_AND_UNLOCK_EVENTS
        printf("renderer unlock: [%d %d %d %d]\n", rendererClips.top().x, rendererClips.top().y, rendererClips.top().w, rendererClips.top().h);
#endif
        SDL_RenderSetClipRect(renderer, &rendererClips.top());
    } else {
#ifdef LANUI_DEBUG_PRINT_RENDERER_LOCK_AND_UNLOCK_EVENTS
        printf("renderer unlock: [full screen]\n");
#endif
        SDL_RenderSetClipRect(renderer, nullptr);
    }
}

void Object::_render_routine(float dpiK){
    sizeBuffer.hold();
    sizeBuffer.data = {
        ((size.get().x)*dpiK),
        ((size.data.y)*dpiK),
        (size.data.w)*dpiK,
        (size.data.h)*dpiK
    };
    scrollingFactor.leave();
    size.leave();
    sizeBuffer.leave();
}

void Object::_handle_others_routine(Event& event, Object * next, const float dpiK, const bool no_focus){
    (next)->_handle_events(event, dpiK, no_focus);
}

void Object::_handle_others(Event& event, const float dpiK, const bool no_focus){
    if(nextInX.get()){
        nextInX.leave();
        _handle_others_routine(event, nextInX.data, dpiK, no_focus);
    } else
        nextInX.leave();
    if(nextInY.get()) {
        nextInY.leave();
        _handle_others_routine(event, nextInY.data, dpiK, no_focus);
    } else
        nextInY.leave();
}

using namespace InteractiveObjecsData;

bool Object::_has_focus(const float dpiK){
    bool focus(false);
    (focus =
     // scrollingFactor doesn't need to be calculated because it is already added with size.
     (cursor.get().x >= (size.get().x*dpiK))
     && (cursor.data.y >= (size.data.y*dpiK))
     && (cursor.data.x <= (size.data.x + size.data.w)*dpiK)
     && (cursor.data.y <= (size.data.y + size.data.h)*dpiK)
     );
    hasFocusBuffer.set(focus);
    scrollingFactor.leave();
    size.leave();
    cursor.leave();
    return focus;
}

void Object::_handle_events(Event & event, const float dpiK, const bool no_focus){
    if(inRootBoundsBuffer.get()&& !no_focus){
        inRootBoundsBuffer.leave();
            if(_has_focus(dpiK) && nextInZ.get()){
                nextInZ.leave();
                if(renderMode.get() != CompositionMode) {
                    renderMode.leave();
                    _handle_others_routine(event, nextInZ.data, dpiK, false);
                } else {
                    renderMode.leave();
                }
            } else
                nextInZ.leave();
            no_focus_repeated = false;
    } else if (!no_focus_repeated)  {
        if(nextInZ.get()){
            nextInZ.leave();
            _handle_others_routine(event, nextInZ.data, dpiK, true);
        } else
            nextInZ.leave();
        no_focus_repeated = true;
    } inRootBoundsBuffer.leave();
    _handle_others(event, dpiK, no_focus);
}

void Object::_run_others_default_animation(){
    if(nextInZ.get()){
        nextInZ.leave();
        nextInZ.data->param_dpiK = param_dpiK;
        nextInZ.data->_run_default_animation();
    } else
        nextInZ.leave();
    
    if(nextInX.get()){
        nextInX.leave();
        nextInX.data->param_dpiK = param_dpiK;
        nextInX.data->_run_default_animation();
    } else
        nextInX.leave();
    
    if(nextInY.get()){
        nextInY.leave();
        nextInY.data->param_dpiK = param_dpiK;
        nextInY.data->_run_default_animation();
    } else
     nextInY.leave();
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
    } else
        root.leave();
}

void Object::_set_position(const float x, const float y){
    size.hold(); padding.hold(); scrollingFactor.hold(); size.data.x = x + padding.data.left + scrollingFactor.data.horizontal; size.data.y = y + padding.data.top + scrollingFactor.data.vertical; size.leave(); padding.leave(); scrollingFactor.leave();
}

Object::Object(): size({0,0,50,50}), padding({0,0,0,0}), scrollingFactor({0,0}), root(nullptr), nextInX(nullptr), nextInY(nullptr), nextInZ(nullptr), usingRootBounds(false), inRootBoundsBuffer(false), reloadingDisabled(false) /*rootType(OtherRoot)*/, canvas(nullptr), compositionCanvas(nullptr), withBackground(false), withBorder(false), foregroundColor(Colors::Transparent), backgroundColor(Colors::Transparent), borderColor(Colors::Transparent), angle(0), renderMode(RenderMode::DefaultMode), delay(0), wasCompiled(false), index(0), no_focus_repeated(false) {
    aligment.set(Alignment::None);
    for(int i = 0 ; i < Requests::totalRequests ; i++){
        requests[i].set(false);
    }
    default_animation.get()._using = false;
    default_animation.data.delay = 0;
    default_animation.leave();
}

void Object::_delete_tree(){
    if(nextInX.get()){
        nextInX.leave();
        nextInX.data->_delete_custom_data();
        nextInX.data->_delete_tree();
        nextInX.set(nullptr);
    } else
        nextInX.leave();
    
    if(nextInY.get()){
        nextInY.leave();
        nextInY.data->_delete_custom_data();
        nextInY.data->_delete_tree();
        nextInY.set(nullptr);
    } else
        nextInY.leave();
    
    if(nextInZ.get()){
        nextInZ.leave();
        nextInZ.data->_delete_custom_data();
        nextInZ.data->_delete_tree();
        nextInZ.set(nullptr);
    } else
        nextInZ.leave();

    delete this;
}

Object::~Object(){
//    _deleteOthers();
    _free_canvas();
    _free_canvas(compositionCanvas);
}
