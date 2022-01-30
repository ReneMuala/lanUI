//
//  List.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 11/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "List.hpp"
#include "../../Core/Core.hpp"

#include <iostream>

BSList::BSList(const Sint32 maxSpeed): count(0), index(0) {
    this->maxSpeed = maxSpeed;
    set_content(content);
    on_scroll(CallbackExpr({
        state.hold();
        state.data.VSpeed += (abs(state.data.VSpeed) < this->maxSpeed) ?  scrollGain.vertical : 0;
        state.leave();
    }));
    
    content
    .set_default_animation(0,
                           CallbackExpr({
        if(state.get().VSpeed)
            state.data.VSpeed = (state.data.VSpeed > 0) ?
            state.data.VSpeed - (.05f) : state.data.VSpeed + (.05f);
        
        if((int)(state.data.VSpeed*10) == 0) state.data.VSpeed = 0;
        content.last->isVericallyBeforeRootEnding.hold();
        content.first->isVericallyAfterRootBeginning.hold();
        if(!content.last->isVericallyBeforeRootEnding.data && state.data.VSpeed < 0
           || !content.first->isVericallyAfterRootBeginning.data && state.data.VSpeed > 0)
            state.data.VScroll += state.data.VSpeed;
        else /* USER IS TRYING TO SCROLL BUT THE LIST IS IN THE ENDED */
        {
            // BEGINNIG
            if(state.data.VSpeed>0){/*reset VScroll*/state.data.VScroll=0;
                
            }
            // ENDING
            else {}
            state.data.VSpeed = 0;
        }
        
        content.first->isVericallyAfterRootBeginning.leave();
        content.last->isVericallyBeforeRootEnding.leave();
                
        if(content.nextInZ.get()) content.nextInZ.data->set_scrollingFactor({0,state.data.VScroll});
        content.nextInZ.leave();
        state.leave();
        return true;
    }));
}

BSList::~BSList(){
    if(count)
        content.free();
    InterativeObject::~InterativeObject();
}

