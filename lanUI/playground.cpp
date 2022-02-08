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

using namespace std;

class MainView : public View {
    Paragraph question;
    List answers;
    VStack mainstask;
    Text * count;
    int counter=0;
public:
    MainView(Window & win){
        create(win).set_alignment(Center);
        set_size(600, 400);
    }
    
    Object& body(Window & win) override {
        question.from_string("%Size:12 %Italic Olá Müendo %n [ %[count] 0 ]");
        if((count = question["count"])){
            count->set_default_animation(0, CallbackExpr({
                counter++;
                count->from_string(std::to_string(counter));
                return true;
            }));
        }
        mainstask.fromList((std::list<Object *>){&question});
        mainstask.set_alignment(Center);
        return mainstask;
    }
};

#include <queue>

int main() {
//   Core program;
    
    
    Window window("LUI") ,
    window2("LUI2"),
    window3("LUI3");
                           
//   ;lpplunm ,
//
//    window.set_window_clear_color(Colors::White_smoke);
//
//    Text text("Hello");
//
//    window.embedInZ(text.extraBold(36).set_alignment(Object::Top));
//
    MainView
    view(window
         .set_window_clear_color(Colors::White_smoke)
         .on_resized(CallbackExpr({
             view.set_relative_size(1.0, 1.0);
    }))
         .on_closed(CallbackExpr({
        window.close();
    }))),
    
    view2(window2
         .set_window_clear_color(Colors::White_smoke)
         .on_resized(CallbackExpr({
             view2.set_relative_size(1.0, 1.0);
    }))
          .on_closed(CallbackExpr({
         window2.close();
     }))),
    view3(window3
         .set_window_clear_color(Colors::White_smoke)
         .on_resized(CallbackExpr({
             view3.set_relative_size(1.0, 1.0);
         }))
          .on_closed(CallbackExpr({
         window3.close();
     })));
//    program.events();
    LUI::on_quit(CallbackExpr({
        window.close_all();
    }));
    
    
    return LUI::run();
}
