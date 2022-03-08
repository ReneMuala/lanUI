//
//  WindowManager.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/01/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include "WindowManager.hpp"
#include "../../../Settings.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../Font/Font.hpp"
#include "../../Theme/Theme.hpp"
#include <unordered_map>
#include <thread>
#include <queue>
#include <vector>
#include "../Window.hpp"
#include "../../Font/Font.hpp"

namespace WMSSharedData
{
Semaphore<unsigned long> WMCount = 0;
bool SYSTEM_FONTS_ARE_VALID = false,
SYSTEM_READY = false, RENDERER_THREAD_READY = false, GLOBAL_FL_LIBRARY_READY = false;
Semaphore<IMG_InitFlags> SDL_IMG_INIT_FLAGS = IMG_InitFlags(0);

std::vector<Window *> programWindows(LUI::MAX_PROGRAM_WINDOWS);
std::vector<BSemaphore> windowCanClose(LUI::MAX_PROGRAM_WINDOWS);
std::vector<FT_Library> WindowFTLibraries(LUI::MAX_PROGRAM_WINDOWS);

std::thread rendererHandler;
std::queue<std::pair<void* /* return address */, unsigned /* function call id */>> ttf_thread_pendent_calls;

bool on_quit_callback_flag;
VoidCallback on_quit_callback;
}

namespace FontsSharedData
{
extern Semaphore<std::unordered_map<unsigned long /* FONT_ID */, TTF_Font*>> allFonts;
}

namespace Fonts
{
extern Font DejaVuSans, WorkSans, OpenSans;
}

namespace InteractiveObjectsData
{
    extern Semaphore<SDL_Point> cursor;
    extern Semaphore<Object*> selectedObject;
}

void WindowManager::on_quit(VoidCallback callback){
    WMSSharedData::on_quit_callback_flag = true;
    WMSSharedData::on_quit_callback = callback;
}

void WindowManager::run_global_events_handler(){
    static bool FRUN = true;
    static SDL_Event event;
    WMSSharedData::SYSTEM_READY = true;
    while (WMSSharedData::WMCount.get()) {
        WMSSharedData::WMCount.leave();
        while (FRUN || SDL_WaitEventTimeout(&event, 5) != 0) {
            if(FRUN) FRUN = false;
            if(event.type == SDL_QUIT && WMSSharedData::on_quit_callback_flag)
                WMSSharedData::on_quit_callback();
            else if(event.type == SDL_MOUSEBUTTONDOWN)
                WindowManager::set_selected_object(nullptr);
            for(int i = 0 ; i < LUI::MAX_PROGRAM_WINDOWS ; i++){
                if(WMSSharedData::programWindows[i]){
                    WMSSharedData::programWindows[i]->sdlEvent.set(event);
                    WMSSharedData::programWindows[i]->_handle_events();
                    if(WMSSharedData::programWindows[i])
                    WMSSharedData::programWindows[i]->_handle_requests();
                }
            }
        }
    } WMSSharedData::WMCount.leave();
}

void WindowManager::rendererHandlerRoutine(){
    while (WMSSharedData::RENDERER_THREAD_READY) {
        for(int i = 0 ; i < LUI::MAX_PROGRAM_WINDOWS ; i++){
            if(WMSSharedData::SYSTEM_READY){
                WMSSharedData::windowCanClose[i].hold();
                if(WMSSharedData::programWindows[i]){
                    WMSSharedData::programWindows[i]->_run_default_animation();
                    WMSSharedData::programWindows[i]->_compose();
                    WMSSharedData::programWindows[i]->_clear();
                    WMSSharedData::programWindows[i]->_render();
                    WMSSharedData::windowCanClose[i].leave();
                    std::this_thread::sleep_for((std::chrono::milliseconds)LUI::WM_RENDERER_THREAD_SLEEP_TIME);
                } else
                    WMSSharedData::windowCanClose[i].leave();
            }
        }
    }
}

WindowManager::WindowManager(void* window) {
    this->window = window;
    init();
    ready = true;
}

void WindowManager::init_incr(){
    WMSSharedData::WMCount.get()++;
    WMSSharedData::WMCount.leave();
}

void WindowManager::init_sdl(){
    if(!SDL_WasInit(SDL_INIT_VIDEO)){
        if(SDL_Init(SDL_INIT_VIDEO))
            log(Error, std::string("Count not initialize SDL, possible cause: ") + SDL_GetError());
    }
}

void WindowManager::init_ttf(){
    if(!TTF_WasInit()){
        if(TTF_Init() == -1){
            log(Error, std::string("Count not initialize TTF, possible cause: ") + SDL_GetError());
        }
    }
}

void WindowManager::init_image(){
    WMSSharedData::SDL_IMG_INIT_FLAGS.hold();
    if(!WMSSharedData::SDL_IMG_INIT_FLAGS.data){
        if(IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG | IMG_INIT_TIF) != (IMG_INIT_PNG | IMG_INIT_PNG | IMG_INIT_TIF)){
            log(Error, std::string("Count not initialize IMG, possible cause: ") + SDL_GetError());
        } else {
            WMSSharedData::SDL_IMG_INIT_FLAGS.data = IMG_InitFlags(IMG_INIT_JPG & IMG_INIT_PNG & IMG_INIT_TIF);
        }
    } WMSSharedData::SDL_IMG_INIT_FLAGS.leave();
}

void WindowManager::init_fonts(){
    if(!WMSSharedData::SYSTEM_FONTS_ARE_VALID)
    {
        WMSSharedData::SYSTEM_FONTS_ARE_VALID = Fonts::DejaVuSans.is_valid() && Fonts::OpenSans.is_valid() && Fonts::WorkSans.is_valid();
                
        Themes::_default._set_default_font();
    }
}

