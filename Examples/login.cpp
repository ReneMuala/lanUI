//
//  login.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 14/01/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"
#include <regex>

#include "../lanUI/Interface/TextField/TextField.hpp"

/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

#include <iostream>

class LoginView : public View {
    
    Paragraph helper;
    TextField username = TextField("UserName");
    TextField passwd = TextField("Password");
    List mainArea;
    
public:
    
    Button continue_ = Button("Continue");
    
    LoginView(Window & win){
        create(win);
        set_size(300, 300);
        set_alignment(Center);
        
        username.textSurface.set_foreground_color(Colors::Light_gray);
        username.set_foreground_color(Colors::White);
        username.set_border_color(Colors::Gray);
        username.set_size(220, -1);
        
        passwd.textSurface.set_foreground_color(Colors::Light_gray);
        passwd.set_foreground_color(Colors::White);
        passwd.set_border_color(Colors::Gray);
        passwd.set_size(220, -1);
    }
    
    Object& body(Window & win) override {
        
        helper.from_string("%headline1 Welcome %n %bodymedium insert your username %n and password", Paragraph::Wrapper::Infty, 0).set_padding(10);
        
        username.on_empty(CallbackExpr({
            username.textSurface.italic(12);
            username.textSurface.set_foreground_color(Colors::Light_gray);
        })).on_not_empty(CallbackExpr({
            username.textSurface.regular(12);
            username.textSurface.set_foreground_color(Colors::Black);
        }))
        .set_renderer_callback(CallbackExpr({
            username.sizeBuffer.hold();
            primitives::roundedBoxColor(username.param_renderer, username.sizeBuffer.data.x, username.sizeBuffer.data.y, username.sizeBuffer.data.w, username.sizeBuffer.data.h, 15
                                        , username.foregroundColor.get());
            primitives::roundedRectangleColor(username.param_renderer, username.sizeBuffer.data.x, username.sizeBuffer.data.y, username.sizeBuffer.data.w, username.sizeBuffer.data.h, 15
                                        , username.borderColor.get());
            username.foregroundColor.leave();
            username.borderColor.leave();
            username.sizeBuffer.leave();
        }))
//        .fromColorScheme(Colors::White)
        .set_padding(10);
        
        passwd.on_empty(CallbackExpr({
            passwd.textSurface.italic(12);
            passwd.textSurface.set_foreground_color(Colors::Light_gray);
        })).on_not_empty(CallbackExpr({
            passwd.textSurface.regular(18);
            passwd.textSurface.set_foreground_color(Colors::Gray);
        }))
        .secret(true)
        .set_renderer_callback(CallbackExpr({
            passwd.sizeBuffer.hold();
            primitives::roundedBoxColor(passwd.param_renderer, passwd.sizeBuffer.data.x, passwd.sizeBuffer.data.y, passwd.sizeBuffer.data.w, passwd.sizeBuffer.data.h, 15
                                        , passwd.foregroundColor.get());
            primitives::roundedRectangleColor(passwd.param_renderer, passwd.sizeBuffer.data.x, passwd.sizeBuffer.data.y, passwd.sizeBuffer.data.w, passwd.sizeBuffer.data.h, 15
                                        , passwd.borderColor.get());
            passwd.foregroundColor.leave();
            passwd.borderColor.leave();
            passwd.sizeBuffer.leave();
        }))
//        .fromColorScheme(Colors::White)
        .set_padding(10);
        
        Text* cont_txt = (Text*)continue_.get_content();
        
        cont_txt->bold(12).set_foreground_color(Colors::Dark_gray);
        
        cont_txt->set_alignment(Center);
        
        continue_.Object::set_size(220, 20);
        
        continue_.disable_reloading();
        
        continue_.set_padding(10);
                
        mainArea.content.fromList((std::list<Object *>){&helper, &username, &passwd, &continue_});
        
        mainArea.set_alignment(Center);
        
        return mainArea;
    }
};

int main() {
//    Core test;
    
    Window mywindow("LUI", 350, 290);
        
    mywindow.set_window_clear_color(Colors::White_smoke);
    
    auto Login = LoginView(mywindow);
    
    Login.continue_.on_click(CallbackExpr({
        mywindow.close();
    }));
    
    mywindow.on_closed(CallbackExpr({
        mywindow.close();
    }));
    
    TextField field = TextField("Empty");
    
    mywindow.embedInZ(field);
    
    field.textSurface.set_font(Fonts::OpenSans);
    
    field.textSurface.bold(16);
    
//    field.secret(true, "x");
    
    field.set_alignment(Object::Center);
    
    field.fromColorScheme(Colors::White);
    
    field.set_border_color(Colors::Gray);
    
    field.on_empty(CallbackExpr({
        field.textSurface.set_foreground_color(Colors::Light_gray);
        field.textSurface.italic(12);
    }));
    
    field.on_not_empty(CallbackExpr({
        field.textSurface.set_foreground_color(Colors::Black);
        field.textSurface.regular(12);
    }));
    
    field.on_change(CallbackExpr({
        printf("The text is: %s\n", field.get_data().c_str());
    }));
//    test.events();
    return LUI::run();
}
