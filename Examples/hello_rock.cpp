//
//  hello_rock.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifdef USING_LANUI_CMAKE_BUILDER
// Use the generated template headers
#include "../Template/lanUI/lanUI.hpp"
#else
#include "../lanUI/lanUI.hpp"
#endif

int main() {
    Core lanUI;
    Window myWindow("Hello rock", 400, 300);
    
    // uma rocha
    DrawableObject rock;
    rock.fromFile("lanUI.Bundle/System/Resources/rock.png", myWindow.sdlRenderer.get());
    rock.set_size(300, 300);
    rock.set_alignment(Object::Alignment::Center);
    myWindow.sdlRenderer.leave();
    
    // mostrando a rocha
    myWindow.embedInZ(rock);
    
    Core::events();
}
