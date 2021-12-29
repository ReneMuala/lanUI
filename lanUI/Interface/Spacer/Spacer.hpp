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

/** An opaque object
 */
class BSSpacer : public Object {
public:
    BSSpacer(){
        BSSpacer(0,0);
    }
    BSSpacer(const float width, const float height){
        Object();
        size.set({0,0,width,height});
    }
};

#endif /* Spacer_hpp */
