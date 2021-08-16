//
//  main.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 17/03/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"

/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

class MyHomeView : public View {
public:
    Text message;
    MyHomeView(Window & win) {
        create(win);
        // set alignemt for this view
        set_alignment(Alignment::Center);
        // set set size for this view
        set_size(200, 300);
    }
    DrawableObject& body(Window& window) override{
        // the message
        message.from_string("Hello lanUI");
        // message color
        message.set_foreground_color(Colors::Black);
        // set alignemt for message
        message.set_alignment(Alignment::Center);
        // return message as the main content for this view
        return message;
    }
};

int main(int argc, const char * argv[]) {
    Core LanUI;
    
    Window myWindow = Window("hello lanUI", 200, 300);
    
    auto home = MyHomeView(myWindow);
    
    LanUI.events();
    return 0;
}
