//
//  aves.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/09/21.
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
    
    VStack mainStack;
    Text title;
    Container titleContainer;
    Image bird;
    Container birdContainer;
    Paragraph description;
    Container descriptionContainer;
    
    MyHomeView(Window & win){
        create(win);
        set_size(600, 400);
        set_alignment(Center);
        fromColorScheme();
    }
    
    void createTitle(Window& window){
        title.from_string("Aves");
        title.set_alignment(Center);
        title.bold(36);
        title.set_foreground_color(Colors::Orange);
        titleContainer.set_content(title);
        titleContainer.set_size(600, 40);
        titleContainer.disable_reloading();
    }
    
    void createBird(Window& window){
        bird.fromFile("lanUI.Bundle/System/Resources/BlueBird.png", window.sdlRenderer.get());
        window.sdlRenderer.leave();
        bird.set_size(256, 256);
        bird.set_alignment(Center);
        birdContainer.set_content(bird);
        birdContainer.set_size(600, 256);
        birdContainer.disable_reloading();
    }
    
    void createDescription(Window& window){
        std::stringstream stream;
        
        stream << "\\color:rgb(0,0,0,0) \\size:14 Uma aplicação feita usando o \\color:rgb(0,50,255) framework LANUI \\regular \\color:rgb(0,0,0) em C++. \\newln As informações contidas neste aplicativo foram coletadas no site da \\bold Wikipédia. \\regular";
        
        description.from_stringstream(stream, Paragraph::Wrapper::Char, 60);
        description.set_alignment(Center);
        descriptionContainer.set_content(description);
        descriptionContainer.set_size(600, 80);
        descriptionContainer.disable_reloading();
    }
    
    DrawableObject& body(Window& window) override {
        createTitle(window);
        createBird(window);
        createDescription(window);
        mainStack.fromList(std::list<Object *>{&titleContainer, &birdContainer, &descriptionContainer});
        mainStack.set_alignment(Center);
        return mainStack;
    }
};

/*
 Animation -> flow animation (controller, sprites) -> destination ? (replace : stop)
 */

int main(int argc, const char * argv[]) {
    // insert code here...
    Core lanUI;
    Window window("hello world", 600, 500, Window::HighDefinition);
    
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
    
    .set_title("lanUI Demo")
    
    .set_window_clear_color(Colors::White);
    
    //.embedInZ(box[0]);
    
    Core::events();
    
    std::cout << "Hello, World!\n";
    return 0;
}

