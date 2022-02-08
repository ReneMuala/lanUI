//
//  TextField.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "TextField.hpp"
#include <regex>
#include <string>
#include <iostream>
#include "../Window/WindowManager/WindowManager.hpp"

void BSTextField::__default_on_selected_callback(){
    activated.set(true);
    textSurface.cursor.get().active = true;
    textSurface.cursor.leave();
    SDL_StartTextInput();
}

void BSTextField::__default_on_unselected_callback(){
    if(activated.get()){
        SDL_StopTextInput();
        activated.data = false;
        activated.leave();
        textSurface.cursor.get().active = false;
        textSurface.cursor.leave();
    } else
        activated.leave();
}

void BSTextField::__default_on_copy_callback(){
    if(!data.get().empty())
        SDL_SetClipboardText(data.data.c_str());
    data.leave();
}

void BSTextField::__default_on_cut_callback(){
    if(!data.get().empty())
        SDL_SetClipboardText(data.data.c_str());
    data.leave();
    inputBuffer.get().clear();
    inputBuffer.leave();
    wasCompiled.set(false);
    input_size = 0;
    
    if(BSTextFieldCallbacks.data.all[OnChange]){
        on_change_callback();
    }
}

void BSTextField::__default_on_paste_callback(){
    char * buffer = SDL_GetClipboardText();
    const UTF8CharList clipboardText = UTF8CharList(buffer);
    const size_t clipTextSize = clipboardText.size();
    if(clipTextSize){
        textSurface.cursor.hold();
        inputBuffer.get().append(buffer, textSurface.cursor.data.colummn);
        textSurface.cursor.data.colummn += clipTextSize;
        textSurface.cursor.leave();
        inputBuffer.leave();
        wasCompiled.set(false);
        SDL_free(buffer);
        
        if(BSTextFieldCallbacks.data.all[OnChange]){
            on_change_callback();
        }
    }
}

void BSTextField::_init(const std::string placeholder){
    activated.set(false);
    isSecret.set(false);
    set_size(150, 25);
    cursor_change_flag = 0;
    this->placeholder = placeholder;
    WindowManager::log(WindowManager::Warning, "TextField isn't stable yet.");
    disable_reloading();
    
    for(int i = 0 ; i < BSTextFieldCallbackTotal ; i++)
    BSTextFieldCallbacks.data.all[i] = false;

    on_copy(CallbackExpr(__default_on_copy_callback();));
    
    on_cut(CallbackExpr(__default_on_cut_callback();));
    
    on_paste(CallbackExpr(__default_on_paste_callback();));
    
    on_selected(CallbackExpr(__default_on_selected_callback();));
    
    on_unselected(CallbackExpr(__default_on_unselected_callback();));
    
    set_content(textSurface);
}

void BSTextField::_refresh_cursor(const float dpiK){
    if(cursor_change_flag){
        textSurface.cursor.hold();
        
        if(cursor_change_flag != 0){
            if(input_size_change) textSurface.cursor.data.colummn+=input_size_change;
            else textSurface.cursor.data.colummn+=cursor_change_flag;
        }
        
        if(textSurface.cursor.data.colummn > input_size){
            textSurface.cursor.data.colummn = input_size;
        } else if (textSurface.cursor.data.colummn < 0) {
            textSurface.cursor.data.colummn = 0;
        }
        
        textSurface.cursor.data.hidden = false;
        if(!input_size) textSurface.cursor.data.empty = true;
        else textSurface.cursor.data.empty = false;
        
        textSurface.cursor.leave();
        
        input_size_change = 0;
        cursor_change_flag = 0;
        }
}

