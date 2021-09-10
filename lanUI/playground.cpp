//
//  playground.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"
/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

int main(int argc, const char * argv[]) {
    Core ola_mundo;
    
//    Window show;
//    show.set_title("Hello World!");
//
//    show.set_window_clear_color(Colors::White_smoke);
//
//    Text texto("Hello i'm a text");
//    show.set_view(texto);
//
//    texto.bold(26).set_font(Fonts::DejaVuSans).set_alignment(Object::Alignment::Center);
//
//    DrawableObject square;
//    square.set_padding({2,2,4,4});
    
    Paragraph test;
    std::stringstream stream;
    
    stream << "Lorem ipsum dolor sit amet consectetur adipisicing elit. Quibusdam, rem! Facilis, maxime ullam velit nostrum necessitatibus quaerat rem suscipit, hic ducimus deserunt perspiciatis adipisci, sequi perferendis repellat totam. Deserunt, ducimus.";
    
    test.from_stringstream(stream, Paragraph::Wrapper({Paragraph::Wrapper::Mode::Char, 20}));
    
    
//    ola_mundo.events();
    return 0;
}

