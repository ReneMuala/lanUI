//
//  Window.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <string>
#include <functional>
#include <SDL2/SDL.h>
#include "../../Semaphore.hpp"
#include "../Color/Color.hpp"
#include "../Object/Object.hpp"

class Window: public Object {
public:
    typedef std::function<void()> VoidCallback;
public:
    // SDL2 things :P
    Semaphore<SDL_Window*> sdlWindow;
    Semaphore<SDL_Color> sdlWindowClearColor;
    Semaphore<SDL_Renderer*> sdlRenderer;
    Semaphore<SDL_Event> sdlEvent;
    
    Semaphore<int32_t> sdlWindowId;
    
    // hints
    Semaphore<bool> shouldClose;
    Semaphore<bool> onFullscreen;
    Semaphore<bool> isShown;
    Semaphore<bool> isMinimixed;
    Semaphore<bool> isMaximized;
    Semaphore<bool> hasMouseFocus;
    Semaphore<bool> hasKeyboardFocus;
    
    // Request data
    Semaphore<std::string> title;
    Semaphore<Color> windowClearColor;
    Semaphore<float> opacity;
    Semaphore<bool> fullscreen;
    
    // callbacks
    Semaphore<VoidCallback> on_start_callback;
    Semaphore<VoidCallback> on_closed_callback;
    Semaphore<VoidCallback> on_focus_gained_callback;
    Semaphore<VoidCallback> on_focus_lost_callback;
    Semaphore<VoidCallback> on_mouse_gained_callback;
    Semaphore<VoidCallback> on_mouse_lost_callback;
    Semaphore<VoidCallback> on_hidden_callback;
    Semaphore<VoidCallback> on_shown_callback;
    Semaphore<VoidCallback> on_minimized_callback;
    Semaphore<VoidCallback> on_maximized_callback;
    Semaphore<VoidCallback> on_resized_callback;
    Semaphore<VoidCallback> on_mouse_button_down_callback;
    Semaphore<VoidCallback> on_mouse_button_up_callback;
    Semaphore<VoidCallback> on_key_down_callback;
    Semaphore<VoidCallback> on_key_up_callback;
    
    typedef enum {
        Title,
        WindowClearColor,
        Opacity,
        FullScreen,
        NoFullScreen,
        Focus,
        Hide,
        Maximize,
        Minimize,
        totalRequests
    } WinRequests;
    
    typedef enum {
        OnStart,
        OnClosed,
        OnFocusGained,
        OnFocusLost,
        OnMouseGained,
        OnMouseLost,
        OnHidden,
        OnShown,
        OnMinimized,
        OnMaximized,
        OnResized,
        OnMouseButtonDown,
        OnMouseButtonUp,
        OnKeyDown,
        OnKeyUp,
        totalCallBacks
    } CallBacks;
    
    Semaphore<bool> winRequests[WinRequests::totalRequests] = {0};
    Semaphore<bool> callbacks[CallBacks::totalCallBacks] = {0};
    
    void subscribe();
    
    void create(const char *, float, float);
    
    void handle_events();
    
    void handle_requests();
    
    void handle_callBacks(const uint8_t, const uint32_t);
    
    void clear();
    
    void render();
    
public:
    
    
    Window(const char * title = "window", float width = 600, float height = 400);
    
    ~Window();
    
    Semaphore<Object*> view();
    
    Window& set_size(const float w, const float h) override;

    Window& set_title(const char * title);
    
    Window& set_opacity(float opacity);
    
    Window& set_window_clear_color(Color);
    
    Window& set_fullscreen(bool fullscreen);

    Window& set_focus();
    
    Window& set_view(Object&);

    Window& hide();

    Window& maximize();

    Window& minimize();
        
    Window& on_start(VoidCallback);
    
    Window& on_closed(VoidCallback);
    
    Window& on_focus_gained(VoidCallback);
    
    Window& on_focus_lost(VoidCallback);
    
    Window& on_mouse_gained(VoidCallback);
    
    Window& on_mouse_lost(VoidCallback);
    
    Window& on_hidden(VoidCallback);
    
    Window& on_shown(VoidCallback);
    
    Window& on_minimized(VoidCallback);
    
    Window& on_maximized(VoidCallback);
    
    Window& on_resized(VoidCallback);
        
    Window& on_mouse_button_down(VoidCallback);
    
    Window& on_mouse_button_up(VoidCallback);
        
    Window& on_key_down(VoidCallback);
    
    Window& on_key_up(VoidCallback);
};

#endif /* Window_hpp */
