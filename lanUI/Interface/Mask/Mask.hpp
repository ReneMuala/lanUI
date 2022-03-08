//
//  Mask.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 19/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Mask_hpp
#define Mask_hpp

#include <stdio.h>
#include "../../CairoSDL/cairosdl.h"
#include "../../Types.hpp"

class Mask {
protected:
    CairoContext* context;
    double w, h, scale_param, lowerSide, greaterSide;
public:
    Mask() {
        set_params(nullptr, 1,1,0,0,0);
    };
    
    void set_params(CairoContext* context, const double w, const double h, const double scale_param, const double lowerSide, const double greaterSide){
        this->context = context;
        this->w = w > 0 ? w : 1;
        this->h = h > 0 ? h : 1;
        this->scale_param = scale_param;
        this->lowerSide = lowerSide;
        this->greaterSide = greaterSide;
    };
    /**
     Callback to apply cairo mask to an object canvas compilation.
     This funcion shoud only create the path to a cairo context (by using patterns ou any other cairo methods...) and fill or stroke if necessary.
     */
    virtual void apply() {
        // example
        cairo_rectangle(context, 0, 0, 1, 1);
        cairo_fill(context);
    };
};

#endif /* Mask_hpp */
