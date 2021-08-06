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
    ZStack mainArea;
    Text mainText;
    
    MyHomeView(Window & win){
        create(win);
        
//        fromFile("lanUi.Bundle/System/Resources/forest.png", win.sdlRenderer.get());
//        win.sdlRenderer.leave();
        fromColorScheme(Colors::Transparent, Colors::Transparent);
        
        set_size(460, 460);
        
        set_alignment(Object::Alignment::Center);
        
    }

    DrawableObject& body(Window& window) override {
        linearGradient
        .set_alignment(Alignment::Top)
        .set_size(250, 200);
        
        linearGradient.fromRadialGradient({0, 0}, 200,Image::GradientElement({Colors::Gray, 1.0}), Image::GradientElement({Colors::Black, 1.0}), window.sdlRenderer.get(), window.sdlWindow.get());
        
        
        
        linearGradient.set_angle(180);
        window.sdlRenderer.leave();
        window.sdlWindow.leave();
        mainText.set_font(CustomFonts::Lobster);
        mainText.bold(64);
        mainText.set_primary_color(Colors::White);
        
        mainText.from_string("omada", window.sdlRenderer.get());
        window.sdlRenderer.leave();
        mainText.set_alignment(Alignment::Center);
        
        mainArea.fromList((std::list<Object *>){&linearGradient, &mainText});
        
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
                                       std::cout << "mouse down" << std::endl;
                                       // box[0].primaryColor.set(Colors::White_smoke);
                                       )
                          )
    .on_mouse_button_up(
                        CallbackExpr(
                                     std::cout << "mouse up" << std::endl;
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

    .set_title("LanUi Demo");

    //.embedInZ(box[0]);

    Core::events();
    
    std::cout << "Hello, World!\n";
    return 0;
}
