//
//  InteractiveObject.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 06/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"

namespace InteractiveObjecsData {
    Semaphore<SDL_FPoint> cursor;
}

using namespace InteractiveObjecsData;

InterativeObject::InterativeObject(VoidCallback mainActivity) {
    main_activity = mainActivity;
    properties[Properties::isInteractive].set(true);
    for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
        callbacks[i].leave();
        callbacks[i].set(false);
    }
}

bool InterativeObject::_has_focus(){
    static bool focus(false);
    (focus =
            (cursor.get().x >= size.get().x) && (cursor.data.x <= size.data.x + size.data.w) &&
            (cursor.data.y >= size.data.y) && (cursor.data.y <= size.data.y + size.data.h));
    size.leave();
    cursor.leave();
    return focus;
}

bool InterativeObject::_has_focus(Object * next){
    if(next->properties[Properties::isInteractive].get()){
        return ((InterativeObject*)next)->_has_focus();
    } return false;
}

void InterativeObject::_handle_others_routine(Event& event, Object * next){
    if(next->properties[Properties::isInteractive].get()){
        ((InterativeObject*)next)->_handle(event);
    }
}

void InterativeObject::_handle_others(Event& event){
    if(nextInX.get()) _handle_others_routine(event, nextInX.data);
    nextInX.leave();
    if(nextInY.get()) _handle_others_routine(event, nextInY.data);
    nextInY.leave();
}

void InterativeObject::_handle(Event & event){
    if(this->_has_focus()){
        // ignore this object if the nextInZ has focus
        if(_has_focus(nextInZ.get())){
            ((InterativeObject*)nextInZ.data)->_handle(event);
        } else {
            _handle_others(event);
        } nextInZ.leave();
    }
}
