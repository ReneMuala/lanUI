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
    ZStack mainArea;
    Image background;
    InterativeObject interative;
    DrawableObject example[5];
    ZStack myList;
    List listDemo;
    VStack mainStack;
    Text title[2];
    Text titleDescrition;
    Container titleDescritionArea;
    VStack titleStack;
    Container titleArea;
    HStack actionArea;
    List codeList;
    
    MyHomeView(Window & win) {
        create(win);
        
//        fromFile("lanUI.Bundle/System/Resources/forest.png", win.sdlRenderer.get());
//        win.sdlRenderer.leave();
        fromColorScheme(Colors::Transparent, Colors::Transparent);
        
        set_size(1300, 850);
        set_padding({0,0,0,0});
        set_alignment(Object::Alignment::Center);
        set_border_color(Colors::Blue);
    }

    void createBackgound(Window& window){
        background.set_size(1200, 650);
        background.set_alignment(Alignment::Center);
        
        background.fromLinearGradient(Image::Vertical, Image::GradientElement(Colors::fromRGBA(255-200, 100-100, 255-200), 1.0), Image::GradientElement(Colors::fromRGBA(31/2, 68/2, 128/2), 1.0), window.sdlRenderer.get(), window.sdlWindow.get(), 1,2);
        window.sdlRenderer.leave();
        window.sdlWindow.leave();
    }
    
    void createHeader(){
        title[0].from_string("Galeria").set_foreground_color(Colors::Orange).bold(65);
        title[1].from_string("LanUI").set_font(CustomFonts::Lobster).regular(65).set_foreground_color(Colors::Dark_violet).set_scrollingFactor(Object::ScrollingFactor({70,-40}));
        titleDescrition.from_string("LanUI é uma biblioteca para criar interfaces gráficas em Cpp").regular(18).set_foreground_color(Colors::fromColorA(Colors::White, 100));
        titleDescritionArea.set_content(titleDescrition);
        titleDescritionArea.set_size(1200, 20);
        titleDescritionArea.disable_reloading();
        titleDescrition.set_alignment(Alignment::Center);
        titleStack.fromList(std::list<Object *>{&title[0], &title[1]});
        titleStack.set_alignment(Alignment::Top);
        titleArea.set_content(titleStack);
        titleArea.set_size(1200, 120);
        titleArea.disable_reloading();
    }
    
    DrawableObject& body(Window& window) override {
        window.disable_resizing(true);
        createBackgound(window);
        createHeader();
        mainStack.fromList(std::list<Object *>{&titleArea,
            &titleDescritionArea,
            &actionArea
            //&example[0],&example[1],&example[2],&example[3]
        });
        codeList.compute<DrawableObject, 100>(CallbackExpr(
                                                          return &(DrawableObject&)(new DrawableObject)->set_foreground_color(Colors::fromColorA(Colors::White, 50)).set_size(580, 500).set_padding({10,10,10,10});
                                                          )
                                             );
        //actionArea.set_foreground_color(Colors::fromColorA(Colors::White, 50));
        actionArea.set_size(1200, 550);
        actionArea.fromList(std::list<Object *>{&codeList});
        codeList.fit_content(600, 500);
        mainArea.fromList(std::list<Object *>{&background, &mainStack});
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
    Window window("Window 1", 1200, 650, Window::HighDefinition);

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

    .set_window_clear_color(Colors::fromRGBA(25, 10, 25, 0.5))

    .set_title("lanUI Demo");

    //.embedInZ(box[0]);

    Core::events();
    
    std::cout << "Hello, World!\n";
    return 0;
}
