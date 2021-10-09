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

/** View / ViewPort is main child of a window.
 Contains the elements that are goind to presented on that window.
 */
class View: public Object {
    Object some_content;
public:
    
    Window * window;
    
    /** Views start point.
     Creates the "Body" of the view,.
     */
    virtual Object& body(Window&){
        return some_content;
    };
    
    /** Creates the view without setting it as the main view of the window.
     */
    void create_silently(Window & win){
        Object();
        window = &win;
        embedInZ(body(win));
    }
    
    /** Creates the view and sets it as the main view of the window.
     */
    void create(Window & win){
        create_silently(win);
        win.set_view(*this);
    }
};

#endif /* View_hpp */
