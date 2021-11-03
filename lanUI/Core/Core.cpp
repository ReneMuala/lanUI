//
//  Core.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Core.hpp"

#include "../Interface/Window/Window.hpp"
#include "../Interface/Font/Font.hpp"
#include "../Project/CustomFonts/CustomFonts.hpp"

#include <map>
#include <unordered_map>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <regex>

namespace CoreData
{
bool running;
bool WAS_INIT;
/*! number of supported windows, you may increase this in faster computers */
const short maxProgramWindows = 16 ;
Semaphore<short> programWindowsCount = 0;
Semaphore<Window*> programWindows[maxProgramWindows] = {0};
// std::thread eventHandler;
std::thread renderHandler;
Semaphore<std::chrono::milliseconds> sleepTime = (std::chrono::milliseconds)20;
}

namespace DrawableObjectsData {
    extern Semaphore<std::map<const char *, SDL_Surface*>> surfacesCache;
}

namespace InteractiveObjecsData {
    extern Semaphore<SDL_Point> cursor;
    extern Semaphore<Object*> selectedObject;
}

namespace Fonts {
    extern Font DejaVuSans, WorkSans, OpenSans;
    extern Semaphore<std::unordered_map<unsigned long /* FONT_ID */, TTF_Font*>> allFonts;
}

Core::Core(): terminated(false){
#ifdef LANUI_DEBUG_MODE
    Core::log(Core::Message, "Using LANUI_DEBUG_MODE");
    CoreData::programWindowsCount.errorless = true;
#endif
    init();
    load_fonts();
    CoreData::running = true;
    //CoreData::eventHandler = std::thread(Core::events);
    CoreData::renderHandler = std::thread(Core::render);
}

Core::~Core(){
    if(!terminated) {
        CoreData::programWindowsCount.set(0);
        CoreData::WAS_INIT = false;
        CoreData::running = false;
        //CoreData::eventHandler.join();
        CoreData::renderHandler.join();
        clearCache();
        free_fonts();
        close_SDL();
        terminated = true;
    }
}

void Core::log(Core::LogLevel level, const char * message){
    switch (level) {
        case Core::LogLevel::Error:
            throw std::runtime_error("[" + std::string(lanUIVersion) + " Error]: " + message);
            break;
        case Core::LogLevel::Warning:
            fprintf(stdout, "[%s Warning]: %s\n", lanUIVersion, message);
            break;
        case Core::LogLevel::Message:
            fprintf(stdout, "[%s Message]: %s\n", lanUIVersion, message);
            break;
        default:
            break;
    }
}

void Core::init(){
    for(short i = 0 ; i < CoreData::maxProgramWindows; i++)
        CoreData::programWindows[i].data = nullptr;
    
    init_SDL();
    
    CoreData::programWindowsCount.set(0);
    CoreData::WAS_INIT = true;
}

void Core::init_SDL(){
    if(SDL_Init(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) != 0)
        log(LogLevel::Error, "Core: Unable to init SDL VIDEO");
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG | IMG_INIT_TIF) != (IMG_INIT_PNG | IMG_INIT_PNG | IMG_INIT_TIF))
        log(LogLevel::Error, "Core: Unable to init SDL Image");
    if(TTF_Init() == -1)
        log(LogLevel::Error, "Core: Unable to init SDL ttf");
}

void Core::events(){
    Core::log(Message, "Starting events thread.");
    while(CoreData::running){
        static short size (0);
        size = CoreData::programWindowsCount.get();
        CoreData::programWindowsCount.leave();
        for(short i = 0 ; i < size;){
            if(CoreData::programWindows[i].data){
                if(CoreData::programWindows[i].isBusy)continue;
                CoreData::programWindows[i].data->_handle_requests();
                CoreData::programWindows[i].data->_handle_events();
                i++;
            }
        }
    }
}

