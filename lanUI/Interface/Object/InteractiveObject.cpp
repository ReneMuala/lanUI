//
//  InteractiveObject.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 06/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"
#include "../../Core/Core.hpp"

namespace InteractiveObjecsData {
    Semaphore<SDL_Point> cursor;
    Semaphore<Object*> selectedObject;
}

using namespace InteractiveObjecsData;

InterativeObject::InterativeObject(): scrollGain({0,0}), focus_repeated(false), isActive(false),was_resized(false),this_event(nullptr),is_selected(false) {
    callbacks.hold();
    for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
        callbacks.data.all[i] = false;
    }
    callbacks.leave();
}

void InterativeObject::_handle_events(Event & event, const float dpiK, const bool no_focus){
    if(inRootBoundsBuffer.get() && isActive.get() && !no_focus){
        inRootBoundsBuffer.leave();
        isActive.leave();
        this_event = &event;
        if(_has_focus(dpiK)){
            callbacks.hold();
            if(!focus_repeated && callbacks.data.all[OnFocusGained])
                on_focus_gained_callback();
            if(!is_selected && callbacks.data.all[OnSelected] && Core::get_selected_object() == this){
                on_selected_callback();
                is_selected = true;
            }
            
            if(event.type == SDL_MOUSEBUTTONDOWN){
                Core::set_selected_object(this);
                if(callbacks.data.all[OnClick]
                   && event.button.button == SDL_BUTTON_LEFT
                   && event.button.clicks == 1)
                    on_click_callback();
            } else if(event.type == SDL_MOUSEBUTTONDOWN
               && callbacks.data.all[OnDoubleClick]
               && event.button.button == SDL_BUTTON_LEFT
               && event.button.clicks == 2)
                on_double_click_callback();
            else if(event.type == SDL_MOUSEBUTTONDOWN
               && callbacks.data.all[OnSecondaryClick]
               && event.button.button == SDL_BUTTON_RIGHT)
                on_secondary_click_callback();
            else if(event.type == SDL_MOUSEBUTTONDOWN
               && callbacks.data.all[OnMouseButtonDown])
                on_mouse_button_down_callback();
            else if(event.type == SDL_MOUSEBUTTONUP
               && callbacks.data.all[OnMouseButtonUp])
                on_mouse_button_up_callback();
            else if(event.type == SDL_KEYDOWN
               && callbacks.data.all[OnKeyDown])
                on_key_down_callback();
            else if(event.type == SDL_KEYUP
               && callbacks.data.all[OnKeyUp])
                on_key_up_callback();
            else if(event.type == SDL_MOUSEWHEEL
               && callbacks.data.all[OnScroll]) {
                scrollGain.vertical = event.wheel.y;
                scrollGain.horizontal = event.wheel.x;
                on_scroll_callback();
            } else if(event.type == SDL_DROPBEGIN
                 && callbacks.data.all[OnDropBegin]){
                on_drop_begin_callback();
            } else if(event.type == SDL_DROPCOMPLETE
                 && callbacks.data.all[OnDropEnd]){
                on_drop_end_callback();
            } else if(event.type == SDL_DROPFILE
                 && callbacks.data.all[OnDropFile]){
                on_drop_file_callback();
            } else if(event.type == SDL_DROPTEXT
                 && callbacks.data.all[OnDropText]){
                on_drop_text_callback();
            }
            
            if(event.key.keysym.sym == SDLK_AUDIOPLAY && callbacks.data.all[OnAudioPlay]){
                on_audio_play_callback();
            } else if(event.key.keysym.sym == SDLK_AUDIOSTOP && callbacks.data.all[OnAudioStop]){
                on_audio_stop_callback();
            } else if(event.key.keysym.sym == SDLK_AUDIONEXT && callbacks.data.all[OnAudioNext]){
                on_audio_next_callback();
            } else if(event.key.keysym.sym == SDLK_AUDIOPREV && callbacks.data.all[OnAudioPrev]){
                on_audio_prev_callback();
            } else if(event.key.keysym.sym == SDLK_AUDIOMUTE && callbacks.data.all[OnAudioMute]){
                on_audio_mute_callback();
            } else if(event.key.keysym.sym == SDLK_AUDIOREWIND && callbacks.data.all[OnAudioRewind]){
                on_audio_rewind_callback();
            } else if(event.key.keysym.sym == SDLK_AUDIOFASTFORWARD && callbacks.data.all[OnAudioFastForward]){
                on_audio_rewind_callback();
            } else if(event.key.keysym.sym == SDLK_F1 && callbacks.data.all[OnF1]){
                on_f1_callback();
            } else if(event.key.keysym.sym == SDLK_F2 && callbacks.data.all[OnF2]){
                on_f2_callback();
            } else if(event.key.keysym.sym == SDLK_F3 && callbacks.data.all[OnF3]){
                on_f3_callback();
            } else if(event.key.keysym.sym == SDLK_F4 && callbacks.data.all[OnF4]){
                on_f4_callback();
            } else if(event.key.keysym.sym == SDLK_F5 && callbacks.data.all[OnF5]){
                on_f5_callback();
            } else if(event.key.keysym.sym == SDLK_F6 && callbacks.data.all[OnF6]){
                on_f6_callback();
            } else if(event.key.keysym.sym == SDLK_F7 && callbacks.data.all[OnF7]){
                on_f7_callback();
            } else if(event.key.keysym.sym == SDLK_F8 && callbacks.data.all[OnF8]){
                on_f8_callback();
            } else if(event.key.keysym.sym == SDLK_F9 && callbacks.data.all[OnF9]){
                on_f9_callback();
            } else if(event.key.keysym.sym == SDLK_F10 && callbacks.data.all[OnF10]){
                on_f10_callback();
            } else if(event.key.keysym.sym == SDLK_F11 && callbacks.data.all[OnF11]){
                on_f11_callback();
            } else if(event.key.keysym.sym == SDLK_F12 && callbacks.data.all[OnF12]){
                on_f12_callback();
            }
            if(callbacks.data.all[CustomEvent]){
                custom_event_callback();
            } if(callbacks.data.all[OnResized]){
                if(was_resized.get()){
                    on_resized_callback();
                }
                was_resized.data = false;
                was_resized.leave();
            }
            callbacks.leave();
        } if(is_selected) {
            callbacks.hold();
            if(event.key.state == SDL_PRESSED){
                if(event.key.keysym.sym == SDLK_x && callbacks.data.all[OnCut] && ((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_GUI))){
                    on_cut_callback();
                } else if(event.key.keysym.sym == SDLK_c && callbacks.data.all[OnCopy] && ((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_GUI))){
                    on_copy_callback();
                } else if(event.key.keysym.sym == SDLK_v && callbacks.data.all[OnPaste] && ((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_GUI))){
                    on_paste_callback();
                }
            }
            if(callbacks.data.all[OnUnselected] && Core::get_selected_object() != this){
                on_unselected_callback();
                is_selected = false;
            }
            callbacks.leave();
        } if(nextInZ.get()){
            nextInZ.leave();
            if(renderMode.get() != CompositionMode) {
                renderMode.leave();
                _handle_others_routine(event, nextInZ.data, dpiK, false);
            } else {
                renderMode.leave();
            }
        } else
            nextInZ.leave();
        focus_repeated = true;
        no_focus_repeated = false;
    } else if(!no_focus_repeated) {
        if(callbacks.data.all[OnFocusLost] && !no_focus_repeated)
            on_focus_lost_callback();
        
        callbacks.leave();
        
        if(nextInZ.get()){
            nextInZ.leave();
            _handle_others_routine(event, nextInZ.data, dpiK, true);
        } else nextInZ.leave();
        
        focus_repeated = false;
        no_focus_repeated = true;
    }
    isActive.leave();
    inRootBoundsBuffer.leave();
    _handle_others(event, dpiK, no_focus);
}

