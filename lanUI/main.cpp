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
    
    Image mainGradient;
    VStack mainList;
    ZStack mainArea;
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
        fromColorScheme(Colors::Transparent, Colors::Transparent);
        
        //set_border_color(Colors::Gray);
        
        set_size(350, 500);
        
        set_padding({0,0,0,0});
        
        set_alignment(Object::Alignment::Center);
    }

    DrawableObject& body(Window& window) override {
        
        mainGradient.set_size(350, 500);
        mainGradient.set_padding({0,0,0,0});
        mainGradient.fromRadialGradient({350, 500}, 1400,Image::GradientElement({Colors::Blue_violet, 1.0}), Image::GradientElement({Colors::Blue, 1.0}), window.sdlRenderer.get(), window.sdlWindow.get());
        window.sdlRenderer.leave();
        window.sdlWindow.leave();
        
        mainText.from_string("LanUi List");
        mainText.set_padding({10,10,5,5});
        mainText.set_foreground_color(Colors::White);
        mainText.bold(26);
        
        mainText2.from_string("LanUi Cpp Library 0.1");
        mainText2.set_foreground_color(Colors::fromColorA(Colors::White, 100));
        mainText2.set_padding({0,0,5,0});
        mainText2.bold(13);
        
        listDemo.compute<Text, 90>(
        CallbackExpr(
                     return &(new Text("Item " + std::to_string(listDemo.index+1) + " of " + std::to_string(listDemo.count)))->regular(18).set_foreground_color(Colors::White).set_padding({0,0,0,0,});
                     //.set_primary_color(Colors::Indian_red);
            )
        );
        
        listDemo.content.set_size(150, 200);
        listDemo.content.disable_reloading();
        listDemo.set_padding({0,0,5,0});
        
        mainList.fromList((std::list<Object*>){&mainText, &listDemo, &mainText2});
        mainList.set_alignment(Alignment::Center);
        mainArea.fromList((std::list<Object*>){&mainGradient, &mainList});
        mainArea.set_alignment(Alignment::Center);
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