void Core::render(){
    Core::log(Message, "Starting render thread.");
    while (CoreData::running) {
        static short size (0);
        size = CoreData::programWindowsCount.get();
        CoreData::programWindowsCount.leave();
        for(short i = 0 ; i < size;){
            if(CoreData::programWindows[i].data){
                if(!CoreData::programWindows[i].isBusy) {
                    if(!CoreData::programWindows[i].data->hasKeyboardFocus.get()) {
                        std::this_thread::sleep_for(CoreData::sleepTime.get());
                        CoreData::sleepTime.leave();
                    }
                    CoreData::programWindows[i].data->hasKeyboardFocus.leave();
                    CoreData::programWindows[i].data->_run_default_animation();
                    CoreData::programWindows[i].data->_compile();
                    CoreData::programWindows[i].data->_clear();
                    CoreData::programWindows[i].data->_render();
                }
                i++;
            }
        }
    }
}

void Core::close_SDL(){
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

bool Core::subscribe(void * address){
    CoreData::programWindowsCount.hold();
    for(short i = 0 ; i < CoreData::maxProgramWindows; i++){
        if(!CoreData::programWindows[i].data){
            CoreData::programWindows[i].data = (Window*)address;
            CoreData::programWindowsCount.data++;
            CoreData::programWindowsCount.leave();
            return true;
        }
    }
    CoreData::programWindowsCount.leave();
    log(Error, "Unable to subscribe window, all buffers are busy.");
    return false;
}

bool Core::unsubscribe(void * address){
    CoreData::programWindowsCount.hold();
    for(short i = 0 ; i < CoreData::programWindowsCount.data; i++){
        if(CoreData::programWindows[i].data == (Window*)address){
            CoreData::programWindows[i].data = nullptr;
            CoreData::programWindowsCount.data--;
            CoreData::programWindowsCount.leave();
            return true;
        }
    }
    if(CoreData::programWindowsCount.data)
        log(Error, "Unable to unsubscribe window, unknowon address.");
    CoreData::programWindowsCount.leave();
    return false;
}

void Core::clearCache(){
    for (auto &surface : DrawableObjectsData::surfacesCache.get()) {
        SDL_FreeSurface(surface.second);
    } DrawableObjectsData::surfacesCache.data.clear();
    DrawableObjectsData::surfacesCache.leave();
}

void Core::set_selected_object(void * object){
    InteractiveObjecsData::selectedObject.set((Object *)object);
}

const void * Core::get_selected_object(){
    return InteractiveObjecsData::selectedObject.data;
}

void Core::compute_path(std::string & path){
    static const std::regex fontsHintReg("(Fonts)\\:", std::regex_constants::icase);
    
    
    if(std::regex_match(path.substr(0, 6), fontsHintReg)){
        path.replace(0,6, LANUI_DEFAULT_FONTS_PATH);
    }
}

void Core::load_fonts(){
    Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans.ttf", Font::Style::Regular);
    Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-Bold.ttf", Font::Style::Bold);
    Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-BoldOblique.ttf", Font::Style::BoldOblique);
    Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-ExtraLight.ttf", Font::Style::ExtraLight);
    Fonts::DejaVuSans.fromFile("Fonts:DejaVuSans-Oblique.ttf", Font::Style::Oblique);
    Fonts::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed-Bold.ttf", Font::Style::Condensed_Bold);
    Fonts::DejaVuSans.fromFile("Fonts:DejaVuSansCondensed-BoldOblique.ttf", Font::Style::Condensed_BoldOblique);
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
    CustomFonts::_loadCustomFonts();
    Fonts::set_default_font();
}

void Core::free_fonts(){
    Fonts::allFonts.hold();
    for (auto& font  : Fonts::allFonts.data) {
        if(font.second){
            TTF_CloseFont(font.second);
            font.second = nullptr;
        }
    }
    Fonts::allFonts.leave();
}

void Core::set_sleep_time(std::chrono::milliseconds sleepTime){
    CoreData::sleepTime.set(sleepTime);
}

void Core::terminate(){
    this->~Core();
}

