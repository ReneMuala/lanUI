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


class MyView : public View {
    
    
public:
    
    Paragraph myText;
    Container myTextContainer = Container(myText);
    VStack MainStack;
    Container MainStackContainer = Container(MainStack);
    Text * test[3];
    Object topBar;
    Button endp = Button("Sair");
    
    MyView(Window & win){
        set_alignment(Top);
        fromColorScheme(Colors::White);
        create(win);
    }
    
    Object& body(Window & win) {
        
        MainStack.fromList(std::list<Object *>{&myTextContainer, &endp});
        
        MainStack.set_alignment(Center);
                
        std::stringstream myTextStream;
        
        myTextStream << "%Display  %[0] Big Things %n %Captioncaps %italic Came from %bold %[1] big %bolditalic ideas";
        
        myText.from_stringstream(myTextStream);
        
        if((test[0] = myText["0"])){
            test[0]->set_foreground_color(Colors::Red);
            test[0]->set_default_animation(0, CallbackExpr({
                const Color color = Color(0, 0, 0, 0);
                static float a;
                static bool inc = true;
                if(a > 254)
                    inc = false;
                else if (a < 1)
                    inc = true;
                test[0]->set_foreground_color(color.from_a(a));
                a += inc ? 0.8 : -0.8;
                return true;
            }));
        }

//        if((test[1] = myText["1"])){
//            test[1]->set_foreground_color(Colors::Red);
//            test[1]->set_default_animation(0, CallbackExpr({
//                const Color color = Color(0, 0, 0);
//                static float a;
//                static bool inc = true;
//                if(a > 254)
//                    inc = false;
//                else if (a < 1)
//                    inc = true;
//                test[1]->set_foreground_color(color.from_a(a));
//                a += inc ? 0.8 : -0.8;
//                return true;
//            }));
//        }
        topBar.fromColorScheme(Colors::Deep_sky_blue);
        topBar.embedInY(MainStackContainer);
        topBar.set_size(-1, 15);
        
        MainStackContainer.disable_reloading();
        myText.set_alignment(Left);
//        myTextContainer.set_size(305, 100);
//        myTextContainer.disable_reloading();
        
        return topBar;
    }
};



int main() {
    Core program;
    
    Window mywindow("LUI", 350, 200);
        
    mywindow.set_window_clear_color(Colors::White);
    
    /*
    MyView teste (mywindow);
        
    mywindow.on_resized(CallbackExpr({
        teste.set_relative_size(1.f, 1.f);
        teste.topBar.set_relative_size(1.f, -1.f);
        teste.MainStackContainer.set_relative_size(1.f, 0.99f);
    }));
    
    teste.myText.compose(mywindow.sdlRenderer.get(), mywindow.DPIConstant.get());
    teste.myText.export_composition_as_PNG(mywindow.sdlRenderer.data, "teste.png");
    mywindow.sdlRenderer.leave();
    mywindow.DPIConstant.leave();
//    teste.myText.set_render_mode(Object::RenderMode::CompositionMode);
    
//    mywindow.on_mouse_button_down(CallbackExpr({
//
////        teste.myText.set_render_mode(Object::RenderMode::CompositionMode);
//    }));

     teste.endp.on_click(CallbackExpr({
         program.terminate();
     }));
     
     
     */
    mywindow.on_closed(CallbackExpr({
        program.terminate();
    }));
    
    TextField field = TextField("Empty");
    
    mywindow.embedInZ(field);
    
    field.textSurface.set_font(Fonts::OpenSans);
    
    field.textSurface.bold(10);
    
    field.hide(true);
    
    field.textSurface.set_foreground_color(Colors::Black);
        
    field.set_alignment(Object::Center);
    
    field.fromColorScheme(Colors::White);
    
    field.set_border_color(Colors::Blue);
    
    program.events();
}

