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
#include "../Semaphore.hpp"

/// Lambda body to be passed as a function argument
#define CallbackExpr(x) [&]{x}

#define lanUI "landia user interface builder"
#define lanUIVersion "lanUI 0.1"

class Core {

public:
    
    typedef enum {
        Error,
        Warning,
        Message,
    } LogLevel;
    
public:
    
    Core();
    ~Core();
    
    static void log(LogLevel ,const char * );
    static void init();
    static void init_SDL();
    static void events();
    static void render();
    static void close();
    static void close_SDL();
    static bool subscribe(void*);
    static bool unsubscribe(void*);
    void load_fonts();
    static void free_all_fonts();
    void set_sleep_time(std::chrono::milliseconds);
    void terminate();
};

namespace CoreData
{
    extern bool WAS_INIT;
    extern Semaphore<short> programWindowsCount;
}

#endif /* Core_hpp */
