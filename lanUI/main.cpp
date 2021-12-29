//
//  main.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 17/03/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"
#include <vector>
#include <stack>
#include <iostream>

/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

class MyHomeView : public View {
public:
    Text title;
    Container titleContainer = Container(title);
    List list;
    VStack mainArea;
    Paragraph newsText;
    Spacer spacer;
    Text * alphaText;
    
    MyHomeView(Window & win) {
        create(win);
        // set alignemt for this view
        set_alignment(Alignment::None);
        // set set size for this view
        set_padding({0,0,0,0});
        // view color 
        fromColorScheme(Colors::Transparent);
    }
    
    Object& body(Window& window) override{
        std::stringstream stream;
        
        stream << " %size:28 %bold Version 0.3 ( %ns %[alpha] Alpha %ns ) %n %size:14 %medium "
        " %color:RGB(200,200,200) %bk %n ADDED %n %color:rgb(0,0,0) %r"
        " - %font:Opensans OpenSans fonts %font:default %n"
        " - %font:Worksans WorkSans fonts %font:default %n"
        " - TextField %n"
        " - lanUTF (support for UTF8) %n"
        " - Paragraphs %n"
        " - DrawableObject::set_renderer_callback() %n"
        " - Core/Graphics (primitives::) from SDL2_gfx %n"
        " - CSS padding definition styles %n"
        " - Static renderization (composition) %n"
        " - InApp screenshot (via composition) %n"
        
        " %color:RGB(200,200,200) %bk %n IMPROVED %n %color:rgb(0,0,0) %r"
        " - LanUI Changes Program %n"
        " - Memory management %n"
        " - Renderer clips %n"
        " - padding is now { " << Colors::Dark_violet.toStr();
        stream << " %ns 0,0,0,0 %ns %color:RGB(0,0,0) } by default %n"
        " - debug mode is better %n"
        " - improved linear gradient %n"
        " - empty text bug fixed %n"
        " - content alignment methods %n"
        " - Text renderization methods %n";
        
        stream << " %size:28 %bold Version 0.2 (stable) %n %size:14 %medium "
        " %color:RGB(200,200,200) %bk %n ADDED %n %color:rgb(0,0,0) %r"
        " - Debug mode (LANUI_DEBUG_MODE) %n"
        " - LanUI Changes Program %n"
        " %color:RGB(200,200,200) %bk %n IMPROVED %n %color:rgb(0,0,0) %r"
        " - Window minimum size (by constructor) %n"
        " - List %bold \" %ns any.fit_content(w,h) %ns \" %regular %n"
        " - Font %n"
        " - View %n"
        " - List %color:RGB(200,200,200) (DEMO) %color:RGB(0,0,0) %n"
        " - DrawableObject (new render methods) %n";
        
        stream << " %size:28 %bold Version 0.1 (stable) %n %size:14 %medium "
        " %color:RGB(200,200,200) %bk %n ADDED %n %color:rgb(0,0,0) %r"
        " - Buttom %color:RGB(200,200,200) (DEMO) %color:RGB(0,0,0) %n"
        " - Spacer %color:RGB(200,200,200) (DEMO) %color:RGB(0,0,0) %n"
        " - Container %n"
        " - Text %n"
        " - Font %n"
        " - View %n"
        " - List %color:RGB(200,200,200) (DEMO) %color:RGB(0,0,0) %n"
        " - Stack (VStack, HStack and ZStack) %n"
        " - Image %n"
        " - Object %n"
        " - Window %n"
        " - Colors %n";
        
        stream << " %size:28 %black ThanKYou %n %size:14 %medium " << Lorem;
        
        newsText.from_stringstream(stream, Paragraph::Wrapper::Char, 40);
        
        
        if((alphaText = newsText["alpha"])){
            alphaText->set_default_animation(0, CallbackExpr({
                static Uint8 alphaChannel = 200;
                
                if(alphaText->_has_focus(param_dpiK)){
                    alphaText->set_foreground_color(Colors::Black.from_a(alphaChannel++));
                }
                    
                return true;
            }));
        }
        
        title.from_string("What's new?");
        title.set_style(TextStyles::Headline1);
        title.black(48);
        title.set_foreground_color(Color(0,160,255));
        title.set_alignment(Center);
        titleContainer.set_padding({20,0});
        list.content.fromList(std::list<Object *>{&newsText});
        //mainArea.set_padding({0,0,0,0});
        mainArea.fromList(std::list<Object *>{&titleContainer,&list});
        mainArea.set_alignment(Top);
        //mainArea.set_border_color(Colors::Green);
        return mainArea;
    }
};

int main(int argc, const char * argv[]) {
    Core lanUI;
    
    Window myWindow = Window("LanUI Changes", 600, 650, Window::HighDefinition);
    
    myWindow.set_window_clear_color(Colors::White);
        
    auto home = MyHomeView(myWindow);
    
    home.set_alignment(Object::Top);
    
    myWindow.on_resized(CallbackExpr({
        myWindow.size.hold();
        home.list.fit_content(350, myWindow.size.data.h-100);
        home.set_size(350, myWindow.size.data.h);
        home.titleContainer.set_size(350, -1);
        myWindow.size.leave();
    }));
    
    myWindow.on_closed(CallbackExpr(
                                    lanUI.terminate();
                                    )
                       );
    
    lanUI.events();
    
    return 0;
}
