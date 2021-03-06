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
#include "../../Semaphore/Semaphore.hpp"
#include "../Color/Color.hpp"
#include "../Object/Object.hpp"
#include "WindowManager/WindowManager.hpp"
#include "../../Types.hpp"
#include "../Text/TextParams/TextParams.hpp"

//! A program window
class Window: public Object {
private:
    
    WindowManager manager;
    bool firstRun;
    bool HDWindow;
    
    Semaphore<std::string> title;
    Semaphore<Color> windowClearColor;
    Semaphore<float> opacity;
    Semaphore<bool> fullscreen;
    Semaphore<Object*> newView;
    
    // callbacks
    VoidCallback on_start_callback;
    VoidCallback on_focus_gained_callback;
    VoidCallback on_focus_lost_callback;
    VoidCallback on_mouse_gained_callback;
    VoidCallback on_mouse_lost_callback;
    VoidCallback on_hidden_callback;
    VoidCallback on_shown_callback;
    VoidCallback on_minimized_callback;
    VoidCallback on_maximized_callback;
    VoidCallback on_resized_callback;
    VoidCallback on_mouse_button_down_callback;
    VoidCallback on_mouse_button_up_callback;
    VoidCallback on_key_down_callback;
    VoidCallback on_key_up_callback;
public:
    /// made public because of WindowManager routines, should not be used outside if LUI internal routines.
    VoidCallback on_closed_callback;
public:
    // SDL2 things :P
    Semaphore<SDL_Window*> sdlWindow;
    Semaphore<SDL_Color> sdlWindowClearColor;
    Semaphore<SDL_Renderer*> sdlRenderer;
    Semaphore<SDL_Event> sdlEvent;
    Semaphore<int32_t> sdlWindowId;
    
    // state
    Semaphore<bool> shouldClose;
    Semaphore<bool> onFullscreen;
    Semaphore<bool> isShown;
    Semaphore<bool> isMinimixed;
    Semaphore<bool> isMaximized;
    Semaphore<bool> hasMouseFocus;
    Semaphore<bool> hasKeyboardFocus;
    
    TextParams textParams;
    
    typedef enum {
        HighDefinition = SDL_WINDOW_ALLOW_HIGHDPI,
        NormalDefinition = 0
    } Definition;
    
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
        SetView,
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
    Semaphore<bool> winRequestsFlag = false;
    Semaphore<bool> callbacks[CallBacks::totalCallBacks] = {0};
    Semaphore<bool> callbacksFlag = false;
    
    void _subscribe();
    
    void _create(const char *, Definition, float, float);
    
    void _handle_events();
    
    void _handle_requests();
        
    void _handle_callBacks(const uint8_t, const uint32_t);
    
    void _run_default_animation() override;
    
    void _compute_dpiK();
    
    void _compose();
    
    void _clear();
    
    void _render();
    
public:
    
    
    /** Create a new window instance.
     @param definition may be HighDefinition for HDPI/Retina monitors or NormalDefinition
     */
    Window(const char * title = "window", float width = 600, float height = 400, Definition definition = HighDefinition);
    
    ~Window();
    
    void close();
    
    void close_all();
    
    /** Get default window view
     */
    Semaphore<Object*> view();
    
    Window& set_size(const float w, const float h) override;
    
    Window& set_minumum_size(const float w, const float h);
    
    Window& set_maximum_size(const float w, const float h);
    
    Window& disable_resizing(const bool);
    
    Window& set_title(const char * title);
    
    Window& set_opacity(float opacity);
    
    Window& set_window_clear_color(Color);
    
    Window& set_fullscreen(bool fullscreen);

    Window& set_focus();
    
    /** Set default window view
     */
    Window& set_view(Object&);

    Window& hide();

    Window& maximize();

    Window& minimize();
    
    // The functions above set callbacks for window events
    
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
