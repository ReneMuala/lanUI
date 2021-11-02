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

    janela.set_window_clear_color(Colors::Light_gray);
    
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
    
    Semaphore<std::string> fieldData,fieldData2;

    auto field = TextField(fieldData, "Username");
    auto field2 = TextField(fieldData2, "Username");
    
    field.set_foreground_color(Colors::White);
    field2.set_foreground_color(Colors::Dark_gray);
    
//    field.set_border_color(Colors::Blue.from_a(100));

    field.set_size(100, 25);
    field2.set_size(100, 25);

    field.textSurface.regular(13);
    field2.textSurface.regular(13);

    janela.embedInZ(field);

    field.set_alignment(Object::Center);
    field.embedInY(field2);
    
    olamundo.events();
    
}
