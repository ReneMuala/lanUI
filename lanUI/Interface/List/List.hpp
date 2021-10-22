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
#include <optional>

/** A Scrollable VStack
 */
class List : public InterativeObject {
    std::list<Object*> baseList;
    Sint32 maxSpeed;
public:
    typedef std::function<Object*()> ObjPtrCallback;
    struct ScrollSate {
        float VScroll, VSpeed;
    };
public:
    unsigned index;
    unsigned count;
    List(const Sint32 maxSpeed = 10);
    ~List();
    
    Semaphore<ScrollSate> state;
    VStack content;
    List& fit_content(const float width, const float height);
    template<typename anyObj, unsigned repeat>
    List& compute(ObjPtrCallback callback){
        count = repeat;
        baseList.clear();
        Object * obj;
        for(index = 0 ; index < repeat; index++)
        {
            if((obj = (anyObj*)callback()))
                baseList.push_back(obj);
            else
                break;
        }
        content.free();
        content.fromList(baseList);
        content.set_padding({0,0,0,0});
        return (*this);
    }
};

#endif /* List_hpp */
