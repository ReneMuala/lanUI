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

InterativeObject::InterativeObject(): scrollGain({0,0}){
    Object();
    _clear_properties();
    properties[Properties::isInteractive].set(true);
    
    for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
        callbacks[i].leave();
        callbacks[i].set(false);
    }
}

void InterativeObject::_handle(Event & event, const float dpiK){
    static bool focus_repeated(false);
    static bool no_focus_repeated(false);
    if(inRootBounds_buffer.get()){
        inRootBounds_buffer.leave();
        if(this->_has_focus(dpiK)){
            if(callbacks[OnFocusGained].get() && !focus_repeated)
                on_focus_gained_callback();
            if(callbacks[OnClick].get()
               && event.type == SDL_MOUSEBUTTONDOWN
               && event.button.button == SDL_BUTTON_LEFT
               && event.button.clicks == 1)
                on_click_callback();
            else if(callbacks[OnDoubleClick].get()
                    && event.type == SDL_MOUSEBUTTONDOWN
                    && event.button.button == SDL_BUTTON_LEFT
                    && event.button.clicks == 2)
                on_double_click_callback();
            else if(callbacks[OnSecondaryClick].get()
                    && event.type == SDL_MOUSEBUTTONDOWN
                    && event.button.button == SDL_BUTTON_RIGHT)
                on_secondary_click_callback();
            else if(callbacks[OnMouseButtonDown].get()
                    && event.type == SDL_MOUSEBUTTONDOWN)
                on_mouse_button_down_callback();
            else if(callbacks[OnMouseButtonUp].get()
                    && event.type == SDL_MOUSEBUTTONUP)
                on_mouse_button_up_callback();
            else if(callbacks[OnKeyDown].get()
                    && event.type == SDL_KEYDOWN)
                on_key_down_callback();
            else if(callbacks[OnKeyUp].get()
                    && event.type == SDL_KEYUP)
                on_key_up_callback();
            else if(callbacks[OnScroll].get()
                    && event.type == SDL_MOUSEWHEEL) {
                scrollGain.vertical = event.wheel.y;
                scrollGain.horizontal = event.wheel.x;
                on_scroll_callback();
            }
            for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++)
                callbacks[i].leave();
            if(nextInZ.get() && nextInZ.data->_has_focus(dpiK)){
                _handle_others_routine(event, nextInZ.data, dpiK);
            } nextInZ.leave();
            focus_repeated = true;
            no_focus_repeated = true;
        } else {
            if(callbacks[OnFocusLost].get() && !no_focus_repeated)
                on_focus_lost_callback();
            callbacks[OnFocusLost].leave();
            
            if(nextInZ.get()){
                nextInZ.leave();
                _handle_others_routine(event, nextInZ.data, dpiK);
            } nextInZ.leave();
            _handle_others(event, dpiK);
            no_focus_repeated = true;
            focus_repeated = false;
        }
    } inRootBounds_buffer.leave();
}

InterativeObject& InterativeObject::set_size(const float w, const float h){
    Object::set_size(w, h);
    if(callbacks[OnResized].get())
        on_resized_callback();
    callbacks[OnResized].leave();
    return (*this);
}

InterativeObject& InterativeObject::main_actiivity(VoidCallback callback){
    main_activity_callback = callback;
    return (*this);
}

InterativeObject& InterativeObject::on_focus_gained(VoidCallback callback){
    on_focus_gained_callback = callback;
    callbacks[OnFocusGained].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_focus_lost(VoidCallback callback){
    on_focus_lost_callback = callback;
    callbacks[OnFocusLost].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_resized(VoidCallback callback){
    on_resized_callback = callback;
    callbacks[OnResized].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_click(VoidCallback callback){
    on_click_callback = callback;
    callbacks[OnClick].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_double_click(VoidCallback callback){
    on_double_click_callback = callback;
    callbacks[OnDoubleClick].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_secondary_click(VoidCallback callback){
    on_secondary_click_callback = callback;
    callbacks[OnSecondaryClick].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_mouse_button_down(VoidCallback callback){
    on_mouse_button_down_callback = callback;
    callbacks[OnMouseButtonDown].set(true);
    return (*this);
}


InterativeObject& InterativeObject::on_mouse_button_up(VoidCallback callback){
    on_mouse_button_up_callback = callback;
    callbacks[OnMouseButtonUp].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_key_down(VoidCallback callback){
    on_key_down_callback = callback;
    callbacks[OnKeyDown].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_key_up(VoidCallback callback){
    on_key_up_callback = callback;
    callbacks[OnKeyUp].set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_scroll(VoidCallback callback){
    on_scroll_callback = callback;
    callbacks[OnScroll].set(true);
    return (*this);
}
