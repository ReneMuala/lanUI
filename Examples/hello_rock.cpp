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
    Window myWindow("Hello rock", 400, 300);
    
    // uma rocha | a rock
    Object rock;
    rock.fromFile("lanUI.Bundle/System/Resources/rock.png", myWindow.sdlRenderer.get());
    rock.set_size(300, 300);
    rock.set_alignment(Object::Alignment::Center);
    myWindow.sdlRenderer.leave();
    
    // mostrando a rocha | showing the rock
    myWindow.embedInZ(rock);
    
    // fechando o programa quando o usuario fecha a janela
    // closing the window when user closes the window
    myWindow.on_closed(CallbackExpr(
                                    myWindow.close();
                                    )
                       );
    
    return LUI::run();
}
