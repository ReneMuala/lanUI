//
//  click_gradient.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 11/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include <iostream>
#ifdef USING_LANUI_CMAKE_BUILDER
// Use the generated template headers
#include "../Template/lanUI/lanUI.hpp"
#else
#include "../lanUI/lanUI.hpp"
#endif


/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

class MyHomeView : public View {
public:
    
    Image gradient;
    VStack mainArea;
    InterativeObject interative;
    DrawableObject example;
    ZStack myList;
    Text mainText;
    List listDemo;
    
    MyHomeView(Window & win) {
        create(win);
        
//        fromFile("lanUI.Bundle/System/Resources/forest.png", win.sdlRenderer.get());
//        win.sdlRenderer.leave();
        fromColorScheme(Colors::Transparent, Colors::Transparent);
        
        set_size(460, 460);
        
        set_alignment(Object::Alignment::Center);
        
    }

    DrawableObject& body(Window& window) override {
//        interative
//        .on_click(
//                 CallbackExpr(
//                              printf("Hello world");
//                              ));
        
        gradient
        .set_alignment(Alignment::Top)
        .set_size(200, 200);
        
        gradient.fromRadialGradient({0, 0}, 400,Image::GradientElement({Colors::Hot_pink, 1.0}), Image::GradientElement({Colors::Orange, 1.0}), window.sdlRenderer.get(), window.sdlWindow.get());
        window.sdlRenderer.leave();
        window.sdlWindow.leave();
        //mainText.set_font(CustomFonts::Lobster);
        // TODO: FIX INTERACTIVE DATA SIZE ISSUES
        mainText.extraLight(18);
        
        mainText.set_foreground_color(Colors::White);
        
        mainText.from_string(" Click ", window.sdlRenderer.data);
        
        mainText.set_background_color(Colors::Dark_violet);
        
        //gradient.set_angle(45);
        
        window.sdlRenderer.leave();
        mainText.set_alignment(Alignment::Center);
        
        interative.set_content(mainText);
                
        example.set_alignment(Alignment::Center);
        
        interative.set_alignment(Alignment::Center);
        
        interative.on_click(
                            CallbackExpr(
                                         printf("Hello world\n");
                                         window.sdlWindow.hold();
                                         window.sdlRenderer.hold();
                                         static int count = 0 ;
                                         count += 1;
//                                         mainText.from_string(std::to_string(count), window.sdlRenderer.data);
                                         gradient.fromRadialGradient({0, 0}, 400,Image::GradientElement({(count % 2==0) ? Colors::Hot_pink : Colors::Blue_violet, 1.0}), Image::GradientElement({Colors::Orange, 1.0}), window.sdlRenderer.data, window.sdlWindow.data);
                                         window.sdlWindow.leave();
                                         window.sdlRenderer.leave();
                                         )
                            );
        
        gradient.set_default_animation(0, CallbackExpr(
                                                             static bool toIncrease(true);
                                                             static float size = 180;
                                                             size = (toIncrease) ? size+.1: size-.1;


//                                                             gradient.angle.hold();
//                                                             gradient.angle.data += 1;
//                                                             if(gradient.angle.data > 360)
//                                                             gradient.angle = 0;
//                                                             gradient.angle.leave();

                                                            if(size < 180 || size >= 200)
                                                             toIncrease = !toIncrease;
                                                             //printf("%f\n", size);
                                                             gradient.size.hold();
                                                             gradient.size.data.w = size;
                                                             gradient.size.data.h = size;
                                                             gradient.size.leave();


                                                             //gradient.set_size(size, size);
                                                             mainArea.reload();
                                                            return true;
                                                             )
                                             );
        
//        mainText.set_default_animation(0, CallbackExpr(
//                                                          mainText.angle.hold();
//                                                          mainText.angle.data += 1;
//                                                          if(mainText.angle.data > 360)
//                                                          mainText.angle = 0;
//                                                          //printf("Hello world %lf\n", mainText.angle.data);
//                                                          mainText.angle.leave();
//                                                          return true;
//                                                          ));
        
        myList.set_alignment(Alignment::Center);
        
        mainArea.fromList((std::list<Object *>){&gradient, &interative});
        
        mainArea.set_padding({0,0,0,0});
        gradient.set_padding({0,0,0,0});
        
        mainArea.set_alignment(Alignment::Center);
                
        return mainArea;
    }
};


/*
 Animation -> flow animation (controller, sprites) -> destination ? (replace : stop)
 */

int main(int argc, const char * argv[]) {
    // insert code here...
    Core lanUI;
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
                            lanUI.terminate();
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
    
    .set_title("lanUI Demo");

    //.embedInZ(box[0]);

    Core::events();
    
    std::cout << "Hello, World!\n";
    return 0;
}
