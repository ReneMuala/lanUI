//
//  TextField.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "TextField.hpp"

TextField::TextField(Semaphore<std::string>& source, const std::string placeholder): activated(false), source(source), placeholder(placeholder){}
