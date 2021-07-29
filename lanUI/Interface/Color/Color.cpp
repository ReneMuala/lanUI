//
//  Color.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include <stdio.h>
#include "Color.hpp"

namespace Colors {
const Color fromRGA(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return {r,g,b,a};
}
const Color fromColorA(const Color color, uint8_t a){
    return {color.r,color.g,color.b,a};
}
}
