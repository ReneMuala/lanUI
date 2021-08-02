//
//  Container.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 01/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Container_hpp
#define Container_hpp

#include "../Object/Object.hpp"

class Container : public Object {
public:
    void set_content(Object& object){
        embedInZ(object);
        set_size(object.size.get().w, object.size.data.h);
        object.size.leave();
    };
};

#endif /* Container_hpp */
