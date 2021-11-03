//
//  playground.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"
#include <regex>

#include "Interface/TextField/TextField.hpp"


/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */
#include <iostream>

#define LANUI_MEM_TESTING

#ifdef LANUI_MEM_TESTING

class MemTest : public Text {
    
};


void TestMemory(size_t numObj, size_t numAlloc){
    MemTest * objs[numObj];
    
    for (int j = 0 ; j < numAlloc ; j++){
        for(int i = 0 ; i < numObj ; i ++){
            objs[i] = new MemTest;
    //        texts[i] -> from_string("Hello world", janela.sdlRenderer.get());
        }
        
        for(int i = 0 ; i < numObj ; i ++){
            delete objs[i];
            objs[i] = nullptr;
        }
    }
}

#endif

int main(){
    
    Core olamundo;
    
    Window janela("hello", 400,500);

    janela.set_window_clear_color(Colors::White);
    
    /*
    
    MemTest * objs[10];
    
    for (int j = 0 ; j < 1000 ; j++){
        for(int i = 0 ; i < 10 ; i ++){
            objs[i] = new MemTest;
            objs[i] -> from_string("lanUI.Bundle/System/Resources/rock.png", janela.sdlRenderer.get());
//            objs[i] -> fromFile("lanUI.Bundle/System/Resources/rock.png", janela.sdlRenderer.get());
            janela.sdlRenderer.leave();
            Core::clearCache();
        }
        
        for(int i = 0 ; i < 10 ; i ++){
            delete objs[i];
            objs[i] = nullptr;
        }
        //std::this_thread::sleep_for((std::chrono::milliseconds)500);
    }
    olamundo.terminate();
    return 0;
     */
        
    Paragraph parag;
    
    std::stringstream stream;
    for (int i = 0 ; i < 1; i++) {
        stream.clear();
        
        stream << "\\FONT:opensans \\size:28 New \\Color:RGBA(10,0,200,255) \\b Open \\R Sans \\n ";
        
        stream << "\\Size:15 "  << Lorem;
        
        parag.from_stringstream(stream, Paragraph::Wrapper::Char, 30);
    }
    
    parag.set_alignment(Object::Center);
    
    janela.embedInZ(parag);
    
    olamundo.events();
    
}
