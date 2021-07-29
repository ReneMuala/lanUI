//
//  Core.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Core.hpp"
#include "../Interface/Window/Window.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <iostream>

namespace CoreData
{
bool running;
bool WAS_INIT;
Semaphore<short> programWindowsCount = 0;
Semaphore<Window*> programWindows[256 /*number of supported windows, you may increase this in faster computers*/] = {0};
// std::thread eventHandler;
std::thread renderHandler;
Semaphore<std::chrono::milliseconds> sleepTime = (std::chrono::milliseconds)20;
Semaphore<std::map<const char *, SDL_Surface*>> surfaces;
}

Core::Core(){
    init();
    CoreData::running = true;
    //CoreData::eventHandler = std::thread(Core::events);
    CoreData::renderHandler = std::thread(Core::render);
}

Core::~Core(){
    close();
    //CoreData::eventHandler.join();
    CoreData::renderHandler.join();
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
    init_SDL_VIDEO();
    CoreData::programWindowsCount.set(0);
    CoreData::WAS_INIT = true;
}

void Core::init_SDL_VIDEO(){
    if(SDL_Init(SDL_Init(SDL_INIT_VIDEO)) != 0)
        log(LogLevel::Error, "Core: Unable to init SDL VIDEO");
}

void Core::events(){
    while(CoreData::running){
        short size = CoreData::programWindowsCount.get();
        CoreData::programWindowsCount.leave();
        for(short i = 0 ; i < size; i++){
            if(CoreData::programWindows[i].isBusy)continue;
            CoreData::programWindows[i].data->handle_requests();
            CoreData::programWindows[i].data->handle_events();
        } std::this_thread::sleep_for(CoreData::sleepTime.get());
        CoreData::sleepTime.leave();
    }
}

void Core::render(){
    while (CoreData::running) {
        short size = CoreData::programWindowsCount.get();
        CoreData::programWindowsCount.leave();
        for(short i = 0 ; i < size; i++){
            if(!CoreData::programWindows[i].isBusy) {
                if(!CoreData::programWindows[i].data->hasKeyboardFocus.get()) {
                    std::this_thread::sleep_for(CoreData::sleepTime.get());
                    CoreData::sleepTime.leave();
                }
                CoreData::programWindows[i].data->hasKeyboardFocus.leave();
                CoreData::programWindows[i].data->hasKeyboardFocus.leave();
                CoreData::programWindows[i].data->clear();
                CoreData::programWindows[i].data->render();
            }
        }
    }
}

void Core::close(){
    CoreData::WAS_INIT = false;
    close_SDL();
}

void Core::close_SDL(){
    SDL_Quit();
}

void Core::subscribe(void * address){
    CoreData::programWindowsCount.hold();
    CoreData::programWindows[CoreData::programWindowsCount.data++] = (Window*)address;
    CoreData::programWindowsCount.leave();
}

void Core::unsubscribe(void *){
    
}

void Core::set_sleep_time(std::chrono::milliseconds sleepTime){
    CoreData::sleepTime.set(sleepTime);
}

void Core::terminate(){
    CoreData::running = false;
    for (auto &test : CoreData::surfaces.get()) {
        SDL_FreeSurface(test.second);
    }
}