void BSTextField::_compute_cursor_position(Renderer * renderer){
    if(textSurface.font && !textSurface.source.get().empty()){
        if(textSurface.font->ttfFont.get() && textSurface.font->scalingFactorConstant >= 1){
            const int constantBuffer = textSurface.font->scalingFactorConstant;
            textSurface.font->ttfFont.leave();
            textSurface.font->set_scaling_factor(1);
            int w, h;
            
            textSurface.cursor.hold();
            
            inputBuffer.hold();
            
            size_t text_len_at_cur = (isSecret.get()) ? secretStrBuffer.c_index_before(textSurface.cursor.data.colummn) : inputBuffer.data.c_index_before(textSurface.cursor.data.colummn);
            
            isSecret.leave();
            inputBuffer.leave();
            //(text_len_at_cur-1>0 ? text_len_at_cur-1 : 0)
            
            TTF_SizeUTF8(textSurface.font->ttfFont.data, textSurface.source.data.substr(0, text_len_at_cur).c_str(), &w, &h);
            textSurface.cursor.data.hBuffer = h;
            textSurface.cursor.data.xBuffer = w;
            
//            std::cout << textSurface.cursor.data.xBuffer << " " << textSurface.cursor.data.hBuffer << std::endl;
            
            size.hold();
            textSurface.padding.hold();
            
            textSurface.scrollingFactor.hold();
            
            const float lhs = w + textSurface.scrollingFactor.data.horizontal;
            const float rlhs = size.data.w - textSurface.padding.data.left - textSurface.padding.data.right;
            
            if (lhs < 0) {
                textSurface.scrollingFactor.data.horizontal += (size.data.w/2);
                if(textSurface.scrollingFactor.data.horizontal > 0)
                    textSurface.scrollingFactor.data.horizontal = 0;
            } else if(lhs > rlhs && lhs > 0){
                textSurface.scrollingFactor.data.horizontal = -(w - rlhs);
            }
            textSurface.scrollingFactor.leave();

            textSurface.padding.leave();
            size.leave();
            
            textSurface.cursor.leave();
            textSurface.font->set_scaling_factor(constantBuffer);
        } else
            textSurface.font->ttfFont.leave();
    } textSurface.source.leave();
    textSurface.source.leave();
}

void BSTextField::_compile(Renderer * renderer, const float dpiK){
    if(!wasCompiled.get()){
        _sync_strings();
        if(data.get().empty()){
            textSurface.from_string(placeholder);
        } else {
            if(isSecret.get()){
                textSurface.from_string(secretStr.c_str());
            } else {
                textSurface.from_string(data.data.c_str());
            } isSecret.leave();
        }
        data.leave();
        _refresh_cursor(dpiK);
        wasCompiled.data = true;
    } wasCompiled.leave();
    _compute_cursor_position(renderer);
    _compile_embedded(renderer, dpiK);
}

std::string BSTextField::get_data() {
    _sync_strings();
    std::string str = data.get();
    data.leave();
    return str;
}

size_t BSTextField::get_data_length(){
    size_t size = data.get().length(); data.leave();
    return size;
}

BSTextField& BSTextField::secret(const bool isSecret, const UTF8Char mask){
    this->isSecret.set(isSecret);
    char c_char[5];
    bzero(c_char, 5);
    strcpy(c_char, mask.c_char());
    secretStrMask.composeUTF8Char(c_char);
    return (*this);
}

BSTextField& BSTextField::on_empty(VoidCallback callback){
    on_empty_callback = callback;
    BSTextFieldCallbacks.hold();
    BSTextFieldCallbacks.data.all[OnEmpty] = true;
    BSTextFieldCallbacks.leave();
    isActive.set(true);
    return (*this);
}

BSTextField& BSTextField::on_not_empty(VoidCallback callback){
    on_not_empty_callback = callback;
    BSTextFieldCallbacks.hold();
    BSTextFieldCallbacks.data.all[OnNotEmpty] = true;
    BSTextFieldCallbacks.leave();
    isActive.set(true);
    return (*this);
}

BSTextField& BSTextField::on_change(VoidCallback callback){
    on_change_callback = callback;
    BSTextFieldCallbacks.hold();
    BSTextFieldCallbacks.data.all[OnChange] = true;
    BSTextFieldCallbacks.leave();
    isActive.set(true);
    return (*this);
}

BSTextField& BSTextField::on_submit(VoidCallback callback){
    on_submit_callback = callback;
    BSTextFieldCallbacks.hold();
    BSTextFieldCallbacks.data.all[OnSubmit] = true;
    BSTextFieldCallbacks.leave();
    isActive.set(true);
    return (*this);
}

BSTextField& BSTextField::on_delete(VoidCallback callback){
    on_delete_callback = callback;
    BSTextFieldCallbacks.hold();
    BSTextFieldCallbacks.data.all[OnDelete] = true;
    BSTextFieldCallbacks.leave();
    isActive.set(true);
    return (*this);
}

BSTextField::BSTextField(const std::string placeholder){
    _init(placeholder);
}

