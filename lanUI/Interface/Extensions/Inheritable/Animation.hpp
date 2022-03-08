//
//  Animation.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 20/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Animation_h
#define Animation_h
#include "../../../Types.hpp"

/**
 Extension to add animation-support in Objects
 */
struct Animation {
protected:
    bool isAnimationBeingUsed;
    Semaphore<FrameCount> animationDelay;
    Semaphore<BoolCallback> animationCallback;
    
    void set_animation(const FrameCount delay, BoolCallback callback){
        animationDelay.set(delay);
        animationCallback.set(callback);
    }
};

typedef Animation Animated;

#endif /* Animation_h */
