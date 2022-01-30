//
//  Loader.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 14/01/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Loader_hpp
#define Loader_hpp

#include <stdio.h>
#include "../Object/Object.hpp"

class BSLoader : public Object {
    float counter, total;
    
    BSLoader(): counter(0), total(100){};
    
    
};

#endif /* Loader_hpp */
