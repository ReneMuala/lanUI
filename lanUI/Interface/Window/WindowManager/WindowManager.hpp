//
//  WindowManager.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/01/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef WindowManager_hpp
#define WindowManager_hpp

#include <stdio.h>
#include <string>
#include <SDL2/SDL_image.h>
#include "../../../Semaphore/Semaphore.hpp"

namespace WMSSharedData
{
    extern Semaphore<IMG_InitFlags> SDL_IMG_INIT_FLAGS;
}

class WindowManager {
    typedef enum {
        Error,
        Warning,
        Message,
    } LogLevel;
    
    WindowManager();
    
    void init_incr();
    
    void init_sdl();
    
    void init_ttf();
    
    void init_image();
    
    void init_fonts();
    
    void init();
    
public:
    
    void log(const LogLevel , const std::string);
    
    static void $set_selected_object(void*);
    
    static const void * $get_selected_object();
    
private:
    /*
     static void set_selected_object(void*);
     static const void * get_selected_object();
     */
    
    bool close_decr();
    
    void close_sdl();
    
    void close_ttf();
    
    void close_image();
    
    void close_fonts();
    
    void close();
    
    ~WindowManager();
};

#endif /* WindowManager_hpp */
