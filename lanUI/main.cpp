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
    
    DrawableObject rock[5];
    VStack myStack;
    
    MyHomeView(Window & win){
        create(win);
    }

    DrawableObject& body(Window& window) override {
        for(int i = 0 ; i < 5 ; i ++){
            rock[i].fromFile("rock.png", window.sdlRenderer.get());
            window.sdlRenderer.leave();
            rock[i].set_size(200, 200);
        }
                
        myStack.fromList((std::list<Object*>){
            &rock[0], &rock[1], &rock[2], &rock[3], &rock[4]});
//        myStack.set_size(300, 120);
//        myStack.set_size(120, 450);
        //myStack.set_primary_color(Colors::fromColorA(Colors::Black, 10));
        myStack.set_alignment(Object::Alignment::Bottom);
        
        return myStack;
    }
};


/*
 Animation -> flow animation (controller, sprites) -> destination ? (replace : stop)
 */

int main(int argc, const char * argv[]) {
    // insert code here...
    Core LanUi;
    Window window("hello world", 700, 650);
    
    auto Home = MyHomeView(window);
            
    Home.set_alignment(Object::Alignment::Center);
    
    Home.fromColorScheme();
    
    Home.fromFile("forest.png", window.sdlRenderer.get());
    window.sdlRenderer.leave();
        
    Home.set_size(600, 600);
    
    /// turn it better by disabling the border when its not necessary
    Home.set_border_color(Colors::fromColorA(Colors::White, 100));
    //Home.set_primary_color(Colors::Alice_blue);
    
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
                                  window
                                  .set_window_clear_color(Colors::Light_gray);
                                  )
                     )

    .on_focus_lost(
                   CallbackExpr(
                                window
                                .set_window_clear_color(Colors::Dim_gray);
                                )
                   )

    .on_resized(
                CallbackExpr(
                             std::cout << "window resized to:\n " << window.size.get().w;
                             window.size.leave();
                             std::cout << ", " << window.size.get().h << std::endl;
                             window.size.leave();
//                             Home.set_relative_size(1, 1, -10, -10);
//                             Home.nextInZ.get()->set_relative_size(0.5, 0.5);
//                             Home.nextInZ.data->nextInZ.get()->set_relative_size(0.5, 0.5);
//                             Home.nextInZ.data->nextInZ.leave();
//                             Home.nextInZ.leave();
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
