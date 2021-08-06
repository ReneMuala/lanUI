//
//  Spacer.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 02/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Spacer_hpp
#define Spacer_hpp

#include "../Object/Object.hpp"

class Spacer : public Object {
public:
    Spacer(){
        Spacer(0,0);
    }
    Spacer(const float w, const float h){
        Object();
        size.set({0,0,w,h});
    }
};

#endif /* Spacer_hpp */