InterativeObject& InterativeObject::set_size(const float w, const float h){
    Object::set_size(w, h);
    was_resized.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_focus_gained(VoidCallback callback){
    on_focus_gained_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnFocusGained] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_focus_lost(VoidCallback callback){
    on_focus_lost_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnFocusLost] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_selected(VoidCallback callback){
    on_selected_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnSelected] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_unselected(VoidCallback callback){
    on_unselected_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnUnselected] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_resized(VoidCallback callback){
    on_resized_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnResized] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_click(VoidCallback callback){
    on_click_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnClick] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_double_click(VoidCallback callback){
    on_double_click_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnDoubleClick] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_secondary_click(VoidCallback callback){
    on_secondary_click_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnSecondaryClick] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_mouse_button_down(VoidCallback callback){
    on_mouse_button_down_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnMouseButtonDown] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}


InterativeObject& InterativeObject::on_mouse_button_up(VoidCallback callback){
    on_mouse_button_up_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnMouseButtonUp] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_key_down(VoidCallback callback){
    on_key_down_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnKeyDown] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_key_up(VoidCallback callback){
    on_key_up_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnKeyUp] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_scroll(VoidCallback callback){
    on_scroll_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnScroll] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_drop_begin(VoidCallback callback){
    on_drop_begin_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnDropBegin] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_drop_end(VoidCallback callback){
    on_drop_end_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnDropEnd] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_drop_file(VoidCallback callback){
    on_drop_file_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnDropFile] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_drop_text(VoidCallback callback){
    on_drop_text_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnDropText] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_cut(VoidCallback callback){
    on_cut_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnCut] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_copy(VoidCallback callback){
    on_copy_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnCopy] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_paste(VoidCallback callback){
    on_paste_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnPaste] = true;
    callbacks.leave();
    isActive.set(true);
    
    
    return (*this);
}

InterativeObject& InterativeObject::on_audio_play(VoidCallback callback){
    on_audio_play_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnAudioPlay] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_audio_stop(VoidCallback callback){
    on_audio_stop_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnAudioStop] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_audio_next(VoidCallback callback){
    on_audio_next_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnAudioNext] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_audio_prev(VoidCallback callback){
    on_audio_prev_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnAudioPrev] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_audio_mute(VoidCallback callback){
    on_audio_mute_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnAudioMute] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_audio_rewind(VoidCallback callback){
    on_audio_rewind_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnAudioRewind] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_audio_fast_forward(VoidCallback callback){
    on_audio_fast_forward_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnAudioFastForward] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f1(VoidCallback callback){
    on_f1_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF1] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f2(VoidCallback callback){
    on_f2_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF2] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f3(VoidCallback callback){
    on_f3_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF3] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f4(VoidCallback callback){
    on_f4_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF4] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f5(VoidCallback callback){
    on_f5_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF5] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f6(VoidCallback callback){
    on_f6_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF6] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f7(VoidCallback callback){
    on_f7_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF7] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f8(VoidCallback callback){
    on_f8_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF8] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f9(VoidCallback callback){
    on_f9_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF9] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f10(VoidCallback callback){
    on_f10_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF10] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f11(VoidCallback callback){
    on_f11_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF11] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::on_f12(VoidCallback callback){
    on_f12_callback = callback;
    callbacks.hold();
    callbacks.data.all[OnF12] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}

InterativeObject& InterativeObject::custom_event(VoidCallback callback){
    custom_event_callback = callback;
    callbacks.hold();
    callbacks.data.all[CustomEvent] = true;
    callbacks.leave();
    isActive.set(true);
    return (*this);
}
