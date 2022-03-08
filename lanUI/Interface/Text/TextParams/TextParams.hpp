//
//  TextParams.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 05/03/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef TextParams_hpp
#define TextParams_hpp

#include <stdio.h>
#include "../../../Types.hpp"
#include "../../../CairoSDL/Cairo.hpp"
#include "../../Font/Font.hpp"

/**
 (Random Access) text parameters calculator.
 */
class TextParams {
protected:
    CairoContext * cr;
    CairoSurface * surface;
    CairoFontFace * crface;
public:
    
    TextParams();
    
    void compute_params( CairoFontFace * ,CairoTextExtents &, CairoFontExtents &, const double size, const char * source);
    
    ~TextParams();
    
};

#endif /* TextParams_hpp */
