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
    Core exemplo;
    
    Window janela;
    
    janela.set_title("LanUI");
    
    janela.set_window_clear_color(Colors::White);
    
    Image dino;
    
    dino.fromFile("lanUI.Bundle/System/Resources/Dino.png", janela.sdlRenderer.get());
    janela.sdlRenderer.leave();
    
    dino.set_size(300, 300);
    
    Text legenda;
    
    legenda.from_string("Velociraptor");
    
    List lista;
    
    lista.content.fromList(std::list<Object *>{&dino, &legenda});
    
    lista.set_alignment(Object::Center);
    
    janela.embedInZ(lista);
    
    legenda.bold(18);
    
    exemplo.events();
}
