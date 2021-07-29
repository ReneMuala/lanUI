//
//  Semaphore.hpp
//  Poster
//
//  Created by René Descartes Domingos Muala on 12/06/21.
//

#ifndef Semaphore_hpp
#define Semaphore_hpp

#include <thread>

template <typename any>
struct Semaphore {
    
    bool isBusy;
    any data;
    
    Semaphore():isBusy(false){}
    Semaphore(any value): isBusy(false), data(value){}
    
    /// async
    any & get(){
        hold();
        return data;
    }
    
    void hold(){
        while(isBusy)
            std::this_thread::sleep_for((const std::chrono::milliseconds)1);
        isBusy = true;
    }
    
    void leave(){
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
