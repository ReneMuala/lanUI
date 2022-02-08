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
#include <thread>

namespace WMSSharedData
{
    extern Semaphore<IMG_InitFlags> SDL_IMG_INIT_FLAGS;
}

class WindowManager {
public:
    typedef std::function<void()> VoidCallback;
    
    static void on_quit(VoidCallback);
    
    static void run_global_events_handler();
    
    typedef enum {
        Error,
        Warning,
        Message,
    } LogLevel;

    struct WindowInitParams {
        std::string title;
        short definition;
        float width;
        float height;
        
        WindowInitParams(const char * title, short definition, float width, float height): title(title), definition(definition), width(width), height(height){
        }
    };
    
private:
    
    void * window;
    
    bool ready;
        
    static void rendererHandlerRoutine();
    
public:
    
    WindowManager(void*);
    
    void init_incr();
    
    void init_sdl();
        
    void init_ttf();
    
    void init_image();
    
    void init_fonts();
        
    void init();
    
    void start(const WindowInitParams);
    
    void start_rendererHandler_thread();
    
    static void log(const LogLevel , const std::string);
    
    static void set_selected_object(void*);
    
    static const void * get_selected_object();
    
    static void close_all();
    
    void close();
    
    
private:
    /*
     static void set_selected_object(void*);
     static const void * get_selected_object();
     */
    
    bool close_disable_window();
    
    bool close_disable_decr_window();
    
    void close_stop_renderer_thread();
    
    void close_sdl();
    
    void close_ttf();
        
    void close_image();
    
    void close_fonts();
};

#endif /* WindowManager_hpp */
