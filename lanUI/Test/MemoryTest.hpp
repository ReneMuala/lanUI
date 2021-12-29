//
//  MemoryTest.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/11/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifdef LANUI_DEBUG_MODE

#ifndef MemoryTest_h
#define MemoryTest_h

#define LANUI_MEM_TESTING
#ifdef LANUI_MEM_TESTING
#endif

class MemTest : public Text {};


void TestMemory(size_t numObj, size_t numAlloc){
    MemTest * objs[numObj];
    
    for (int j = 0 ; j < numAlloc ; j++){
        for(int i = 0 ; i < numObj ; i ++){
            objs[i] = new MemTest;
    //        texts[i] -> from_string("Hello world", janela.sdlRenderer.get());
        }
        
        for(int i = 0 ; i < numObj ; i ++){
            delete objs[i];
            objs[i] = nullptr;
        }
    }
}

#endif /* MemoryTest_h */

#endif
