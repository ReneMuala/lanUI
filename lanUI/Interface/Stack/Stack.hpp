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
class BSStack : public Object {
public:
    Object * first, * last;
    virtual void free();
    virtual Object * requestObject(size_t index){return nullptr;};
    virtual BSStack fromList(std::list<Object*>){return *this;};
    BSStack();
};

/** Vertical Stack.
 Embeds objects in a vertical plan
 */
class BSVStack : public BSStack {
public:
    BSStack& reload() override;
    Object * requestObject(size_t index) override;
    BSStack fromList(std::list<Object*>) override;
};

/** Vertical Stack.
 Embeds objects in a horizontal plan
 */
class BSHStack : public BSStack {
public:
    BSStack& reload() override;
    Object * requestObject(size_t index) override;
    BSStack fromList(std::list<Object*>) override;
};

/** Vertical Stack.
 Embeds objects in a "Z" plan
 */
// (CAN'T EMBED MORE THAN ONE TEXT, OR STACK-BASED OBJECT)
class BSZStack : public BSStack {
public:
    BSStack& reload() override;
    Object * requestObject(size_t index) override;
    BSStack fromList(std::list<Object*>) override;
};

#endif /* Stack_hpp */