BSTextField::BSTextField(BSTextField const & other){
    _init(other.placeholder);
}


void BSTextField::_sync_strings() {
    
    char * c_string = inputBuffer.get().alloc_c_str();
    
    data.hold();

    if((input_size = inputBuffer.data.size())){
        if(isSecret.get()){
            secretStr.clear();
            for(int i = 0;i < input_size ; i++, secretStr+=secretStrMask.c_char());
            secretStrBuffer.clear();
            secretStrBuffer.append(secretStr.c_str());
        } isSecret.leave();
        data.data = c_string;
    } else {
        data.data.clear();
    }
    data.leave();
    
    inputBuffer.data.free_c_str(c_string);
    inputBuffer.leave();
}

void BSTextField::_handle_events(Event & event, const float dpiK, const bool no_focus){
    InteractiveObject::_handle_events(event, dpiK, no_focus);
    if(activated.get()){
        if(!SDL_IsTextInputActive()){
            SDL_StartTextInput();
        }
        
        inputBuffer.hold();
        BSTextFieldCallbacks.hold();
        
        if(event.type == SDL_TEXTINPUT){
            input_size_change +=
            (textSurface.cursor.get().colummn < input_size) ?
            inputBuffer.data.append(event.text.text, textSurface.cursor.data.colummn) :
            inputBuffer.data.append(event.text.text);
            textSurface.cursor.leave();
            wasCompiled.set(false);
            cursor_change_flag = 1;
            inputBuffer.leave();

            if(BSTextFieldCallbacks.data.all[OnChange]){
                on_change_callback();
            }
            
        } else if (event.key.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE){
                if (textSurface.cursor.get().colummn < input_size){
                    if(textSurface.cursor.data.colummn-1 >= 0){
                        inputBuffer.data.remove(textSurface.cursor.data.colummn-1);
                    } textSurface.cursor.leave();
                } else {
                    inputBuffer.data.remove_last();
                    textSurface.cursor.leave();
                }
                wasCompiled.set(false);
                cursor_change_flag = -1;
                inputBuffer.leave();

                if(BSTextFieldCallbacks.data.all[OnChange]){
                    on_change_callback();
                } if(BSTextFieldCallbacks.data.all[OnDelete]){
                    on_delete_callback();
                }
            } else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_TAB){
                WindowManager::set_selected_object(nullptr);
                if(BSTextFieldCallbacks.data.all[OnSubmit]){
                    on_submit_callback();
                }
            } else if (event.key.keysym.sym == SDLK_LEFT){
                if(((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_GUI))){
                    textSurface.cursor.get().colummn = 0;
                    textSurface.cursor.leave();
                } else if (SDL_GetModState() & KMOD_SHIFT){
                    cursor_change_flag = -4;
                } else cursor_change_flag = -1;
                wasCompiled.set(false);
            } else if (event.key.keysym.sym == SDLK_RIGHT){
                if(((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_GUI))){
                    textSurface.cursor.get().colummn = input_size;
                    textSurface.cursor.leave();
                } else if (SDL_GetModState() & KMOD_SHIFT){
                    cursor_change_flag = +4;
                } else cursor_change_flag = +1;
                wasCompiled.set(false);
            }
        }
        
        BSTextFieldCallbacks.leave();
        
    } activated.leave();
    
    BSTextFieldCallbacks.hold();
    
    inputBuffer.force_hold();
    
    if(BSTextFieldCallbacks.data.all[OnEmpty] && !BSTextFieldCallbacks.data.onEmptyTriggered && !inputBuffer.data.size()){
        inputBuffer.leave();
        on_empty_callback();
        BSTextFieldCallbacks.data.onEmptyTriggered = true;
    } else if (inputBuffer.data.size()) BSTextFieldCallbacks.data.onEmptyTriggered = false;
    
    inputBuffer.force_hold();
    
    if(BSTextFieldCallbacks.data.all[OnNotEmpty] && !BSTextFieldCallbacks.data.onNotEmptyTriggered && inputBuffer.data.size()){
        inputBuffer.leave();
        on_not_empty_callback();
        BSTextFieldCallbacks.data.onNotEmptyTriggered = true;
    } else if (!inputBuffer.data.size()) BSTextFieldCallbacks.data.onNotEmptyTriggered = false;
    inputBuffer.leave();
    BSTextFieldCallbacks.leave();
}
