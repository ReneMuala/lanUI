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

class MemTest : public Object {
    
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

    janela.set_window_clear_color(Colors::Blue_violet);
    
    Semaphore<std::string> fieldData;
//
    auto field = TextField(fieldData, "<>");
//
    field.set_foreground_color(Colors::White);
//
//    field.set_border_color(Colors::Blue.from_a(100));
//
    field.set_alignment(Object::Center);
//
    field.set_size(200, 25);
//
    field.set_input_style("\\r \\size:13 \\color:rgb(0,0,0)");
//
    janela.embedInZ(field);
//
    field.set_alignment(Object::Center);
//
    Text test("Hello world");
//
    field.embedInY(test);
    
    olamundo.events();
    
}
