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
class Stack : public Object {
public:
    Object * first, * last;
    virtual void free();
    virtual Object * requestObject(size_t index){return nullptr;};
    virtual Stack fromList(std::list<Object*>){return *this;};
    Stack();
};

/** Vertical Stack.
 Embeds objects in a vertical plan
 */
class VStack : public Stack {
public:
    Stack& reload() override;
    Object * requestObject(size_t index) override;
    Stack fromList(std::list<Object*>) override;
};

/** Vertical Stack.
 Embeds objects in a horizontal plan
 */
class HStack : public Stack {
public:
    Stack& reload() override;
    Object * requestObject(size_t index) override;
    Stack fromList(std::list<Object*>) override;
};

/** Vertical Stack.
 Embeds objects in a "Z" plan
 */
// (CAN'T EMBED MORE THAN ONE TEXT, OR STACK-BASED OBJECT)
class ZStack : public Stack {
public:
    Stack& reload() override;
    Object * requestObject(size_t index) override;
    Stack fromList(std::list<Object*>) override;
};

#endif /* Stack_hpp */
