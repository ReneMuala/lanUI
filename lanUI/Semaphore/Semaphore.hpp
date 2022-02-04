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
void _hold_debug_callback(void*, bool, const bool, const std::string typname = "");
void _leave_debug_callback(void*, bool, const bool, const std::string typname = "");
}
#endif


struct BSemaphore {
    
#ifdef LANUI_DEBUG_MODE
    bool errorless = false;
#endif
    
    bool isBusy;
    BSemaphore():isBusy(false){};
    
    void force_hold(){
        if(!isBusy)
            hold();
    }
    
    virtual void hold() {
#ifdef LANUI_DEBUG_MODE
        SemaphoreDebuggingExtension::_hold_debug_callback(this, isBusy, errorless);
#endif
        while(isBusy)
            std::this_thread::sleep_for((const std::chrono::milliseconds)1);
        isBusy = true;
    }
    
    virtual void leave() {
#ifdef LANUI_DEBUG_MODE
        SemaphoreDebuggingExtension::_leave_debug_callback(this, isBusy, errorless);
#endif
        isBusy = false;
    }
};

template <typename any>
struct Semaphore : public BSemaphore {
    
    any data;
    
    Semaphore(){
        BSemaphore();
    }
    Semaphore(any value): data(value){
        BSemaphore();
    }
    
    void hold() override {
#ifdef LANUI_DEBUG_MODE
        SemaphoreDebuggingExtension::_hold_debug_callback(this, isBusy, errorless, typeid(any).type_info::name());
#endif
        while(isBusy)
            std::this_thread::sleep_for((const std::chrono::milliseconds)1);
        isBusy = true;
    }
    
    void leave() override {
#ifdef LANUI_DEBUG_MODE
        SemaphoreDebuggingExtension::_leave_debug_callback(this, isBusy, errorless, typeid(any).type_info::name());
#endif
        isBusy = false;
    }
    
    /// async
    any & get(){
        hold();
        return data;
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
