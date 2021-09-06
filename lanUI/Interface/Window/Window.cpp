//
//  Window.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Window.hpp"
#include "../../Core/Core.hpp"
#include <thread>
#include <iostream>

Window::Window(const char * title, float width, float height, Definition definition){
    if(!CoreData::WAS_INIT){
        Core::init();
    }
    
    for(int i = 0 ; i < (WinRequests::totalRequests) ; i++){
        winRequests[i].leave();
        winRequests[i].set(false);
    }
    
    for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
        callbacks[i].leave();
        callbacks[i].set(false);
    }
    
    sdlWindowClearColor.set({15, 15, 15, 255});
    
    _create(title, definition, width, height);
    
    _compute_DPIConstant();
    
    set_title(title);
    
    shouldClose.set(false);
    
    Core::subscribe(this);
}

Window::~Window(){
    Core::unsubscribe(this);
    SDL_DestroyWindow(sdlWindow.get());
    sdlWindow.leave();
}

void Window::_create(const char *title, Definition definition, float width, float height){
    if(!(sdlWindow.data = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (size.data.w = width), (size.data.h = height), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | definition)))
        Core::log(Core::Error, "Unable to create window");
    else
        if(!(sdlRenderer.data = SDL_CreateRenderer(sdlWindow.data, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
            Core::log(Core::Error, "Unable to create window renderer");
    SDL_SetRenderDrawBlendMode(sdlRenderer.data, SDL_BLENDMODE_BLEND);
    sdlWindowId.data = SDL_GetWindowID(sdlWindow.data);
    SDL_SetWindowMinimumSize(sdlWindow.data, width, height);
}

void Window::_handle_events(){
    while (SDL_WaitEventTimeout(&sdlEvent.data, 5) != 0) {
        if(sdlEvent.data.type == SDL_DISPLAYEVENT)
            _compute_DPIConstant();
        if(sdlEvent.data.window.windowID == sdlWindowId.data){
            if(sdlEvent.data.type == SDL_MOUSEMOTION){
                InteractiveObjecsData::cursor.get().x = sdlEvent.data.motion.x * DPIConstant.get();
                InteractiveObjecsData::cursor.data.y = sdlEvent.data.motion.y * DPIConstant.data;
                InteractiveObjecsData::cursor.leave();
                DPIConstant.leave();
            }
            
            switch (sdlEvent.data.window.event) {
                case SDL_QUIT:
                case SDL_WINDOWEVENT_CLOSE: shouldClose.data = true; break;
                case SDL_WINDOWEVENT_ENTER: hasMouseFocus.set(true); break;
                case SDL_WINDOWEVENT_LEAVE: hasMouseFocus.set(false); break;
                case SDL_WINDOWEVENT_FOCUS_GAINED: hasKeyboardFocus.set(true); break;
                case SDL_WINDOWEVENT_FOCUS_LOST:   hasKeyboardFocus.set(false); break;
                case SDL_WINDOWEVENT_EXPOSED:
                case SDL_WINDOWEVENT_SHOWN: isShown.set(true); isMinimixed.set(false);  break;
                case SDL_WINDOWEVENT_HIDDEN: isShown.set(false); break;
                case SDL_WINDOWEVENT_MINIMIZED: isMinimixed.set(true); isMaximized.set(false); break;
                case SDL_WINDOWEVENT_MAXIMIZED: isMaximized.set(true); fullscreen.set(true); isMinimixed.set(true); break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                case SDL_WINDOWEVENT_RESIZED:
                    size.get();
                    size.data.w = (sdlEvent.data.window.data1);
                    size.data.h = (sdlEvent.data.window.data2);
                    size.leave();
                    fullscreen.set(false);
                    break;
                default:
                    break;
            }
            _handle_callBacks(sdlEvent.data.window.event, sdlEvent.data.type);
            
            if(nextInZ.get())
                _handle_others_routine(sdlEvent.data, nextInZ.data, DPIConstant.get());
            DPIConstant.leave();
            nextInZ.leave();
        }
    }
}

void Window::_handle_requests(){
    if(winRequests[WinRequests::Title].get()) {
        SDL_SetWindowTitle(sdlWindow.get(), title.get().c_str());
        sdlWindow.leave();
        title.leave();
    } if(winRequests[WinRequests::Opacity].get()) {
        SDL_SetWindowOpacity(sdlWindow.get(), opacity.get());
        opacity.leave();
        sdlWindow.leave();
    } if(winRequests[WinRequests::WindowClearColor].get()) {
        sdlWindowClearColor.set(windowClearColor.get());
        windowClearColor.leave();
    } if(winRequests[WinRequests::FullScreen].get()) {
        SDL_SetWindowFullscreen(sdlWindow.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
        fullscreen.set(true);
        sdlWindow.leave();
    } if(winRequests[WinRequests::NoFullScreen].get()) {
        SDL_SetWindowFullscreen(sdlWindow.get(), 0);
        fullscreen.set(false);
        sdlWindow.leave();
    } if(winRequests[WinRequests::Focus].get()) {
        SDL_RaiseWindow(sdlWindow.get());
        sdlWindow.leave();
    } if(winRequests[WinRequests::Maximize].get()) {
        SDL_MaximizeWindow(sdlWindow.get());
        sdlWindow.leave();
    } if(winRequests[WinRequests::Minimize].get()) {
        SDL_MinimizeWindow(sdlWindow.get());
        sdlWindow.leave();
    }
    // we are not reseting the busy state in ths if's
    for(int i = 0 ; i < (WinRequests::totalRequests) ; i++){
        winRequests[i].leave();
        winRequests[i].set(false);
    }
}

void Window::_handle_callBacks(const uint8_t window_event, const uint32_t input_event){
    if(callbacks[OnStart].get()){
        on_start_callback();
        // disable after first use
        callbacks[OnStart].data = false;
    } if(callbacks[OnClosed].get() && shouldClose.data){
        on_closed_callback();
        shouldClose.set(false);
    } if(callbacks[OnFocusGained].get() && window_event == SDL_WINDOWEVENT_FOCUS_GAINED){
        on_focus_gained_callback();
    } if(callbacks[OnFocusLost].get() && window_event == SDL_WINDOWEVENT_FOCUS_LOST){
        on_focus_lost_callback();
    } if(callbacks[OnMouseGained].get() && window_event == SDL_WINDOWEVENT_ENTER){
        on_mouse_gained_callback();
    } if(callbacks[OnMouseLost].get() && window_event == SDL_WINDOWEVENT_LEAVE){
        on_mouse_lost_callback();
    } if(callbacks[OnHidden].get() && window_event == SDL_WINDOWEVENT_HIDDEN){
        on_hidden_callback();
    } if(callbacks[OnShown].get() && window_event == SDL_WINDOWEVENT_SHOWN){
        on_shown_callback();
    } if(callbacks[OnMinimized].get() && window_event == SDL_WINDOWEVENT_MINIMIZED){
        on_minimized_callback();
    } if(callbacks[OnMaximized].get() && window_event == SDL_WINDOWEVENT_MAXIMIZED){
        on_maximized_callback();
    } if(callbacks[OnResized].get() && (window_event == SDL_WINDOWEVENT_RESIZED || window_event == SDL_WINDOWEVENT_SIZE_CHANGED)){
        on_resized_callback();
    } if(input_event == SDL_MOUSEBUTTONDOWN && callbacks[OnMouseButtonDown].get()){
        on_mouse_button_down_callback();
    } if(input_event == SDL_MOUSEBUTTONUP && callbacks[OnMouseButtonUp].get()){
        on_mouse_button_up_callback();
    }  if(input_event == SDL_KEYDOWN && callbacks[OnKeyDown].get()){
        on_key_down_callback();
    } if(input_event == SDL_KEYUP && callbacks[OnKeyUp].get()){
        on_key_up_callback();
    }
    
    hasMouseFocus.leave();
    // we are not reseting the busy state in ths if's
    for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
        callbacks[i].leave();
    }
}

void Window::_run_default_animation(){
    if(nextInZ.get()) {
        nextInZ.leave();
        nextInZ.data->_run_default_animation();
    } nextInZ.leave();
}

void Window::_compute_DPIConstant(){
    static int realw, realh;
    SDL_GL_GetDrawableSize(sdlWindow.get(), &realw, &realh);
    DPIConstant.set((float)realw/size.get().w);
    size.leave();
    sdlWindow.leave();
}

void Window::_clear(){
    hasKeyboardFocus.leave();
    sdlWindowClearColor.hold();
    // hold renderer
    SDL_SetRenderDrawColor(sdlRenderer.get(), sdlWindowClearColor.data.r,sdlWindowClearColor.data.g,sdlWindowClearColor.data.b, sdlWindowClearColor.data.a);
    sdlWindowClearColor.leave();
    SDL_RenderClear(sdlRenderer.data);
    sdlRenderer.leave();
}

void Window::_render(){
    _run_default_animation();
    if(nextInZ.get())
        nextInZ.data->_render(sdlRenderer.data, 0.0, 0.0, DPIConstant.get());
    DPIConstant.leave();
    nextInZ.leave();
    SDL_RenderPresent(sdlRenderer.get());
    sdlRenderer.leave();
}

Semaphore<Object*> Window::view(){
    if(!nextInZ.get()) Core::log(Core::Error, "A window view must do be declared befure using window.view()");
    nextInZ.leave();
    return nextInZ;
}

Window& Window::set_title(const char *title){
    winRequests[WinRequests::Title].set(true);
    this->title.set(title);
    return (*this);
}

Window& Window::set_size(const float w, const float h){
    if(!fullscreen.get()){
        size.set({0,0,w,h});
        size.hold();
        SDL_SetWindowSize(sdlWindow.get(), size.data.w, size.data.h);
        size.leave();
        sdlWindow.leave();
    } fullscreen.leave();
    return (*this);
}

Window& Window::set_opacity(float opacity){
    winRequests[WinRequests::Opacity].set(true);
    this->opacity.set(opacity);
    return (*this);
}

Window& Window::set_window_clear_color(Color color){
    winRequests[WinRequests::WindowClearColor].set(true);
    this->windowClearColor.set(color);
    return (*this);
}

Window& Window::set_fullscreen(bool fullscreen){
    winRequests[WinRequests::FullScreen].set(true);
    this->fullscreen.set(fullscreen);
    return (*this);
}

Window& Window::set_focus(){
    winRequests[WinRequests::Focus].set(true);
    return (*this);
}

Window& Window::set_view(Object & view){
    embedInZ(view);
    return (*this);
}

Window& Window::hide(){
    winRequests[WinRequests::Hide].set(true);
    return (*this);
}

Window& Window::maximize(){
    winRequests[WinRequests::Maximize].set(true);
    return (*this);
}

Window& Window::minimize(){
    winRequests[WinRequests::Minimize].set(true);
    return (*this);
}

Window& Window::on_start(VoidCallback callback){
    callbacks[CallBacks::OnStart].set(true);
    this->on_start_callback = callback;
    return (*this);
}

Window& Window::on_closed(VoidCallback callback){
    callbacks[CallBacks::OnClosed].set(true);
    this->on_closed_callback = callback;
    return (*this);
}

Window& Window::on_focus_gained(VoidCallback callback){
    callbacks[CallBacks::OnFocusGained].set(true);
    this->on_focus_gained_callback = callback;
    return (*this);
}

Window& Window::on_focus_lost(VoidCallback callback){
    callbacks[CallBacks::OnFocusLost].set(true);
    this->on_focus_lost_callback = callback;
    return (*this);
}

Window& Window::on_mouse_gained(VoidCallback callback){
    callbacks[CallBacks::OnMouseGained].set(true);
    this->on_mouse_gained_callback = callback;
    return (*this);
}

Window& Window::on_mouse_lost(VoidCallback callback){
    callbacks[CallBacks::OnMouseLost].set(true);
    this->on_mouse_lost_callback = callback;
    return (*this);
}

Window& Window::on_hidden(VoidCallback callback){
    callbacks[CallBacks::OnHidden].set(true);
    this->on_hidden_callback = callback;
    return (*this);
}

Window& Window::on_shown(VoidCallback callback){
    callbacks[CallBacks::OnShown].set(true);
    this->on_shown_callback = callback;
    return (*this);
}

Window& Window::on_minimized(VoidCallback callback){
    callbacks[CallBacks::OnMinimized].set(true);
    this->on_minimized_callback = callback;
    return (*this);
}

Window& Window::on_maximized(VoidCallback callback){
    callbacks[CallBacks::OnMaximized].set(true);
    this->on_maximized_callback = callback;
    return (*this);
}

Window& Window::on_resized(VoidCallback callback){
    callbacks[CallBacks::OnResized].set(true);
    this->on_resized_callback = callback;
    return (*this);
}

Window& Window::on_mouse_button_down(VoidCallback callback){
    callbacks[CallBacks::OnMouseButtonDown].set(true);
    this->on_mouse_button_down_callback = callback;
    return (*this);
}

Window& Window::on_mouse_button_up(VoidCallback callback){
    callbacks[CallBacks::OnMouseButtonUp].set(true);
    this->on_mouse_button_up_callback = callback;
    return (*this);
}

Window& Window::on_key_down(VoidCallback callback){
    callbacks[CallBacks::OnKeyDown].set(true);
    this->on_key_down_callback = callback;
    return (*this);
}

Window& Window::on_key_up(VoidCallback callback){
    callbacks[CallBacks::OnKeyUp].set(true);
    this->on_key_up_callback = callback;
    return (*this);
}
