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
#include "../../../Project/CustomFonts/CustomFonts.hpp"
#include <unordered_map>
#include <thread>
#include <queue>
#include "../Window.hpp"

namespace WMSSharedData
{
Semaphore<unsigned long> WMCount = 0;
bool SYSTEM_FONTS_ARE_VALID = false,
SYSTEM_READY = false, RENDERER_THREAD_READY = false;
Semaphore<IMG_InitFlags> SDL_IMG_INIT_FLAGS = IMG_InitFlags(0);
Window * programWindows[LUI_MAX_PROGRAM_WINDOWS];
BSemaphore windowCanClose[LUI_MAX_PROGRAM_WINDOWS];

std::thread rendererHandler;
std::queue<std::pair<void* /* return address */, unsigned /* function call id */>> ttf_thread_pendent_calls;

bool on_quit_callback_flag;
Window::VoidCallback on_quit_callback;
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
            if(event.type == SDL_MOUSEBUTTONDOWN)
                WindowManager::set_selected_object(nullptr);
            for(int i = 0 ; i < LUI_MAX_PROGRAM_WINDOWS ; i++){
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
        for(int i = 0 ; i < LUI_MAX_PROGRAM_WINDOWS ; i++){
            if(WMSSharedData::SYSTEM_READY){
                WMSSharedData::windowCanClose[i].hold();
                if(WMSSharedData::programWindows[i]){
                    WMSSharedData::programWindows[i]->_run_default_animation();
                    WMSSharedData::programWindows[i]->_compile();
                    WMSSharedData::programWindows[i]->_clear();
                    WMSSharedData::programWindows[i]->_render();
                    WMSSharedData::windowCanClose[i].leave();
                    std::this_thread::sleep_for((std::chrono::milliseconds)LUI_WM_RENDERER_THREAD_SLEEP_TIME);
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
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans.ttf", Font::Style::Regular);
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-Bold.ttf", Font::Style::Bold);
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-BoldOblique.ttf", Font::Style::BoldOblique);
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-ExtraLight.ttf", Font::Style::ExtraLight);
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-Oblique.ttf", Font::Style::Oblique);
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed-Bold.ttf", Font::Style::Condensed_Bold);
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed-BoldOblique.ttf", Font::Condensed_BoldOblique);
        Fonts::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed.ttf", Font::Style::Condensed);
        Fonts::DejaVuSans.set_global_name("DejaVuSans");
        
        Fonts::OpenSans.fromFile("Fonts:OpenSans-Bold.ttf", Font::Bold);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-BoldItalic.ttf", Font::BoldItalic);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-ExtraBold.ttf", Font::ExtraBold);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-ExtraBoldItalic.ttf", Font::ExtraBoldItalic);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-Italic.ttf", Font::Italic);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-Light.ttf", Font::Light);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-LightItalic.ttf", Font::LightItalic);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-Medium.ttf", Font::Medium);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-MediumItalic.ttf", Font::MediumItalic);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-Regular.ttf", Font::Regular);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-SemiBold.ttf", Font::SemiBold);
        Fonts::OpenSans.fromFile("Fonts:OpenSans-SemiBoldItalic.ttf", Font::SemiBoldItalic);
        Fonts::OpenSans.set_global_name("OpenSans");
        
        Fonts::WorkSans.fromFile("Fonts:WorkSans-Black.ttf", Font::Black);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-BlackItalic.ttf", Font::BlackItalic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-Bold.ttf", Font::Bold);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-BoldItalic.ttf", Font::BoldItalic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-ExtraBold.ttf", Font::ExtraBold);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-ExtraBoldItalic.ttf", Font::ExtraBoldItalic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-ExtraLight.ttf", Font::ExtraLight);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-ExtraLightItalic.ttf", Font::ExtraLightItalic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-Italic.ttf", Font::Italic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-Light.ttf", Font::Light);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-LightItalic.ttf", Font::LightItalic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-Medium.ttf", Font::Medium);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-MediumItalic.ttf", Font::MediumItalic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-Regular.ttf", Font::Regular);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-SemiBold.ttf", Font::SemiBold);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-SemiBoldItalic.ttf", Font::SemiBoldItalic);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-Thin.ttf", Font::Thin);
        Fonts::WorkSans.fromFile("Fonts:WorkSans-ThinItalic.ttf", Font::ThinItalic);
        Fonts::WorkSans.set_global_name("WorkSans");
        
        WMSSharedData::SYSTEM_FONTS_ARE_VALID = Fonts::DejaVuSans.isValid() && Fonts::OpenSans.isValid() && Fonts::WorkSans.isValid();
        
        CustomFonts::_loadCustomFonts();
        
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
    start_rendererHandler_thread();
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
    for(int i = 0 ; i < LUI_MAX_PROGRAM_WINDOWS ; i++){
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
        last_WM = !(--WMSSharedData::WMCount.get());
        WMSSharedData::WMCount.leave();
        WMSSharedData::SYSTEM_READY = !last_WM;
    } return last_WM;
}

void WindowManager::close_stop_renderer_thread(){
    WMSSharedData::RENDERER_THREAD_READY = false;
    WMSSharedData::rendererHandler.join();
}

void WindowManager::close_sdl(){
    SDL_Quit();
}

void WindowManager::close_ttf(){
    close_fonts();
    TTF_Quit();
}

void WindowManager::close_image(){
    IMG_Quit();
}

void WindowManager::close_fonts(){
    FontsSharedData::allFonts.hold();
    for (auto& font  : FontsSharedData::allFonts.data) {
        if(font.second){
            TTF_CloseFont(font.second);
            font.second = nullptr;
        }
    } FontsSharedData::allFonts.leave();
}

void WindowManager::close(){
    // last WM in LUI eco-system
    if(ready && close_disable_decr_window()){
        ready = false;
        close_stop_renderer_thread();
        close_sdl();
        close_fonts();
        close_ttf();
        close_image();
    }
}
