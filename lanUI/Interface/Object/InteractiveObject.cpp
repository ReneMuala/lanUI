//
//  InteractiveObject.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 06/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"

namespace InteractiveObjecsData {
Semaphore<SDL_Point> cursor;
}

using namespace InteractiveObjecsData;

InterativeObject::InterativeObject(): scrollGain({0,0}), focus_repeated(false), isActive(false),was_resized(false) {
    for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
        callbacks[i].set(false);
    }
}

void InterativeObject::_handle_events(Event & event, const float dpiK, const bool no_focus){
    if(inRootBoundsBuffer.get() && isActive.get() && !no_focus && _has_focus(dpiK)){
        inRootBoundsBuffer.leave();
        isActive.leave();
        if(callbacks[OnFocusGained].get() && !focus_repeated)
            on_focus_gained_callback();
        if(callbacks[OnClick].get()
           && event.type == SDL_MOUSEBUTTONDOWN
           && event.button.button == SDL_BUTTON_LEFT
           && event.button.clicks == 1)
            on_click_callback();
        if(callbacks[OnDoubleClick].get()
           && event.type == SDL_MOUSEBUTTONDOWN
           && event.button.button == SDL_BUTTON_LEFT
           && event.button.clicks == 2)
            on_double_click_callback();
        if(callbacks[OnSecondaryClick].get()
           && event.type == SDL_MOUSEBUTTONDOWN
           && event.button.button == SDL_BUTTON_RIGHT)
            on_secondary_click_callback();
        if(callbacks[OnMouseButtonDown].get()
           && event.type == SDL_MOUSEBUTTONDOWN)
            on_mouse_button_down_callback();
        if(callbacks[OnMouseButtonUp].get()
           && event.type == SDL_MOUSEBUTTONUP)
            on_mouse_button_up_callback();
        if(callbacks[OnKeyDown].get()
           && event.type == SDL_KEYDOWN)
            on_key_down_callback();
        if(callbacks[OnKeyUp].get()
           && event.type == SDL_KEYUP)
            on_key_up_callback();
        if(callbacks[OnScroll].get()
           && event.type == SDL_MOUSEWHEEL) {
            scrollGain.vertical = event.wheel.y;
            scrollGain.horizontal = event.wheel.x;
            on_scroll_callback();
        } if(callbacks[OnResized].get()){
            if(was_resized.get()){
                on_resized_callback();
                callbacks[OnResized].leave();
            }
            was_resized.data = false;
            was_resized.leave();
        }
        
        for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++)
        if(i != 1)
            callbacks[i].leave();
        if(nextInZ.get()){
            nextInZ.leave();
            if(drawMode.get() != CompositionMode) {
                drawMode.leave();
                _handle_others_routine(event, nextInZ.data, dpiK, false);
            } else {
                drawMode.leave();
            }
        } else
            nextInZ.leave();
        focus_repeated = true;
        no_focus_repeated = false;
    } else {
        inRootBoundsBuffer.leave();
        isActive.leave();
        
        if(callbacks[OnFocusLost].get() && !no_focus_repeated)
            on_focus_lost_callback();
        callbacks[OnFocusLost].leave();
        
        if(nextInZ.get()){
            nextInZ.leave();
            _handle_others_routine(event, nextInZ.data, dpiK, true);
        } else
            nextInZ.leave();
        _handle_others(event, dpiK, false);
        focus_repeated = false;
        no_focus_repeated = true;
    }
}

InterativeObject& InterativeObject::set_size(const float w, const float h){
    Object::set_size(w, h);
    was_resized.set(true);
    return (*this);
}

InterativeObject& InterativeObject::main_actiivity(VoidCallback callback){
    main_activity_callback = callback;
    return (*this);
}

InterativeObject& InterativeObject::on_focus_gained(VoidCallback callback){
    on_focus_gained_callback = callback;
    callbacks[OnFocusGained].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_focus_lost(VoidCallback callback){
    on_focus_lost_callback = callback;
    callbacks[OnFocusLost].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_resized(VoidCallback callback){
    on_resized_callback = callback;
    callbacks[OnResized].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_click(VoidCallback callback){
    on_click_callback = callback;
    callbacks[OnClick].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_double_click(VoidCallback callback){
    on_double_click_callback = callback;
    callbacks[OnDoubleClick].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_secondary_click(VoidCallback callback){
    on_secondary_click_callback = callback;
    callbacks[OnSecondaryClick].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_mouse_button_down(VoidCallback callback){
    on_mouse_button_down_callback = callback;
    callbacks[OnMouseButtonDown].set(true);
    isActive.set(true);
    return (*this);
}


InterativeObject& InterativeObject::on_mouse_button_up(VoidCallback callback){
    on_mouse_button_up_callback = callback;
    callbacks[OnMouseButtonUp].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_key_down(VoidCallback callback){
    on_key_down_callback = callback;
    callbacks[OnKeyDown].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_key_up(VoidCallback callback){
    on_key_up_callback = callback;
    callbacks[OnKeyUp].set(true);
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_scroll(VoidCallback callback){
    on_scroll_callback = callback;
    callbacks[OnScroll].set(true);
    isActive.set(true);
    return (*this);
}
