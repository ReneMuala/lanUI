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
    
    Window win("teste", 350, 500);
    
    win.set_window_clear_color(Colors::Light_gray);
    
    Paragraph teste;
    
    Image fundo;
    
    fundo.set_size(1800, 1800);
    //fundo.set_foreground_color(Colors::Red);
    
    fundo.fromLinearGradient(Image::Horizontal, Image::GradientElement(Colors::Blue_violet, 1.0) , Image::GradientElement(Colors::Green, 1.0), win.sdlRenderer.get(), win.sdlWindow.get());
    
    win.sdlRenderer.leave();
    win.sdlWindow.leave();
    std::stringstream stream;
        stream << TextStyles::BigTitle.toStr();
        stream << "\\color:rgba(255,255,255,100) \\boldOblique O rato \\color:rgba(255,255,255,255) roeu \\color:rgba(255,255,255,100) \\newln \\color:rgba(255,255,255,255) a roupa \\color:rgba(255,255,255,100) \\newln do rei \\newln de \\color:rgba(255,255,255,255) Roma \\color:rgba(255,255,255,100)";
        stream << TextStyles::Default.toStr();
    
    teste.from_stringstream(stream, Paragraph::Wrapper::Char, 50);
    
    teste.set_alignment(Object::Alignment::Center);
    
    win.embedInZ(fundo);
    fundo.set_alignment(Object::Alignment::Center);
    fundo.embedInZ(teste);
    
    fundo.set_default_animation(5, CallbackExpr(
                                                fundo.angle.get();
                                                fundo.angle.data++;
                                                fundo.angle.leave();
                                                return true;
                                                )
                                );
    
    ola_mundo.events();
    return 0;
}
