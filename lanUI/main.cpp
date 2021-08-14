//
//  main.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 17/03/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include <iostream>
#include "lanUI.hpp"

/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

class MyHomeView : public View {
public:
    
    Image linearGradient;
    HStack mainArea;
    InterativeObject interative;
    DrawableObject example;
    ZStack myList;
    Text mainText;
    Text mainText2;
    List listDemo;
    Container contTest;
    
    MyHomeView(Window & win) {
        create(win);
        
//        fromFile("lanUi.Bundle/System/Resources/forest.png", win.sdlRenderer.get());
//        win.sdlRenderer.leave();
        fromColorScheme(Colors::Dark_gray, Colors::White);
        
        set_size(340, 490);
        
        //set_alignment(Object::Alignment::Center);
    }

    DrawableObject& body(Window& window) override {
        
        contTest.set_content(mainText);
        
        mainText.from_string("Hello world");
        mainText.set_background_color(Colors::Blue_violet);
        mainText.set_padding({10,10,10,10});
        
        mainText2.from_string("Hello world");
        mainText2.set_background_color(Colors::Hot_pink);
        mainText2.set_padding({5,5,100,100});
        std::list<Object*> test;
        
        listDemo.compute<3>(
        CallbackExpr(
                     Text * text = new Text();
                     text->from_string("Hello world");
                     //test.push_back(text);
                     return text;
            )
        );
        
        Text * text = new Text();
        text->from_string("Hello wow");
        text->set_padding({1,1,1,1});
        text->set_background_color(Colors::Blue_violet);
                
        mainArea.fromList((std::list<Object*>){&listDemo});
        
        example.set_padding({0,0,0,0});
                
        //contTest.set_content(listDemo);
        
        mainArea.set_alignment(Alignment::Center);
                
        //mainArea.set_padding({0,0,0,0});
        //mainArea.set_size(300, 300);
        
        mainArea.set_border_color(Colors::Yellow);
        return mainArea;
    }
};


/*
 Animation -> flow animation (controller, sprites) -> destination ? (replace : stop)
 */

int main(int argc, const char * argv[]) {
    // insert code here...
    Core LanUi;
    Window window("Window 1", 350, 500, Window::HighDefinition);

    auto Home = MyHomeView(window);
    
    window

    .on_start(
              CallbackExpr(
                           // box[0].set_primary_color(Colors::White);
                           std::cout << "window started" << std::endl;
                           // box[0].set_relative_size(1.0/3.0, 1.0/3.0);
                           )
              )

    .on_closed(
               CallbackExpr(
                            std::cout << "window closed" << std::endl;
                            LanUi.terminate();
                            //window.hide();
                            )
               )

    .on_mouse_gained(
                     CallbackExpr(
                                  //.set_size({0, 0, 600, 400});
                                  std::cout << "mouse gained" << std::endl;
                                  )
                     )

    .on_mouse_lost(
                   CallbackExpr(
                                //.set_size({0, 0, 700, 500});
                                std::cout << "mouse lost" << std::endl;
                                )
                   )

    .on_focus_gained(
                     CallbackExpr(
//                                  window
//                                  .set_window_clear_color(Colors::Light_gray);
                                  )
                     )

    .on_focus_lost(
                   CallbackExpr(
//                                window
//                                .set_window_clear_color(Colors::Dim_gray);
                                )
                   )

    .on_resized(
                CallbackExpr(
                             std::cout << "window resized to:\n " << window.size.get().w;
                             window.size.leave();
                             std::cout << ", " << window.size.get().h << std::endl;
                             window.size.leave();
                             )
                )

    .on_key_down(
                 CallbackExpr(
                              std::cout << "key down" << std::endl;
                              // box[0].set_primary_color(Colors::fromRGA(255, 255, 255, 127));
                              )
                 )

    .on_key_up(
               CallbackExpr(
                            std::cout << "key up" << std::endl;
                            // box[0].set_primary_color(Colors::White);
                            )
               )

    .on_mouse_button_down(
                          CallbackExpr(
                                       //std::cout << "mouse down" << std::endl;
                                       // box[0].primaryColor.set(Colors::White_smoke);
                                       )
                          )
    .on_mouse_button_up(
                        CallbackExpr(
                                     //std::cout << "mouse up" << std::endl;
                                     // box[0].primaryColor.set(Colors::White);
                                     )
                        )
    .on_hidden(
               CallbackExpr(
                            std::cout << "hidden" << std::endl;
                            )
               )

    .on_shown(
              CallbackExpr(
                           std::cout << "shown" << std::endl;
                           )
            )

    .on_minimized(
                  CallbackExpr(
                               std::cout << "minimized" << std::endl;
                               )
                  )

    .on_maximized(
                  CallbackExpr(
                               std::cout << "maximized" << std::endl;
                               //window.set_window_clear_color(Colors::Pink);
                               )
                  )

    .set_window_clear_color(Colors::Light_gray)
    
    .set_title("LanUi Demo");

    //.embedInZ(box[0]);

    Core::events();
    
    std::cout << "Hello, World!\n";
    return 0;
}
