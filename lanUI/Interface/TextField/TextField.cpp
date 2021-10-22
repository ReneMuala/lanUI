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

TextField::TextSurface::Cursor::Cursor(): line(0), colummn(0), empty(true),active(false), hidden(false), size({0,0,0,0}), color(Colors::Dark_cyan){
}

void TextField::TextSurface::_render(SDL_Renderer * renderer, float x, float y, float dpiK, bool inComposition){
    Text * target = (nextInZ.get()) ? (Text*)((HStack*)nextInZ.data)->requestObject((cursor.data.colummn-1 < 0) ? 0 : cursor.data.colummn-1) : nullptr;
    nextInZ.leave();
    
    
    Paragraph::_render(renderer, x, y, dpiK, inComposition);
    cursor.hold();

    if(target && cursor.data.active && !cursor.data.hidden){
        target->sizeBuffer.hold();
        
        cursor.data.size = {
            (!cursor.data.colummn) ? target->sizeBuffer.data.x : target->sizeBuffer.data.x + target->sizeBuffer.data.w,
            target->sizeBuffer.data.y,
            2 * dpiK,
            target->sizeBuffer.data.h,
        };
        
        if(cursor.data.size.x > sizeBuffer.get().x + sizeBuffer.data.w){
            first->scrollingFactor.get().horizontal = -(((cursor.data.size.x + cursor.data.size.w) - (sizeBuffer.data.x + sizeBuffer.data.w))/dpiK);
            first->scrollingFactor.leave();
        } sizeBuffer.leave();
        
        target->sizeBuffer.leave();
        SDL_SetRenderDrawColor(renderer, cursor.data.color.r, cursor.data.color.g, cursor.data.color.b, cursor.data.color.a);
        SDL_RenderFillRectF(renderer, &cursor.data.size);
    } else if (!cursor.data.active){
        first->scrollingFactor.get().horizontal = 0;
        first->scrollingFactor.leave();
    }
    cursor.leave();
}

void TextField::_init(Semaphore<std::string>& source, const std::string placeholder){
    activated = (false);
    this->source = (source);
    this->placeholder = (placeholder);
    stream.str("");
    Core::log(Core::Warning, "TextField isn't stable yet.");
    set_size(150, 25);
    disable_reloading();
    textSurface.disable_reloading();
    set_draw_mode(ColorSchemeMode);
    textSurface.set_alignment(Left);
    textSurface.set_padding({0,5});
    
    textSurface.set_default_animation
    (60,CallbackExpr({
        textSurface.cursor.get().hidden = !textSurface.cursor.data.hidden;
        textSurface.cursor.leave();
        return true;
    }));
    
    on_click(CallbackExpr({
        activated.set(true);
        textSurface.cursor.get().active = true;
        textSurface.cursor.leave();
        SDL_StartTextInput();
    }));
    
    on_focus_lost(CallbackExpr({
        if(activated.get()){
            SDL_StopTextInput();
            activated.data = false;
            activated.leave();
            textSurface.cursor.get().active = false;
            textSurface.cursor.leave();
        } else
            activated.leave();
    }));
    
    inputStyle.set("\\regular \\size:13 \\color:rgb(0,0,0)");
    placeholderStyle.set("\\regular \\size:13 \\color:rgba(0,0,0,100)");
    
    set_content(textSurface);
}

TextField::TextField(Semaphore<std::string>& source, const std::string placeholder){
    _init(source, placeholder);
}

TextField::TextField(TextField const & other){
    void * ptr = (void*)&other.source;
    _init((Semaphore<std::string>&)ptr, other.placeholder);
}

TextField& TextField::set_size(const float w, const float h){
    if(w < 5 || h < 5)
        Core::log(Core::Error, "TextField::set_size(...) w and h must to be greater than 5px");
    textSurface.set_size(w-5, h-5);
    InterativeObject::set_size(w, h);
    return (*this);
}

void TextField::_sync_strings(){
    
    size_t c_string_len = inputBuffer.get().c_str_size();
    
    char c_string [c_string_len];
    
    bzero(c_string, c_string_len);
    
    c_string[0] = '\0';
        
    inputBuffer.data.composeCStr(c_string, c_string_len);
    
    source.set(c_string);
    
    input_size = inputBuffer.data.size();
            
    inputBuffer.leave();
}

void TextField::_compile_source(){
    static const std::regex spaceInput("[[:space:]]");
    std::string buffer;
    inputBuffer.hold();
    stream.clear();
    stream << inputStyle.get() << ' ';
    inputStyle.leave();
    for (int i = 0 ; i < inputBuffer.data.size() ; i++) {
        buffer = inputBuffer.data[i];
        if(std::regex_match(buffer, spaceInput)){
            stream << " \\s ";
        } else {
            stream << " \\ns " << buffer;
        }
    } inputBuffer.leave();
}

void TextField::_compile(Renderer * renderer, const float dpiK){
    if(!wasCompiled.get()){
        wasCompiled.data = true;
        wasCompiled.leave();
        
        _sync_strings();
        
        _refresh_cursor(dpiK);
        
        source.hold();
        if(source.data.empty()){
            stream.clear();
            stream << placeholderStyle.get() << ' ' << placeholder;
            placeholderStyle.leave();
        } else {
            _compile_source();
        } source.leave();
        textSurface.from_stringstream(stream);
    } else wasCompiled.leave();
    _compile_embedded(renderer, dpiK);
}

TextField& TextField::set_input_style(const std::string style){
    inputStyle.set(style);
    return (*this);
}

TextField& TextField::set_placeholder_style(const std::string style){
    placeholderStyle.set(style);
    return (*this);
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

void TextField::_handle_events(Event & event, const float dpiK, const bool no_focus){
    InterativeObject::_handle_events(event, dpiK, no_focus);
    if(activated.get()){
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
                activated = false;
            } else if (event.key.keysym.sym == SDLK_LEFT){
                cursor_change_flag = -1;
                wasCompiled.set(false);
            } else if (event.key.keysym.sym == SDLK_RIGHT){
                cursor_change_flag = 1;
                wasCompiled.set(false);
            }
        }
        activated.leave();
    } else
        activated.leave();
}
