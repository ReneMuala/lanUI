//
//  Color.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include <stdio.h>
#include "Color.hpp"

const std::string Color::toStr() const {
    return
    " \\color:rgba("+std::to_string(r)+","+std::to_string(g)+","+std::to_string(b)+","+std::to_string(a)+")";
}
