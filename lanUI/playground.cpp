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
    
    Window janela("hello", 300,300);
    
    janela.set_window_clear_color(Colors::White_smoke);
    
    DrawableObject test;
    
    Text hello("Continue");
    
    hello.bold(12).set_foreground_color(Colors::White).set_alignment(Object::Right);
    
    test.embedInZ(hello);
    
    test.set_size(150,30);
    
    hello.set_padding({0,0});
        
    janela.embedInZ(test);
    
    test.set_alignment(Object::Center);
    
    test.set_renderer_callback(CallbackExpr({
        test.sizeBuffer.hold();
        test.foregroundColor.hold();
        test.backgroundColor.hold();
        static float radius = 15;

        primitives::roundedBoxColor(test.renderer_param, (int)test.sizeBuffer.data.x, (int)test.sizeBuffer.data.y, (int)test.sizeBuffer.data.w, (int)test.sizeBuffer.data.h, (int)radius*test.dpiK_param, test.foregroundColor.data);
        test.sizeBuffer.leave();
        test.foregroundColor.leave();
        test.backgroundColor.leave();
    }));
    
    test.set_foreground_color(Colors::fromColorA(Colors::Black, 200));
    test.set_background_color(Colors::Hot_pink);
    
    olamundo.events();
}
