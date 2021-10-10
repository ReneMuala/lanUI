//
//  Core.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Core_hpp
#define Core_hpp

#include <stdio.h>
#include <thread>
#include <string>

#include "../Settings.hpp"
#include "../Semaphore/Semaphore.hpp"
#include <SDL2/SDL.h>

class Core {

public:
    
    typedef enum {
        Error,
        Warning,
        Message,
    } LogLevel;
    
    bool terminated;
    
public:
    
    Core();
    ~Core();
    
    static void log(LogLevel ,const char * );
    static void init();
    static void init_SDL();
    static void events();
    static void render();
    static void close_SDL();
    static bool subscribe(void*);
    static bool unsubscribe(void*);
    static void clearCache();
    void load_fonts();
    static void free_all_fonts();
    void set_sleep_time(std::chrono::milliseconds);
    void terminate();
};

namespace CoreData {
    extern bool WAS_INIT;
    extern Semaphore<short> programWindowsCount;
}

namespace InteractiveObjecsData{
    /// cursor position (DPI Enabled)
    extern Semaphore<SDL_Point> cursor;
}

#endif /* Core_hpp */
