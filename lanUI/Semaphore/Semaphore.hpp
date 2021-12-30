//
//  Semaphore.hpp
//  Poster
//
//  Created by René Descartes Domingos Muala on 12/06/21.
//

#ifndef Semaphore_hpp
#define Semaphore_hpp

#include <thread>
#include "../Settings.hpp"

#ifdef LANUI_DEBUG_MODE
namespace SemaphoreDebuggingExtension {
void _hold_debug_callback(void*, bool, const bool);
void _leave_debug_callback(void*, bool, const bool);
}
#endif

template <typename any>
struct Semaphore {
    
#ifdef LANUI_DEBUG_MODE
    bool errorless = false;
#endif
    
    bool isBusy;
    any data;
    
    Semaphore():isBusy(false){}
    Semaphore(any value): isBusy(false), data(value){}
    
    /// async
    any & get(){
        hold();
        return data;
    }
    
    void force_hold(){
        if(!isBusy)
            hold();
    }
    
    void hold(){
#ifdef LANUI_DEBUG_MODE
        SemaphoreDebuggingExtension::_hold_debug_callback(this, isBusy, errorless);
#endif
        while(isBusy)
            std::this_thread::sleep_for((const std::chrono::milliseconds)1);
        isBusy = true;
    }
    
    void leave(){
#ifdef LANUI_DEBUG_MODE
        SemaphoreDebuggingExtension::_leave_debug_callback(this, isBusy, errorless);
#endif
        isBusy = false;
    }
    
    /// async
    void set(const any & data){
        hold();
        this->data = data;
        leave();
    }
    
    /// async
    any const operator=(any&other){
        set(other);
        return data;
    }
};

#endif /* semaphore_hpp */
