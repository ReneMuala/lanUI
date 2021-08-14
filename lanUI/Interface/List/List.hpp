//
//  List.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 11/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

#include "../Object/Object.hpp"
#include "../Stack/Stack.hpp"

class List : public InterativeObject {
    std::list<Object*> baseList;
public:
    typedef std::function<Object*()> ObjPtrCallback;
    struct ScrollSate {
        Sint32 state, limit;
    };
public:
    List();
    Semaphore<ScrollGain> totalScrollGain;
    // TODO: IMPLEMENT LIST::DEFAULT_ANIMATION
    ScrollSate state;
    VStack content;
    
    // TODO: IMPLEMENT LIST::RENDER(...);
    
    template<unsigned repeat>
    List& compute(ObjPtrCallback callback){
        baseList.clear();
        Object * obj;
        for(unsigned i = 0 ; i < repeat; i++)
        {
            if((obj = callback()))
                baseList.push_back(obj);
            else
                break;
        } content.fromList(baseList);
        content.set_padding({0,0,0,0});
        return (*this);
    }
};

#endif /* List_hpp */
