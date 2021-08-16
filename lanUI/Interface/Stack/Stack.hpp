//
//  Stack.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <list>
#include "../Object/Object.hpp"

/// Base class for VStack , HStack and ZStack.
class Stack : public DrawableObject {
public:
    Object * first, * last;
    virtual void fromList(std::list<Object*>){};
    Stack();
};

/** Vertical Stack.
 Embeds objects in a vertical plan
 */
class VStack : public Stack {
public:
    VStack& reload() override;
    void fromList(std::list<Object*>) override;
};

/** Vertical Stack.
 Embeds objects in a horizontal plan
 */
class HStack : public Stack {
public:
    HStack& reload() override;
    void fromList(std::list<Object*>) override;
};


/** Vertical Stack.
 Embeds objects in a "Z" plan
 */
// (CAN'T EMBED MORE THAN ONE TEXT, OR STACK-BASED OBJECT)
class ZStack : public Stack {
public:
    ZStack& reload() override;
    void fromList(std::list<Object*>) override;
};

#endif /* Stack_hpp */
