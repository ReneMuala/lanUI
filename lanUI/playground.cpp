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

class MainView : public View {
    Paragraph question;
    List answers;
    VStack mainstask;
public:
    MainView(Window & win){
        create(win).set_alignment(Center);
    }
    
    Object& body(Window & win) override {
        question.from_string("%Size:16 %Bold O que é um resumo?");
        mainstask.fromList((std::list<Object *>){&question});
        return mainstask;
    }
};

int main() {
    
    std::cout << (PathResolver::resolve("Fonts:test\\image.png")) << std::endl;
    
    return 0;
    Core program;
    
    Window window;
    
    MainView
    view(window
         .set_window_clear_color(Colors::White_smoke)
         .on_resized(CallbackExpr({view.set_relative_size(0.65, 0.9);})));
    program.events();
}
