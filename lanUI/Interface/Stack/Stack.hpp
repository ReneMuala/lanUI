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
    virtual void fromList(std::list<Object*>){};
    Stack();
};

class VStack : public Stack {
public:
    void fromList(std::list<Object*>) override;
};

class HStack : public Stack {
public:
    void fromList(std::list<Object*>) override;
};


/// Embeds  objects in the Z axis  (can't embed more than one Text, VStack or HStack)
class ZStack : public Stack {
public:
    void fromList(std::list<Object*>) override;
};

#endif /* Stack_hpp */
