//
//  ScrollingFactor.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 20/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef ScrollingFactor_h
#define ScrollingFactor_h

struct ScrollingFactor {
    
    float horizontal, vertical;
    
    bool operator==(const ScrollingFactor other){
        return horizontal == other.horizontal && vertical == other.vertical;
    }
    
    bool operator != (const ScrollingFactor other){
        return !((*this) == other);
    }
};

#endif /* ScrollingFactor_h */
