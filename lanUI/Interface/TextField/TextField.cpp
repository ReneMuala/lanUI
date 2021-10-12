//
//  TextField.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "TextField.hpp"
#include "../../Core/Core.hpp"

TextField::TextField(Semaphore<std::string>& source, const std::string placeholder): activated(false), source(source), placeholder(placeholder){
    Core::log(Core::Warning, "TextField isn't stable yet.");
    drawMode.set(ColorSchemeMode);
    disable_reloading();
    surface.set_alignment(Left);
    
    on_click(CallbackExpr({
        activated = true;
        auto newColor = borderColor.get().from_a(255);
        borderColor.leave();
        set_border_color(newColor);
        printf("::Actvated");
    }));
    
    on_focus_lost(CallbackExpr({
        if(activated){
            activated = false;
            auto newColor = borderColor.get().from_a(50);
            borderColor.leave();
            set_border_color(newColor);
            printf("::Desactvated");
        }
    }));
    
    set_content(surface);
}

TextField& TextField::set_size(const float w, const float h){
    InterativeObject::set_size(w, h);
    return (*this);
}

void TextField::_compile(Renderer * renderer, const float dpiK){
    if(!wasCompiled.get()){
        wasCompiled.data = true;
        wasCompiled.leave();
        sourceStream.clear();
        source.hold();
        sourceStream << "\\regular \\size:12 \\color:rgb(0,0,0) " << ((source.data.empty()) ? placeholder : source.data);
        source.leave();
        surface.from_stringstream(sourceStream);
    } else
        wasCompiled.leave();
    _compile_embedded(renderer, dpiK);
}
