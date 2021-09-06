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

#include <vector>
#include <list>
#include <map>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


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
    extern Semaphore<std::map<const char *, SDL_Surface*>> surfaces;
}

namespace InteractiveObjecsData {
    extern Semaphore<SDL_Point> cursor;
}

namespace Fonts {
    extern std::list<Font*> allFonts;
    extern Font DejaVuSans;
}

Core::Core(): terminated(false){
#ifdef LANUI_DEBUG_MODE
        Core::log(Core::Message, "Using LANUI_DEBUG_MODE");
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
        
        for (auto &test : DrawableObjectsData::surfaces.get()) {
            SDL_FreeSurface(test.second);
        }
        
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
    free_all_fonts();
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
    CoreData::programWindowsCount.leave();
    if(CoreData::programWindowsCount.data)
        log(Error, "Unable to unsubscribe window, unknowon address.");
    return false;
}

void Core::load_fonts(){
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSans.ttf", Font::Style::Regular);
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSans-Bold.ttf", Font::Style::Bold);
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSans-BoldOblique.ttf", Font::Style::BoldOblique);
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSans-ExtraLight.ttf", Font::Style::ExtraLight);
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSans-Oblique.ttf", Font::Style::Oblique);
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSansCondensed-Bold.ttf", Font::Style::Condensed_Bold);
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSansCondensed-BoldOblique.ttf", Font::Style::Condensed_BoldOblique);
    Fonts::DejaVuSans.fromFile("lanUI.Bundle/System/Library/Fonts/DejaVuSansCondensed.ttf", Font::Style::Condensed);
    CustomFonts::_loadCustomFonts();
}

void Core::free_all_fonts(){
    for (auto & it : Fonts::allFonts) {
        it->free();
    }
}

void Core::set_sleep_time(std::chrono::milliseconds sleepTime){
    CoreData::sleepTime.set(sleepTime);
}

void Core::terminate(){
    this->~Core();
}

