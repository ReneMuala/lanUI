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
#include "../Interface/Theme/Theme.hpp"

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
bool running = false, firstRun = true, WAS_INIT = false, FRendCompleted = false;
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

namespace FontsSharedData {
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
        while (!CoreData::FRendCompleted);
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
            throw std::runtime_error("[" + std::string(LUIVersion) + " Error]: " + message);
            break;
        case Core::LogLevel::Warning:
            fprintf(stdout, "[%s Warning]: %s\n", LUIVersion, message);
            break;
        case Core::LogLevel::Message:
            fprintf(stdout, "[%s Message]: %s\n", LUIVersion, message);
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
    Core::log(Message, "Starting events   thread.");
    while(CoreData::running){
        static short size (0);
        size = CoreData::programWindowsCount.get();
        CoreData::programWindowsCount.leave();
        for(short i = 0 ; i < size;){
            if(CoreData::programWindows[i].data){
                if(CoreData::programWindows[i].isBusy) continue;
                CoreData::programWindows[i].data->_handle_requests();
                CoreData::programWindows[i].data->_handle_events();
                i++;
            } if(CoreData::firstRun) CoreData::firstRun = false;
        }
    }
}

void Core::render(){
    Core::log(Message, "Starting renderer thread.");
    while (CoreData::running) {
        static short size (0);
        size = CoreData::programWindowsCount.get();
        CoreData::programWindowsCount.leave();
        if(CoreData::FRendCompleted) CoreData::FRendCompleted = false;
        for(short i = 0 ; i < size;){
            if(CoreData::programWindows[i].data){
                if(!CoreData::programWindows[i].isBusy) {
                    if(!CoreData::programWindows[i].data->hasKeyboardFocus.get()) {
                        std::this_thread::sleep_for(CoreData::sleepTime.get());
                        CoreData::sleepTime.leave();
                    } CoreData::programWindows[i].data->hasKeyboardFocus.leave();
                    CoreData::programWindows[i].data->_run_default_animation();
                    CoreData::programWindows[i].data->_compile();
                    CoreData::programWindows[i].data->_clear();
                    CoreData::programWindows[i].data->_render();
                }
                i++;
            }
        }
        CoreData::FRendCompleted = true;
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
    CustomFonts::_loadCustomFonts();
    Themes::_default._set_default_font();
}

void Core::free_fonts(){
    FontsSharedData::allFonts.hold();
    for (auto& font  : FontsSharedData::allFonts.data) {
        if(font.second){
            TTF_CloseFont(font.second);
            font.second = nullptr;
        }
    }
    FontsSharedData::allFonts.leave();
}

void Core::set_sleep_time(std::chrono::milliseconds sleepTime){
    CoreData::sleepTime.set(sleepTime);
}

void Core::terminate(){
    this->~Core();
}