void WindowManager::init(){
    init_incr();
    init_sdl();
    init_ttf();
    init_fonts();
    init_image();
}

void WindowManager::start(const WindowInitParams params){
    ((Window*)(window))->_create(params.title.c_str(), (Window::Definition) params.definition, params.width, params.height);
    start_init_window_ft_library();
    start_rendererHandler_thread();
}

void WindowManager::start_init_global_ft_library(){
    if(!WMSSharedData::GLOBAL_FL_LIBRARY_READY){
        FT_Error error;
        const int32_t id = 0;
        if((error = FT_Init_FreeType(&WMSSharedData::WindowFTLibraries[id]))){
            log(Error, (std::string("Unable to init FreeType2 for global library. FT_Error: ")+ std::to_string(error)).c_str());
        } else {
            WMSSharedData::GLOBAL_FL_LIBRARY_READY = true;
        }
    }
}

void WindowManager::start_init_window_ft_library(){
    FT_Error error;
    const int32_t id = ((Window*)(window))->sdlWindowId.get_copy();
    if((error = FT_Init_FreeType(&WMSSharedData::WindowFTLibraries[id]))){
        log(Error, (std::string("Unable to init FreeType2 for window id(") + std::to_string(id) + "). FT_Error: " + std::to_string(error)).c_str());
    }
}

void WindowManager::start_rendererHandler_thread(){
    if(!WMSSharedData::RENDERER_THREAD_READY){
        WMSSharedData::RENDERER_THREAD_READY = true;
        WMSSharedData::rendererHandler = std::thread(rendererHandlerRoutine);
    }
}

void WindowManager::log(const WindowManager::LogLevel level, const std::string message){
    static unsigned long long logCount = 0;
    switch (level) {
        case LogLevel::Warning:
            fprintf(stderr, "[%s WARNING %llu]: \"%s\"\n", LUIVersion, logCount++, message.c_str());
            break;
            
        case LogLevel::Error:
            throw std::runtime_error("[" + std::string(LUIVersion) + " ERROR " + std::to_string(logCount++) + "]: \"" + message + "\"");
            break;
            
        default:
            fprintf(stderr, "[%s LOG %llu]: \"%s\"\n", LUIVersion, logCount++, message.c_str());
            break;
    }
}

void WindowManager::set_selected_object(void*object){
    if(InteractiveObjectsData::selectedObject.get())
        InteractiveObjectsData::selectedObject.data->_on_unselected_default_callback();
    InteractiveObjectsData::selectedObject.leave();
    InteractiveObjectsData::selectedObject.set((Object*)object);
}

const void * WindowManager::get_selected_object(){
    return (void*)InteractiveObjectsData::selectedObject.data;
}

void WindowManager::close_all(){
    for(int i = 0 ; i < LUI::MAX_PROGRAM_WINDOWS ; i++){
        if(WMSSharedData::programWindows[i]){
            if(WMSSharedData::programWindows[i]->callbacks[Window::OnClosed].get()){
                WMSSharedData::programWindows[i]->on_closed_callback();
            }
        }
    }
}

bool WindowManager::close_disable_window(){
    if(window){
        WMSSharedData::windowCanClose[((Window*)(window))->sdlWindowId.get_copy()].hold();
        WMSSharedData::programWindows[((Window*)(window))->sdlWindowId.data] = NULL;
        ((Window*)(window))->~Window();
        WMSSharedData::windowCanClose[((Window*)(window))->sdlWindowId.data].leave();
        return true;
    } return false;
}

bool WindowManager::close_disable_decr_window(){
    bool last_WM = !(WMSSharedData::WMCount.get());
    WMSSharedData::WMCount.leave();
    if(close_disable_window()){
        close_done_window_ft_library();
        last_WM = !(--WMSSharedData::WMCount.get());
        WMSSharedData::WMCount.leave();
        WMSSharedData::SYSTEM_READY = !last_WM;
    } return last_WM;
}

void WindowManager::close_stop_renderer_thread(){
    WMSSharedData::RENDERER_THREAD_READY = false;
    WMSSharedData::rendererHandler.join();
}

void WindowManager::close_done_global_ft_library(){
    if(WMSSharedData::GLOBAL_FL_LIBRARY_READY){
        FT_Error error;
        if((error = FT_Done_FreeType(WMSSharedData::WindowFTLibraries[0]))){
            log(Error, (std::string("Unable to done FreeType2 for global library. FT_Error: ") + std::to_string(error)).c_str());
        } else {
            WMSSharedData::GLOBAL_FL_LIBRARY_READY = false;
        }
    }
}

void WindowManager::close_done_window_ft_library(){
    FT_Error error;
    const int32_t id = ((Window*)(window))->sdlWindowId.get_copy();
    if((error = FT_Done_FreeType(WMSSharedData::WindowFTLibraries[id]))){
        log(Error, (std::string("Unable to done FreeType2 for window id(") + std::to_string(id) + "). FT_Error: " + std::to_string(error)).c_str());
    }
}

void WindowManager::close_sdl(){
    SDL_Quit();
}

void WindowManager::close_ttf(){
    TTF_Quit();
}

void WindowManager::close_image(){
    IMG_Quit();
}

void WindowManager::close(){
    // last WM in LUI eco-system
    if(ready && close_disable_decr_window()){
        ready = false;
        close_done_global_ft_library();
        close_stop_renderer_thread();
        close_sdl();
        close_ttf();
        close_image();
    }
}
