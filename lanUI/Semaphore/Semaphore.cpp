//
//  Semaphore.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Semaphore.hpp"
#ifdef LANUI_DEBUG_MODE

void SemaphoreDebuggingExtension::_hold_debug_callback(void* address, bool isBusy, const bool errorless, const std::string typname){
    static long long hold_id = 0;
    if(typname.empty()){
#ifdef LANUI_DEBUG_BSEMAPHORE_WARN_WHEN_INVALID_HOLD
    if(isBusy && !errorless)
        Core::log(Core::Warning, ("BSemaphore::hold(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] is being \'hold()\' when its busy. (if this message is being shown for a while and the id is not changind, the program probably stoped because of a bad implementation of Semaphore<any>).").c_str());
#elif defined LANUI_DEBUG_BSEMAPHORE_ERROR_WHEN_INVALID_HOLD
    if(isBusy && !errorless)
        Core::log(Core::Error, ("BSemaphore::hold(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] is being \'hold()\' when its busy. (if this message is being shown for a while and the id is not changind, the program probably stoped because of a bad implementation of Semaphore<any>).").c_str());
#endif
    } else {
#ifdef LANUI_DEBUG_SEMAPHORE_WARN_WHEN_INVALID_HOLD
    if(isBusy && !errorless)
        Core::log(Core::Warning, ("Semaphore::hold(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] if typeid::name: \"" + typname +"\" is being \'hold()\' when its busy. (if this message is being shown for a while, the program  was probably stoped because of a bad implementation of Semaphore<any>).").c_str());
#elif defined LANUI_DEBUG_SEMAPHORE_ERROR_WHEN_INVALID_HOLD
        if(isBusy && !errorless)
            Core::log(Core::Error, ("Semaphore::hold(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] if typeid::name: \"" + typname +"\" is being \'hold()\' when its busy. (if this message is being shown for a while, the program  was probably stoped because of a bad implementation of Semaphore<any>).").c_str());
#endif
    }
    hold_id++;
}

void SemaphoreDebuggingExtension::_leave_debug_callback(void* address, bool isBusy, const bool errorless, const std::string typname){
    static long long hold_id = 0;
    if(typname.empty()){
#ifdef LANUI_DEBUG_BSEMAPHORE_WARN_WHEN_INVALID_LEAVE
    if(!isBusy && !errorless)
        Core::log(Core::Warning, ("BSemaphore::leave(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] is being \'leave()\' when its already not busy. (bad implementation of Semaphore<any>).").c_str());
#elif defined LANUI_DEBUG_BSEMAPHORE_ERROR_WHEN_INVALID_LEAVE
    if(!isBusy && !errorless)
        Core::log(Core::Error, ("BSemaphore::leave(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] is being \'leave()\' when its already not busy. (bad implementation of Semaphore<any>).").c_str());
#endif
    } else {
#ifdef LANUI_DEBUG_SEMAPHORE_WARN_WHEN_INVALID_LEAVE
    if(!isBusy && !errorless)
        Core::log(Core::Warning, ("Semaphore::leave(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] of typeid::name: \"" + typname +"\" is being \'leave()\' when its already not busy. (bad implementation of Semaphore<any>).").c_str());
#elif defined LANUI_DEBUG_SEMAPHORE_ERROR_WHEN_INVALID_LEAVE
    if(!isBusy && !errorless)
        Core::log(Core::Error, ("Semaphore::leave(...) id " + std::to_string(hold_id)+ " : Object ["+std::to_string((long long)address)+"] of typeid::name: \"" + typname +"\" is being \'leave()\' when its already not busy. (bad implementation of Semaphore<any>).").c_str());
#endif
    }
    hold_id++;
}

#endif
