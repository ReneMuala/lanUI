//
//  Button.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 08/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

#include "../Object/Object.hpp"

class Button : public InterativeObject {
public:
    Button(){
        Object();
        InterativeObject();
    }
};

#endif /* Button_hpp */
