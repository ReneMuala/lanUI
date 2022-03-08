//
//  Window.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Window.hpp"
#include "WindowManager/WindowManager.hpp"
#include <thread>
#include <iostream>
#include <queue>

namespace WMSSharedData
{
    extern std::vector<Window *> programWindows;
}

namespace InteractiveObjectsData
{
    extern Semaphore<SDL_Point> cursor;
    extern Semaphore<Object*> selectedObject;
}

Window::Window(const char * title, float width, float height, Definition definition): manager(this), firstRun(true){
    //
    for(int i = 0 ; i < (WinRequests::totalRequests) ; i++){
        winRequests[i].set(false);
    }
    
    for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
        callbacks[i].set(false);
    }
    
    sdlWindowClearColor.set({15, 15, 15, 255});
    
    shouldClose.set(false);
    
    manager.start(WindowManager::WindowInitParams(title, (short)definition, width, height));
    
    _compute_dpiK();
}

Window::~Window(){
    if(sdlWindow.get()){
        SDL_DestroyWindow(sdlWindow.data);
        sdlWindow.data = nullptr;
    } sdlWindow.leave();
}

void Window::_create(const char *title, Definition definition, float width, float height){
    if(!(sdlWindow.data = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (size.data.w = width), (size.data.h = height), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | definition)))
        WindowManager::log(WindowManager::Error, "Unable to create window");
    else
        if(!(sdlRenderer.data = SDL_CreateRenderer(sdlWindow.data, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
            WindowManager::log(WindowManager::Error, "Unable to create window renderer");
    SDL_SetRenderDrawBlendMode(sdlRenderer.data, SDL_BLENDMODE_BLEND);
    HDWindow = definition & SDL_WINDOW_ALLOW_HIGHDPI;
    sdlWindowId.data = SDL_GetWindowID(sdlWindow.data);
    WMSSharedData::programWindows[sdlWindowId.data] = this;
    SDL_SetWindowMinimumSize(sdlWindow.data, 10, 10);
}

void Window::_handle_events(){
    if(firstRun){
        firstRun = false;
        _handle_others_routine(sdlEvent.data, nextInZ.data, obj_dpiK.get(), true);
        obj_dpiK.leave();
    }
    
    if(sdlEvent.data.window.windowID == sdlWindowId.data){
        if(sdlEvent.data.type == SDL_MOUSEMOTION){
            InteractiveObjectsData::cursor.get().x = sdlEvent.data.motion.x * obj_dpiK.get();
            InteractiveObjectsData::cursor.data.y = sdlEvent.data.motion.y * obj_dpiK.data;
            InteractiveObjectsData::cursor.leave();
            obj_dpiK.leave();
        } else if(HDWindow && sdlEvent.data.type == SDL_WINDOWEVENT){
            _compute_dpiK();
        }
        
        switch (sdlEvent.data.window.event) {
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
                break;
            default:
                break;
        }
        _handle_callBacks(sdlEvent.data.window.event, sdlEvent.data.type);
        if(sdlEvent.data.type == SDL_MOUSEBUTTONDOWN)
            WindowManager::set_selected_object(nullptr);
        if(nextInZ.get()) {
            _handle_others_routine(sdlEvent.data, nextInZ.data, obj_dpiK.get(), false);
            obj_dpiK.leave();
        } nextInZ.leave();
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
    } if(winRequests[WinRequests::SetView].get()) {
        embedInZ(*newView.get());
        newView.data = nullptr;
        newView.leave();
    } if(winRequests[WinRequests::Hide].get()) {
        SDL_HideWindow(sdlWindow.get());
        sdlWindow.leave();
    }
    // we are not reseting the busy state in ths if's
    for(int i = 0 ; i < (WinRequests::totalRequests) ; i++){
        winRequests[i].leave();
        winRequests[i].set(false);
    }
}

void Window::_handle_callBacks(const uint8_t window_event, const uint32_t input_event){
    if(callbacksFlag.get()){
        callbacksFlag.leave();
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
        } if(callbacks[OnResized].get() && (window_event == SDL_WINDOWEVENT_RESIZED || window_event == SDL_WINDOWEVENT_SIZE_CHANGED || window_event == SDL_WINDOWEVENT_MAXIMIZED || window_event == SDL_WINDOWEVENT_MINIMIZED)){
            on_resized_callback();
        } if(callbacks[OnMouseButtonDown].get() && input_event == SDL_MOUSEBUTTONDOWN){
            on_mouse_button_down_callback();
        } if(callbacks[OnMouseButtonUp].get() && input_event == SDL_MOUSEBUTTONUP){
            on_mouse_button_up_callback();
        }  if(callbacks[OnKeyDown].get() && input_event == SDL_KEYDOWN){
            on_key_down_callback();
        } if(callbacks[OnKeyUp].get() && input_event == SDL_KEYUP){
            on_key_up_callback();
        }
        
        // we are not reseting the busy state in ths if's
        for(int i = 0 ; i < (CallBacks::totalCallBacks) ; i++){
            callbacks[i].leave();
        }
    } else
        callbacksFlag.leave();
}

void Window::_run_default_animation(){
    if(nextInZ.get()) {
        nextInZ.leave();
        nextInZ.data->rendererParams.dpik = obj_dpiK.get();
        obj_dpiK.leave();
        nextInZ.data->_run_default_animation();
    } else
        nextInZ.leave();
}

void Window::_compute_dpiK(){
    int
    // drawable x and y
    drw, drh,
    // window x and y
    ww, wh;
    SDL_GL_GetDrawableSize(sdlWindow.get(), &drw, &drh);
    SDL_GetWindowSize(sdlWindow.data, &ww, &wh);
    sdlWindow.leave();
    obj_dpiK.set((float)drw/ww);
    size.leave();
}

void Window::_compose(){
    if(nextInZ.get()) {
        nextInZ.leave();
        nextInZ.data->_compose(sdlRenderer.get(), obj_dpiK.get(), sdlWindowId.get());
        sdlRenderer.leave();
        obj_dpiK.leave();
        sdlWindowId.leave();
    } else
        nextInZ.leave();
}

void Window::_clear(){
    sdlWindowClearColor.hold();
    // hold renderer
    SDL_SetRenderDrawColor(sdlRenderer.get(), sdlWindowClearColor.data.r,sdlWindowClearColor.data.g,sdlWindowClearColor.data.b, sdlWindowClearColor.data.a);
    sdlWindowClearColor.leave();
    SDL_RenderClear(sdlRenderer.data);
    sdlRenderer.leave();
}

void Window::_render(){
    _run_default_animation();
    sdlRenderer.hold();
    nextInZ.hold();
    obj_dpiK.hold();
    sdlWindowId.hold();
    _render_routine(obj_dpiK.data);
    _lock_renderer_in_bounds(sdlWindowId.data, sdlRenderer.data, obj_dpiK.data);
    if(nextInZ.data) nextInZ.data->_render(sdlWindowId.data, sdlRenderer.data, 0.0, 0.0, obj_dpiK.data);
    obj_dpiK.leave();
    _unlock_renderer_from_bounds(sdlWindowId.data, sdlRenderer.data);
    sdlWindowId.leave();
    nextInZ.leave();
    SDL_RenderPresent(sdlRenderer.data);
    sdlRenderer.leave();
}

void Window::close(){
    manager.close();
}

void Window::close_all(){
    manager.close_all();
}

Semaphore<Object*> Window::view(){
    if(!nextInZ.get()) WindowManager::log(WindowManager::Error, "A window view must to be declared befure using window.view()");
    nextInZ.leave();
    return nextInZ;
}

Window& Window::disable_resizing(const bool disable){
    SDL_SetWindowResizable(sdlWindow.get(), (SDL_bool)!disable);
    sdlWindow.leave();
    return (*this);
}

Window& Window::set_title(const char *title){
    winRequests[WinRequests::Title].set(true);
    this->title.set(title);
    return (*this);
}

Window& Window::set_size(const float w, const float h){
    if(!fullscreen.get()){
        size.hold();
        size.data.w = (w >= 0) ? w : size.data.w;
        size.data.h = (w >= 0) ? w : size.data.h;
        SDL_SetWindowSize(sdlWindow.get(), size.data.w, size.data.h);
        size.leave();
        sdlWindow.leave();
    } fullscreen.leave();
    return (*this);
}

Window& Window::set_minumum_size(const float w, const float h){
    sdlWindow.hold();
    SDL_SetWindowMinimumSize(sdlWindow.data, w, h);
    sdlWindow.leave();
    return (*this);
}

Window& Window::set_maximum_size(const float w, const float h){
    sdlWindow.hold();
    SDL_SetWindowMaximumSize(sdlWindow.data, w, h);
    sdlWindow.leave();
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
    // to avoid "Object already has a view" errors
    view.root.set(nullptr);
    winRequests[WinRequests::SetView].set(true);
    newView.set(&view);
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
    callbacksFlag.set(true);
    callbacks[CallBacks::OnStart].set(true);
    this->on_start_callback = callback;
    return (*this);
}

Window& Window::on_closed(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnClosed].set(true);
    this->on_closed_callback = callback;
    return (*this);
}

