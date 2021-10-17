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
#include <unicode/unistr.h>
#include <iostream>
#include <clocale>

void TextField::_init(Semaphore<std::string>& source, const std::string placeholder){
    activated = (false);
    this->source = (source);
    this->placeholder = (placeholder);
    uniSource.remove();
    stream.str("");
    Core::log(Core::Warning, "TextField isn't stable yet.");
    disable_reloading();
    set_draw_mode(ColorSchemeMode);
    textSurface.set_alignment(Left);
    textSurface.set_padding({5,5});
    
    on_click(CallbackExpr({
        activated.set(true);
        SDL_StartTextInput();
    }));
    
    on_focus_lost(CallbackExpr({
        if(activated.get()){
            SDL_StopTextInput();
            activated.data = false;
            activated.leave();
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

TextField::~TextField(){
    
}

void TextField::_sync_strings(){
    if(inputBuffer.get().length()){
        std::mbstate_t state;
        
        size_t c_wstr_len;
        const char * _char = inputBuffer.data.c_str();
        // get the len
        c_wstr_len = mbsrtowcs(NULL, &_char, 0, &state);
        
        if(c_wstr_len){
            // initialize buffer
            std::wstring wbuffer(c_wstr_len, L'\0');
            
            size_t converted_len = mbsrtowcs(&wbuffer[0], &_char, c_wstr_len, &state);
            
            if(converted_len == static_cast<std::size_t>(-1)){
                std::cout << "error!" << std::endl;
            }
            
            if(converted_len){
                std::cout << "working, len: " << converted_len << std::endl;
            }
        }
        inputBuffer.leave();
    } else
        inputBuffer.leave();
    
    wideSource.leave();
}

void TextField::_compile_source(){
    std::regex spaceInput("[[:space:]]");
    std::regex textInput("[[:print:]]");
    
        
    stream << source.get();
    source.leave();
    
//    for (char element : source.get()) {
//        buffer = element;
//        if(std::regex_match(buffer, spaceInput)){
//            stream << " \\s ";
//        } else if (std::regex_match(buffer, textInput)){
//            stream << " \\ns " << buffer;
//        }
//    } source.leave();
}

void TextField::_compile(Renderer * renderer, const float dpiK){
    if(!wasCompiled.get()){
        wasCompiled.data = true;
        wasCompiled.leave();
        
        _sync_strings();
        
        stream.clear();
        source.hold();
        if(source.data.empty()){
            source.leave();
            stream << placeholderStyle.get() << ' ' << placeholder;
            placeholderStyle.leave();
        } else {
            source.leave();
            stream << inputStyle.get() << ' ';
            _compile_source();
            inputStyle.leave();
        }
        
        source.leave();
        textSurface.from_stringstream(stream);
    } else
        wasCompiled.leave();
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

void TextField::_handle_events(Event & event, const float dpiK, const bool no_focus){
    InterativeObject::_handle_events(event, dpiK, no_focus);
    if(activated.get()){
        if(event.type == SDL_TEXTINPUT){
            inputBuffer.get() +=event.text.text;
            inputBuffer.leave();
            wasCompiled.set(false);
        } if (event.key.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE){
                if(!wideSource.get().empty()){
                    wideSource.data.pop_back();
                } wideSource.leave();
                wasCompiled.set(false);
            }
        }
        activated.leave();
    } else
        activated.leave();
}
