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
    Text Title;
    List list;
    VStack mainArea;
    
    MyHomeView(Window & win) {
        create(win);
        // set alignemt for this view
        set_alignment(Alignment::None);
        // set set size for this view
        set_size(300, 400);
        set_padding({0,0,0,0});
        // view color 
        fromColorScheme(Colors::Transparent);
    }
    
    Object& body(Window& window) override{
        Title.from_string("LanUI Changes Log ");
        Title.set_style(TextStyles::Header);

        std::stack<std::string> changes;
        changes.push("- Colors");
        changes.push("- Window");
        changes.push("- Object (LanUi base)");
        changes.push("- Image");
        changes.push("- Stack (VStack, HStack and ZStack)");
        changes.push("- List (DEMO)");
        changes.push("- View");
        changes.push("- Font");
        changes.push("- Text");
        changes.push("- Container");
        changes.push("- Spacer (DEMO)");
        changes.push("- Button (DEMO)");
        changes.push("Added:");
        changes.push(":: Version 0.1 (Stable)");
        changes.push("- DrawableObject (new render methods)");
        changes.push("- List (any.fit_content(w,h))");
        changes.push("- Window minimum size (by constructor)");
        changes.push("Improved:");
        changes.push("- LanUI Changes Program");
        changes.push("- Debug mode (LANUI_DEBUG_MODE)");
        changes.push("Added:");
        changes.push(":: Version 0.2 (Latest)");
        list.compute<Text, 200>(CallbackExpr(
                                             if(!changes.empty()){
                                                std::string src = changes.top();
                                                bool enableBold = (src.substr(0, 2) == "::");
                                                changes.pop();
                                                return enableBold ? &(new Text(src))->set_foreground_color(Colors::Black).bold(15) : &(new Text(src))->set_foreground_color(Colors::Black).regular(13);
                                            } else
                                                 return (Text*)nullptr;
                                             )
                                );
        list.fit_content(280, 340);
        //mainArea.set_padding({0,0,0,0});
        mainArea.fromList(std::list<Object *>{&Title,&list});
        //mainArea.set_border_color(Colors::Green);
        return mainArea;
    }
};

int main(int argc, const char * argv[]) {
    Core lanUI;
    
    Window myWindow = Window("LanUI Changes", 300, 400);
    
    myWindow.set_window_clear_color(Colors::Old_lace);
        
    auto home = MyHomeView(myWindow);
    
    myWindow.on_closed(CallbackExpr(
                                    lanUI.terminate();
                                    )
                       );
    
    lanUI.events();
    
    return 0;
}
