//
//  TextField.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "TextField.hpp"
#include "../../Core/Core.hpp"
#include <regex>
#include <string>
#include <iostream>

void TextField::__default_on_selected_callback(){
    activated.set(true);
    textSurface.cursor.get().active = true;
    textSurface.cursor.leave();
    SDL_StartTextInput();
}

void TextField::__default_on_unselected_callback(){
    if(activated.get()){
        SDL_StopTextInput();
        activated.data = false;
        activated.leave();
        textSurface.cursor.get().active = false;
        textSurface.cursor.leave();
    } else
        activated.leave();
}

void TextField::_init(Semaphore<std::string> &source, const std::string placeholder){
    activated.set(false);
    set_size(150, 25);
    cursor_change_flag = 0;
    this->source = &source;
    this->placeholder = placeholder;
    Core::log(Core::Warning, "TextField isn't stable yet.");
    disable_reloading();

    on_copy(CallbackExpr({
        if(!source.get().empty())
            SDL_SetClipboardText(source.data.c_str());
        source.leave();
    }));
    
    on_cut(CallbackExpr());
    
    on_paste(CallbackExpr({
        char * buffer = SDL_GetClipboardText();
        const UTF8CharList clipboardText = UTF8CharList(buffer);
        const size_t clipTextSize = clipboardText.size();
        printf("PASTE \"%s\" (%ld)\n", buffer, clipTextSize);
        if(clipTextSize){
            textSurface.cursor.hold();
            inputBuffer.get().append(buffer, textSurface.cursor.data.colummn);
            textSurface.cursor.data.colummn += clipTextSize;
            textSurface.cursor.leave();
            inputBuffer.leave();
            wasCompiled.set(false);
            SDL_free(buffer);
        }
    }));
    
    on_selected(CallbackExpr(__default_on_selected_callback();));
    
    on_unselected(CallbackExpr(__default_on_unselected_callback();));
    
    set_content(textSurface);
}

void TextField::_refresh_cursor(const float dpiK){
    if(cursor_change_flag){
        if(cursor_change_flag > 0){
            if(input_size_change) textSurface.cursor.get().colummn+=input_size_change;
            else textSurface.cursor.get().colummn++;
        } else {
            textSurface.cursor.hold();
            if(textSurface.cursor.data.colummn - 1 < 0){
                textSurface.cursor.data.colummn = 0;
            } else {
                textSurface.cursor.data.colummn--;
            }
        }
        
        if(textSurface.cursor.data.colummn > input_size){
            textSurface.cursor.data.colummn = input_size;
        }
        
        textSurface.cursor.data.hidden = false;
        if(!input_size) textSurface.cursor.data.empty = true;
        else textSurface.cursor.data.empty = false;
        
        textSurface.cursor.leave();
        
        input_size_change = 0;
        cursor_change_flag = 0;
        }
}

void TextField::_compute_cursor_position(Renderer * renderer){
    if(textSurface.font && !textSurface.source.get().empty()){
        if(textSurface.font->ttfFont.get() && textSurface.font->scalingFactorConstant > 1){
            const int constantBuffer = textSurface.font->scalingFactorConstant;
            textSurface.font->ttfFont.leave();
            textSurface.font->set_scaling_factor(1);
            int w, h;
            
            
            textSurface.cursor.hold();
            
            inputBuffer.hold();
            
            const size_t text_len_at_cur = inputBuffer.data.c_index_before(textSurface.cursor.data.colummn);
            
            inputBuffer.leave();
            //(text_len_at_cur-1>0 ? text_len_at_cur-1 : 0)
            TTF_SizeUTF8(textSurface.font->ttfFont.data, textSurface.source.data.substr(0, text_len_at_cur).c_str(), &w, &h);
            textSurface.cursor.data.hBuffer = h;
            textSurface.cursor.data.xBuffer = w;
            
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

void TextField::_compile(Renderer * renderer, const float dpiK){
    if(!wasCompiled.get()){
        _sync_strings();
        if(source->get().empty()){
            textSurface.from_string(placeholder);
        } else {
            textSurface.from_string(source->data.c_str());
        } source->leave();
        _refresh_cursor(dpiK);
        wasCompiled.data = true;
    } wasCompiled.leave();
    _compute_cursor_position(renderer);
    _compile_embedded(renderer, dpiK);
}

TextField::TextField(Semaphore<std::string>&source, const std::string placeholder){
    _init(source, placeholder);
}

TextField::TextField(TextField const & other){
    void * ptr = (void*)&other.source;
    _init((Semaphore<std::string>&)ptr, other.placeholder);
}


void TextField::_sync_strings(){
    
    size_t c_string_len = inputBuffer.get().c_str_size();
    
    char c_string [c_string_len];
    
    bzero(c_string, c_string_len);
    
    c_string[0] = '\0';
        
    inputBuffer.data.composeCStr(c_string, c_string_len);
    
    source->set(c_string);
        
    input_size = inputBuffer.data.size();
            
    inputBuffer.leave();
}

void TextField::_handle_events(Event & event, const float dpiK, const bool no_focus){
    InterativeObject::_handle_events(event, dpiK, no_focus);
    if(activated.get()){
        if(!SDL_IsTextInputActive()){
            SDL_StartTextInput();
        }
        if(event.type == SDL_TEXTINPUT){
            input_size_change +=
            (textSurface.cursor.get().colummn < input_size) ?
            inputBuffer.get().append(event.text.text, textSurface.cursor.data.colummn) :
            inputBuffer.get().append(event.text.text);
            textSurface.cursor.leave();
            inputBuffer.leave();
            wasCompiled.set(false);
            cursor_change_flag = 1;
        } else if (event.key.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE){
                if (textSurface.cursor.get().colummn < input_size){
                    if(textSurface.cursor.data.colummn-1 >= 0){
                        inputBuffer.get().remove(textSurface.cursor.data.colummn-1);
                    } textSurface.cursor.leave();
                } else {
                    inputBuffer.get().remove_last();
                    textSurface.cursor.leave();
                } inputBuffer.leave();
                wasCompiled.set(false);
                cursor_change_flag = -1;
            } else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_TAB){
                Core::set_selected_object(nullptr);
            } else if (event.key.keysym.sym == SDLK_LEFT){
                if(((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_GUI))){
                    textSurface.cursor.get().colummn = 0;
                    textSurface.cursor.leave();
                } else cursor_change_flag = -1;
                wasCompiled.set(false);
            } else if (event.key.keysym.sym == SDLK_RIGHT){
                if(((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_GUI))){
                    textSurface.cursor.get().colummn = input_size;
                    textSurface.cursor.leave();
                } else cursor_change_flag = 1;
                wasCompiled.set(false);
            }
        }
    } activated.leave();
}
