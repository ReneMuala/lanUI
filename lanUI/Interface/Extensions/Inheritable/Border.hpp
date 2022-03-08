//
//  Border.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 20/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Border_h
#define Border_h
#include "../../../Semaphore/Semaphore.hpp"
#include "../../../Types.hpp"

/**
 Extension to add border-support in Objects
 */
struct Border {
    Semaphore<CairoPattern*> borderPattern;
    Semaphore<Color> borderColor;
    
    void set_border_pattern(CairoPattern* pattern){
        borderPattern.set(pattern);
    }
    
    void set_border_color(const Color color){
        borderColor.set(color);
    }
};

typedef Border Bordered;

#endif /* Border_h */
