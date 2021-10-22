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
    Paragraph::_render(renderer, x, y, dpiK, inComposition);
    cursor.hold();
    Object * letter = nullptr;
    if(nextInZ.get()){
        letter = ((HStack *)nextInZ.data)->requestObject((cursor.data.colummn-1 < 0) ? 0 : cursor.data.colummn-1);
    } nextInZ.leave();

    if(letter && cursor.data.active && !cursor.data.hidden){
        letter->sizeBuffer.hold();
        
        cursor.data.size = {
            (!cursor.data.colummn) ? letter->sizeBuffer.data.x : letter->sizeBuffer.data.x + letter->sizeBuffer.data.w,
            letter->sizeBuffer.data.y,
            2 * dpiK,
            letter->sizeBuffer.data.h,
        };
        
        letter->sizeBuffer.leave();
        SDL_SetRenderDrawColor(renderer, cursor.data.color.r, cursor.data.color.g, cursor.data.color.b, cursor.data.color.a);
        SDL_RenderFillRectF(renderer, &cursor.data.size);
    }
    cursor.leave();
}

void TextField::_init(Semaphore<std::string>& source, const std::string placeholder){
    activated = (false);
    this->source = (source);
    this->placeholder = (placeholder);
    stream.str("");
    Core::log(Core::Warning, "TextField isn't stable yet.");
    disable_reloading();
    set_draw_mode(ColorSchemeMode);
    textSurface.set_alignment(Left);
    textSurface.set_padding({5,5});
    
    textSurface.set_default_animation
    (60,CallbackExpr({
        textSurface.cursor.get().hidden = !textSurface.cursor.data.hidden;
        textSurface.cursor.leave();
        return true;
    }));
    
    set_default_animation
    (0, CallbackExpr({
//        textSurface.scrollingFactor.hold();
//        sizeBuffer.hold();
//        textSurface.cursor.hold();
//
//        const float sizeBuffer_x_area;
//        sizeBuffer_x_area = (sizeBuffer.data.x/param_dpiK) + (sizeBuffer.data.w/param_dpiK);
//        const float cursor_x_area;
//        cursor_x_area = (textSurface.cursor.data.size.x/param_dpiK)+(textSurface.cursor.data.size.w/param_dpiK);
//
////        if(textSurface.cursor.data.active && cursor_x_area >= sizeBuffer_x_area){
////            textSurface.scrollingFactor.data.horizontal -= 5;
////            std::cout << ">>" << textSurface.scrollingFactor.data.horizontal <<std::endl;
////        }
//        printf("%f %f\n", textSurface.cursor.data.size.x, sizeBuffer_x_area);
////        else {
//////            printf("<<\n");
////        }
//
//        textSurface.cursor.leave();
//        sizeBuffer.leave();
//        textSurface.scrollingFactor.leave();
        return true;
    }));
    
    on_focus_gained(CallbackExpr({
        printf("Has focus\n");
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
