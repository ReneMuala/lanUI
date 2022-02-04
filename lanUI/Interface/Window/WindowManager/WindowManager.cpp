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
SYSTEM_READY = false;
Semaphore<IMG_InitFlags> SDL_IMG_INIT_FLAGS = IMG_InitFlags(0);
Window * programWindows[LUI_MAX_PROGRAM_WINDOWS];
std::thread ttf_thread;
std::queue<std::pair<void* /* return address */, unsigned /* function call id */>> ttf_thread_pendent_calls;
}

namespace FontsSharedData
{
extern Font DejaVuSans, WorkSans, OpenSans;
extern Semaphore<std::unordered_map<unsigned long /* FONT_ID */, TTF_Font*>> allFonts;
}

namespace InteractiveObjecsData
{
    extern Semaphore<SDL_Point> cursor;
    extern Semaphore<Object*> selectedObject;
}

void WindowManager::run_global_events_handler(){
#ifdef LUI_WM_MODE
    static SDL_Event event;
    WMSSharedData::SYSTEM_READY = true;
    while (WMSSharedData::WMCount.get()) {
        WMSSharedData::WMCount.leave();
        do {
            for(int i = 0 ; i < LUI_MAX_PROGRAM_WINDOWS ; i++){
                if(WMSSharedData::programWindows[i]){
                    WMSSharedData::programWindows[i]->_handle_requests();
                    WMSSharedData::programWindows[i]->sdlEvent.set(event);
                    WMSSharedData::programWindows[i]->_handle_events();
                }
            }
        } while (SDL_WaitEventTimeout(&event, 5) != 0);
    } WMSSharedData::WMCount.leave();
#else
    Core::log(Core::Error, "LUI_WM_MODE is not defined.");
#endif
}

void WindowManager::rendererHandlerRoutine(void * window, bool * running){
    Window * win = ((Window*)window);
    while (*running) {
        if(WMSSharedData::SYSTEM_READY){
            win->_run_default_animation();
            win->_compile();
            win->_clear();
            win->_render();
            std::this_thread::sleep_for((std::chrono::milliseconds)LUI_WM_RENDERER_THREAD_SLEEP_TIME);
        }
    }
}

WindowManager::WindowManager(void* window) {
    this->window = window;
    running = true;
    init();
}

void WindowManager::init_incr(){
    WMSSharedData::WMCount.get()++;
    WMSSharedData::WMCount.leave();
}

void WindowManager::init_sdl(){
    if(!SDL_WasInit(SDL_INIT_VIDEO)){
        if(SDL_Init(SDL_INIT_VIDEO))
            log(Error, std::string("Count not initialize SDL, possible cause: ") + SDL_GetError());
        init_ttf_thread();
    }
}

void WindowManager::init_ttf(){
    if(!TTF_WasInit()){
        if(TTF_Init() == -1){
            log(Error, std::string("Count not initialize TTF, possible cause: ") + SDL_GetError());
        } init_ttf_thread();
    }
}

void WindowManager::init_ttf_thread(){
    // FIXME: ALLOW TTF_THREAD TO BE SUCESSUF INITIALIZED (RIGHT NPW ITS NOT WORKING BECAUSE ITS BEING INITIALIZED AT WM CONSTRUCTOR)
    WMSSharedData::ttf_thread = std::thread(ttfThreadRoutine);
}

void WindowManager::ttfThreadRoutine(){
    while (WMSSharedData::SYSTEM_READY) {
        if(WMSSharedData::ttf_thread_pendent_calls.empty()){
            std::this_thread::sleep_for((std::chrono::milliseconds)100);
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
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSans.ttf", Font::Style::Regular);
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSans-Bold.ttf", Font::Style::Bold);
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSans-BoldOblique.ttf", Font::Style::BoldOblique);
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSans-ExtraLight.ttf", Font::Style::ExtraLight);
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSans-Oblique.ttf", Font::Style::Oblique);
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed-Bold.ttf", Font::Style::Condensed_Bold);
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed-BoldOblique.ttf", Font::Style::Condensed_BoldOblique);
        FontsSharedData::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed.ttf", Font::Style::Condensed);
        FontsSharedData::DejaVuSans.set_global_name("DejaVuSans");
        
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-Bold.ttf", Font::Bold);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-BoldItalic.ttf", Font::BoldItalic);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-ExtraBold.ttf", Font::ExtraBold);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-ExtraBoldItalic.ttf", Font::ExtraBoldItalic);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-Italic.ttf", Font::Italic);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-Light.ttf", Font::Light);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-LightItalic.ttf", Font::LightItalic);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-Medium.ttf", Font::Medium);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-MediumItalic.ttf", Font::MediumItalic);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-Regular.ttf", Font::Regular);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-SemiBold.ttf", Font::SemiBold);
        FontsSharedData::OpenSans.fromFile("Fonts:OpenSans-SemiBoldItalic.ttf", Font::SemiBoldItalic);
        FontsSharedData::OpenSans.set_global_name("OpenSans");
        
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-Black.ttf", Font::Black);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-BlackItalic.ttf", Font::BlackItalic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-Bold.ttf", Font::Bold);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-BoldItalic.ttf", Font::BoldItalic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-ExtraBold.ttf", Font::ExtraBold);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-ExtraBoldItalic.ttf", Font::ExtraBoldItalic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-ExtraLight.ttf", Font::ExtraLight);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-ExtraLightItalic.ttf", Font::ExtraLightItalic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-Italic.ttf", Font::Italic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-Light.ttf", Font::Light);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-LightItalic.ttf", Font::LightItalic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-Medium.ttf", Font::Medium);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-MediumItalic.ttf", Font::MediumItalic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-Regular.ttf", Font::Regular);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-SemiBold.ttf", Font::SemiBold);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-SemiBoldItalic.ttf", Font::SemiBoldItalic);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-Thin.ttf", Font::Thin);
        FontsSharedData::WorkSans.fromFile("Fonts:WorkSans-ThinItalic.ttf", Font::ThinItalic);
        FontsSharedData::WorkSans.set_global_name("WorkSans");
        
        WMSSharedData::SYSTEM_FONTS_ARE_VALID = FontsSharedData::DejaVuSans.isValid() && FontsSharedData::OpenSans.isValid() && FontsSharedData::WorkSans.isValid();
        
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
    rendererHandler = std::thread(rendererHandlerRoutine, window, &running);
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

void WindowManager::$set_selected_object(void*object){
    InteractiveObjecsData::selectedObject.set((Object*)object);
}

const void * WindowManager::$get_selected_object(){
    return (void*)InteractiveObjecsData::selectedObject.data;
}

bool WindowManager::close_decr(){
    const bool last_WM = !(--WMSSharedData::WMCount.get());
    WMSSharedData::SYSTEM_READY = !last_WM;
    WMSSharedData::WMCount.leave();
    return last_WM;
}

void WindowManager::close_sdl(){
    SDL_Quit();
}

void WindowManager::close_ttf(){
    close_ttf_thread();
    close_fonts();
    TTF_Quit();
}

void WindowManager::close_ttf_thread(){
    WMSSharedData::ttf_thread.join();
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
    if(close_decr()){
        close_sdl();
        close_fonts();
        close_ttf();
        close_image();
    } rendererHandler.join();
}

WindowManager::~WindowManager(){
    close();
}
