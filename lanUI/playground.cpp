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

int main(){
    
    Core olamundo;
    
    Window janela("hello", 400,500);
    
    janela.set_window_clear_color(Colors::White_smoke);
    
    Semaphore<std::string> fieldData;

    auto field = TextField(fieldData, "Username");

    field.set_foreground_color(Colors::White);

    field.set_border_color(Colors::Blue.from_a(100));

    field.set_alignment(Object::Center);

    field.set_size(200, 25);
    
    field.set_scrollingFactor({-20,-15});
    
    field.set_input_style("\\r \\size:13 \\color:rgb(0,0,0)");
    
    janela.embedInZ(field);
    
    olamundo.events();
    
    Paragraph test;
    
    std::stringstream stream;
    
    stream << "\\regular \\size:12 \\color:rgb(255,0,0) " << "Hello world";
    
    test.from_stringstream(stream);
    
    janela.embedInZ(test);

    olamundo.events();
}
