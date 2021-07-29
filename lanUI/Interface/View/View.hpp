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
public:

    Window * window;
    DrawableObject content;
    
    virtual DrawableObject& body(Window&){
        return content;
    };
        
    void create(Window & win){
        DrawableObject();
        win.set_view(*this);
        window = &win;
        embedInZ(body(win));
    }
};

#endif /* View_hpp */
