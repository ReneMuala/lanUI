//
//  playground.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"
#include <regex>
/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

int main(){
    Core olamundo;
    
    Window janela("hello", 400,500);
    
    janela.set_window_clear_color(Colors::White_smoke);
    
    Container cont;
    
    Object test;
    
    Paragraph parag;
    
    std::stringstream stream;
    
    stream << "\\size:36 \\color:rgb(255,255,255) " << "\\b Test \\n {10}";
    
    parag.from_stringstream(stream, Paragraph::Wrapper::Char, 23);
    parag.set_padding({20,20});
//    hello.bold(12).set_foreground_color(Colors::White).set_alignment(Object::Center);
    
    test.set_size(300,450);
            
    cont.set_content(test);
        
    cont.set_alignment(Object::Center);
    
    janela.embedInZ(cont);
    
    test.set_alignment(Object::Center);
    
    test.set_foreground_color(Colors::fromColorA(Colors::Orange_red, 255));
    test.set_background_color(Colors::Hot_pink);
    
    test.embedInZ(parag);
    
    test.set_renderer_callback(CallbackExpr({
        test.sizeBuffer.hold();
        test.foregroundColor.hold();
        test.backgroundColor.hold();
        static float radius = 50;

        primitives::roundedBoxColor(test.param_renderer, (int)test.sizeBuffer.data.x, (int)test.sizeBuffer.data.y, (int)test.sizeBuffer.data.w, (int)test.sizeBuffer.data.h, (int)radius*test.param_dpiK, test.foregroundColor.data);
        test.sizeBuffer.leave();
        test.foregroundColor.leave();
        test.backgroundColor.leave();
    }));
    
//    cont.compose(janela.sdlRenderer.get(), 2);
//    janela.sdlRenderer.leave();
//    cont.set_draw_mode(Object::CompositionMode);
    
//    test.compose(janela.sdlRenderer.get(), 2);
//    janela.sdlRenderer.leave();
//    test.set_draw_mode(Object::CompositionMode);
    
    test.compose(janela.sdlRenderer.get(), 2);
    janela.sdlRenderer.leave();
//    test.set_draw_mode(Object::CompositionMode);
    test.export_composition_as_PNG(janela.sdlRenderer.get(), "test2.png");
    janela.sdlRenderer.leave();
    test.compose(janela.sdlRenderer.get(), 2);
    janela.sdlRenderer.leave();
    test.set_draw_mode(Object::CompositionMode);
    
    olamundo.events();
}