Window& Window::on_focus_gained(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnFocusGained].set(true);
    this->on_focus_gained_callback = callback;
    return (*this);
}

Window& Window::on_focus_lost(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnFocusLost].set(true);
    this->on_focus_lost_callback = callback;
    return (*this);
}

Window& Window::on_mouse_gained(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnMouseGained].set(true);
    this->on_mouse_gained_callback = callback;
    return (*this);
}

Window& Window::on_mouse_lost(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnMouseLost].set(true);
    this->on_mouse_lost_callback = callback;
    return (*this);
}

Window& Window::on_hidden(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnHidden].set(true);
    this->on_hidden_callback = callback;
    return (*this);
}

Window& Window::on_shown(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnShown].set(true);
    this->on_shown_callback = callback;
    return (*this);
}

Window& Window::on_minimized(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnMinimized].set(true);
    this->on_minimized_callback = callback;
    return (*this);
}

Window& Window::on_maximized(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnMaximized].set(true);
    this->on_maximized_callback = callback;
    return (*this);
}

Window& Window::on_resized(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnResized].set(true);
    this->on_resized_callback = callback;
    return (*this);
}

Window& Window::on_mouse_button_down(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnMouseButtonDown].set(true);
    this->on_mouse_button_down_callback = callback;
    return (*this);
}

Window& Window::on_mouse_button_up(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnMouseButtonUp].set(true);
    this->on_mouse_button_up_callback = callback;
    return (*this);
}

Window& Window::on_key_down(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnKeyDown].set(true);
    this->on_key_down_callback = callback;
    return (*this);
}

Window& Window::on_key_up(VoidCallback callback){
    callbacksFlag.set(true);
    callbacks[CallBacks::OnKeyUp].set(true);
    this->on_key_up_callback = callback;
    return (*this);
}
