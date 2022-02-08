//
//  lanUI.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef lanUI_h
#define lanUI_h

// A framework to create consistent user interfaces in C++

#include "Interface/Theme/Theme.hpp"
#include "Interface/Object/Object.hpp"
#include "Interface/Color/Color.hpp"
#include "Interface/Window/Window.hpp"
#include "Interface/View/View.hpp"
#include "Interface/Stack/Stack.hpp"
#include "Interface/List/List.hpp"
#include "Interface/Text/Text.hpp"
#include "Interface/Paragraph/Paragraph.hpp"
#include "Interface/Container/Container.hpp"
#include "Interface/Spacer/Spacer.hpp"
#include "Interface/Image/Image.hpp"
#include "Interface/Button/Button.hpp"
#include "Project/Project.hpp"

#include "Utilities/PathResolver.hpp"

namespace LUI
{
void clear_cache(){
    Object::clear_surfaces_cache();
}
void on_quit(Window::VoidCallback callback){
    WindowManager::on_quit(callback);
}
int run(){
    WindowManager::run_global_events_handler();
    return 0;
}
}

#endif /* lanUI_h */
