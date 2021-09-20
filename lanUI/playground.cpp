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

int main(int argc, const char * argv[]) {
    Core ola_mundo;
    
    Window win;
    
    win.set_window_clear_color(Colors::Light_gray);
    
    Paragraph teste;
    
    std::stringstream stream;
    
    stream << TextStyles::BigTitle.toStr();
    stream << "Lorem ipsum \\newln ";
    stream << TextStyles::Default.toStr();
    stream << "Lorem ipsum dolor sit amet consectetur adipisicing elit. Ullam, magni. Ad culpa dolores id, aspernatur soluta quidem distinctio architecto tempore magni eos odio autem a quibusdam! Delectus cum nam iusto.";
    
    teste.from_stringstream(stream, Paragraph::Wrapper::Char, 20);
    
    teste.set_alignment(Object::Alignment::Center);
    
    win.embedInZ(teste);
    
    ola_mundo.events();
    return 0;
}
