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
    
    Window janela("hello");
    
    janela.set_window_clear_color(Colors::White);
    
    DrawableObject test;
    
    test.set_size(200,200);
    
    janela.embedInZ(test);
    
    test.set_alignment(Object::Center);
    
    test.set_renderer_callback(CallbackExpr({
        test.sizeBuffer.hold();
        test.foregroundColor.hold();
        test.backgroundColor.hold();
        static float radius = 10;
//        static bool decr = true;
//        if(decr && radius > 5) {
//            radius-=0.5;
//        } else if(!decr && radius < 20){
//            radius+=0.5;
//        } else {
//            decr = !decr;
//        }
//        primitives::simpleRoundedRectangleRGBA(test.renderer_param, (int)test.sizeBuffer.data.x, (int)test.sizeBuffer.data.y, (int)test.sizeBuffer.data.w, (int)test.sizeBuffer.data.h, radius*test.dpiK_param, test.foregroundColor.data.r, test.foregroundColor.data.g, test.foregroundColor.data.b, test.foregroundColor.data.a);
        
//        primitives::rectangleColor(test.renderer_param, test.sizeBuffer.data.x, test.sizeBuffer.data.y, test.sizeBuffer.data.w, test.sizeBuffer.data.h, test.foregroundColor.data);
        
//        primitives::roundedBoxColor(test.renderer_param, test.sizeBuffer.data.x, test.sizeBuffer.data.y, test.sizeBuffer.data.w, test.sizeBuffer.data.h, 30, test.foregroundColor.data);
        
        primitives::filledTrigonColor(test.renderer_param, test.sizeBuffer.data.x + test.sizeBuffer.data.w / 2, test.sizeBuffer.data.y, test.sizeBuffer.data.x, test.sizeBuffer.data.h, test.sizeBuffer.data.x + test.sizeBuffer.data.w, test.sizeBuffer.data.h, test.foregroundColor.data);
        
        primitives::roundedRectangleRGBA(test.renderer_param, (int)test.sizeBuffer.data.x, (int)test.sizeBuffer.data.y, (int)test.sizeBuffer.data.x+(int)test.sizeBuffer.data.w, (int)test.sizeBuffer.data.y+(int)test.sizeBuffer.data.h, (int)radius*test.dpiK_param, test.backgroundColor.data.r, test.backgroundColor.data.g, test.backgroundColor.data.b, test.backgroundColor.data.a);
        test.sizeBuffer.leave();
        test.foregroundColor.leave();
        test.backgroundColor.leave();
    }));
    
    test.set_foreground_color(Colors::fromColorA(Colors::Black, 200));
    
    olamundo.events();
}
