//
//  View.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef View_hpp
#define View_hpp

#include "../Object/Object.hpp"
#include "../Window/Window.hpp"

/// Base to create Views
class View: public DrawableObject {
    Window * window;
    DrawableObject some_content;
public:
    virtual DrawableObject& body(Window&){
        return some_content;
    };
    
    void create_silently(Window & win){
        DrawableObject();
        win.set_view(*this);
        window = &win;
    }
    
    void create(Window & win){
        create_silently(win);
        embedInZ(body(win));
    }
};

#endif /* View_hpp */
