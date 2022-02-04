//
//  Object.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"
#include "../../Core/Core.hpp"
#include <SDL2/SDL_image.h>
#include <map>
#include "../../Utilities/PathResolver.hpp"

namespace DrawableObjectsData {
Semaphore<std::map<const char *, SDL_Surface*>> surfacesCache;
}

SDL_Surface * get_surface(std::string source){
    source = PathResolver::resolve(source);
    SDL_Surface * surfc = nullptr;
    if(!(surfc = DrawableObjectsData::surfacesCache.get()[source.c_str()])){
        if((surfc = IMG_Load(source.c_str()))){
            DrawableObjectsData::surfacesCache.data[source.c_str()] = surfc;
        } else {
            goto get_surface_error;
        }
    } else {
        get_surface_error:
        Core::log(Core::Warning, (std::string("Unable to load image source from path: ") + source).c_str());
    } DrawableObjectsData::surfacesCache.leave();
    return surfc;
}

void Object::_compile_embedded(SDL_Renderer* renderer, const float dpiK){
    if(nextInX.get()) {
        nextInX.leave();
        nextInX.data->_compile(renderer, dpiK);
    } else nextInX.leave();
    
    if(nextInY.get()){
        nextInY.leave();
        nextInY.data->_compile(renderer, dpiK);
    } else nextInY.leave();
    
    if(nextInZ.get()) {
        nextInZ.leave();
        nextInZ.data->_compile(renderer, dpiK);
    } else nextInZ.leave();
}

void Object::_compile(SDL_Renderer* renderer, const float dpiK){
    _compile_embedded(renderer, dpiK);
}

void Object::_render_composition(SDL_Renderer * renderer, float x, float y, const float dpiK){
    sizeBuffer.hold();
    SDL_RenderCopyExF(renderer, compositionCanvas.get(), NULL, &sizeBuffer.data, angle.get(), NULL, SDL_FLIP_NONE);
    compositionCanvas.leave();
    angle.leave();
    
#ifdef LANUI_DEBUG_MODE
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 10);
        SDL_RenderFillRectF(renderer, &sizeBuffer.data);
//        SDL_SetRenderDrawColor(renderer, 20, 255, 200, 200);
//        SDL_RenderDrawRectF(renderer, &sizeBuffer.data);
#endif
    
    sizeBuffer.leave();
}

void Object::_render_image(SDL_Renderer * renderer, float x, float y, const float dpiK){
    sizeBuffer.hold();
    if(withBackground)
        _render_background(renderer, &sizeBuffer.data);
    SDL_RenderCopyExF(renderer, canvas.get(), NULL, &sizeBuffer.data, angle.get(), NULL, SDL_FLIP_NONE);
    //SDL_RenderCopyF(renderer, canvas.get(), NULL, &sizeBuffer.data);
    canvas.leave();
    angle.leave();
    if(withBorder)
        _render_border(renderer, &sizeBuffer.data);
    sizeBuffer.leave();
}

void Object::_render_color_scheme(SDL_Renderer * renderer, float x, float y, const float dpiK){
    foregroundColor.hold();
    SDL_SetRenderDrawColor(renderer, foregroundColor.data.r, foregroundColor.data.g, foregroundColor.data.b, foregroundColor.data.a);
    foregroundColor.leave();
    SDL_RenderFillRectF(renderer, &sizeBuffer.get());
    if(withBorder) _render_border(renderer, &sizeBuffer.data);
    sizeBuffer.leave();
}

void Object::_render_default(SDL_Renderer * renderer, float x, float y, const float dpiK) {
    _render_color_scheme(renderer, x, y, dpiK);
    borderColor.hold();
    SDL_SetRenderDrawColor(renderer, borderColor.data.r, borderColor.data.g, borderColor.data.b, borderColor.data.a);
    borderColor.leave();
    sizeBuffer.hold();
    _render_border(renderer, &sizeBuffer.data);
    SDL_RenderDrawLineF(renderer, sizeBuffer.data.x, sizeBuffer.data.y, sizeBuffer.data.x + sizeBuffer.data.w, sizeBuffer.data.y + sizeBuffer.data.h);
    SDL_RenderDrawLineF(renderer, sizeBuffer.data.x, sizeBuffer.data.y + sizeBuffer.data.h, sizeBuffer.data.x + sizeBuffer.data.w, sizeBuffer.data.y);
    sizeBuffer.leave();
}

Object& Object::set_render_mode(RenderMode mode){
    renderMode.set(mode);
    return (*this);
}

void Object::_free_canvas(Semaphore<Texture *>& canvas){
    if (canvas.get()){
        SDL_DestroyTexture(canvas.data);
    }
    canvas.data = nullptr;
    canvas.leave();
}

void Object::_free_canvas(){
    _free_canvas(canvas);
}

void Object::_generate_canvas(Renderer * renderer, Semaphore<Texture *>& canvas, const float dpiK){
    canvas.hold();
    size.hold();
    canvas.data = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.data.w * dpiK, size.data.h * dpiK);
    SDL_SetTextureBlendMode(canvas.data, SDL_BLENDMODE_BLEND);
    canvas.leave();
    size.leave();
}

void Object::_clear_canvas(Renderer * renderer, Semaphore<Texture *> & canvas){
    Texture* buffer = SDL_GetRenderTarget(renderer);
    canvas.hold();
    SDL_SetRenderTarget(renderer, canvas.data);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    canvas.leave();
    SDL_SetRenderTarget(renderer, buffer);
}

void Object::clear_surfaces_cache(){
    for (auto &surface : DrawableObjectsData::surfacesCache.get()) {
        SDL_FreeSurface(surface.second);
    } DrawableObjectsData::surfacesCache.data.clear();
    DrawableObjectsData::surfacesCache.leave();
}

Object& Object::fromFile(const char *filename, Renderer * renderer){
    fromSurface(get_surface(filename), renderer);
    return (*this);
}

Object& Object::fromSurface(Surface * surfc, Renderer * renderer, const bool reset_backgroundColor){
    if(surfc) {
        _free_canvas();
        this->canvas.set(SDL_CreateTextureFromSurface(renderer, surfc));
        this->renderer.set(renderer);
        size.set({0,0,(float)surfc->w, (float)surfc->h});
        renderMode.set(RenderMode::ImageMode);
        // ocultes the border / secondary color
        if(reset_backgroundColor)
            backgroundColor.set(Colors::Transparent);
    } else {
        Core::log(Core::Warning, "Unable to load canvas from image source (nullptr)");
    }
    return (*this);
}

Object& Object::fromColorScheme(const Color color, const Color color2){
    foregroundColor.set(color);
    backgroundColor.set(color2);
    withBackground = true;
    renderMode.set(RenderMode::ColorSchemeMode);
    return (*this);
}

Object& Object::set_foreground_color(const Color color){
    foregroundColor.set(color);
    return (*this);
}

Object& Object::set_background_color(const Color color){
    backgroundColor.set(color);
    withBackground = true;
    return (*this);
}

Object& Object::set_border_color(const Color color){
    borderColor.set(color);
    withBorder = true;
    return (*this);
}

void Object::_render_background(SDL_Renderer * renderer, Rect * rect){
    backgroundColor.hold();
    SDL_SetRenderDrawColor(renderer, backgroundColor.data.r, backgroundColor.data.g, backgroundColor.data.b, backgroundColor.data.a);
    backgroundColor.leave();
    SDL_RenderFillRectF(renderer, rect);
}

void Object::_render_border(SDL_Renderer * renderer, Rect * rect){
    borderColor.hold();
    SDL_SetRenderDrawColor(renderer, borderColor.data.r, borderColor.data.g, borderColor.data.b, borderColor.data.a);
    borderColor.leave();
    SDL_RenderDrawRectF(renderer, rect);
}

void Object::_render_using_callback(SDL_Renderer * renderer, float x, float y, float dpiK){
    param_renderer = renderer;
    param_dpiK = dpiK;
    
    renderer_callback.get()();
    renderer_callback.leave();
}

void Object::_render(const unsigned int id, SDL_Renderer * renderer, float x, float y, const float dpiK, bool inComposition){
    if(_inRootBounds(x, y) || inComposition){
        _align(x, y);
        _set_position(x, y);
        _render_routine(dpiK);
            switch (renderMode.get()) {
                case RenderMode::ImageMode:
                    if(renderer == this->renderer.get()){
                        this->renderer.leave();
                        _render_image(renderer, x, y, dpiK);
                    } else {
                        this->renderer.leave();
                        _render_default(renderer, x, y, dpiK);
                    } break;
                    
                case RenderMode::CallbackMode:
                    if(using_renderer_callback){
                        _render_using_callback(renderer, x, y, dpiK);
                    } else {
                        _render_default(renderer, x, y, dpiK);
                    } break;
                    
                case RenderMode::CompositionMode:
                    if(renderer == this->renderer.get()){
                        this->renderer.leave();
                        _render_composition(renderer, x, y, dpiK);
                    } else {
                        this->renderer.leave();
                        _render_default(renderer, x, y, dpiK);
                    } break;
                    
                case RenderMode::CanvasCompositionMode:
                    if(renderer == this->renderer.get()){
                        this->renderer.leave();
                        _render_composition(renderer, x, y, dpiK);
                    } else {
                        this->renderer.leave();
                        _render_default(renderer, x, y, dpiK);
                    } break;
                    
                case RenderMode::ColorSchemeMode:
                    _render_color_scheme(renderer, x, y, dpiK);
                    break;
                    
                default:
                    _render_default(renderer, x, y, dpiK);
                    break;
            }
        if(renderMode.data != CompositionMode && !inComposition)
            _renderEmbedded(id, renderer, x, y, dpiK, _RenderEmbeddedMode::_renderOnlyNextInZ);

        renderMode.leave();
#ifdef LANUI_DEBUG_MODE
        if(!inComposition){
            //SDL_SetRenderDrawColor(renderer, 255, 200, 200, 10);
            //SDL_RenderFillRectF(renderer, &sizeBuffer.get());
            SDL_SetRenderDrawColor(renderer, 20, 255, 200, 200);
            SDL_RenderDrawRectF(renderer, &sizeBuffer.data);
        }
#endif
    }
    if(!inComposition) _renderEmbedded(id, renderer, x, y, dpiK, _RenderEmbeddedMode::_renderOnlyNextInX_Y);
}

Object& Object::set_renderer_callback(VoidCallback callback){
    using_renderer_callback = true;
    renderMode.set(RenderMode::CallbackMode);
    renderer_callback.set(callback);
    return (*this);
}
 
void Object::_run_default_animation(){
    if(renderMode.get() != CompositionMode){
        renderMode.leave();
        default_animation.hold();
        if(default_animation.data._using){
            if(delay >= default_animation.data.delay){
                delay = 0;
                default_animation.data._using =
                default_animation.data.callback();
            } delay++;
        } default_animation.leave();
        _run_others_default_animation();
    } else
        renderMode.leave();
}

Object& Object::set_default_animation(const FrameCount delay, BoolCallback callback){
    default_animation.hold();
    default_animation.data._using = true;
    default_animation.data.delay = delay;
    if(delay<0) Core::log(Core::Error, "animation::delay must to be >= 0.");
    default_animation.data.callback = callback;
    default_animation.leave();
    return (*this);
}

void Object::_start_composition_mode(SDL_Renderer* renderer, const float dpiK){
    _generate_canvas(renderer, compositionCanvas, dpiK);
    _clear_canvas(renderer, compositionCanvas);
    compositionBuffer.hold();
    size.hold();
    padding.hold();
    compositionBuffer.data.push(size.data.x);
    compositionBuffer.data.push(size.data.y);
    size.data.x = size.data.y = 0;
    compositionBuffer.data.push(padding.data.top);
    compositionBuffer.data.push(padding.data.bottom);
    compositionBuffer.data.push(padding.data.left);
    compositionBuffer.data.push(padding.data.right);
    padding.data = {0,0,0,0};
    padding.leave();
    size.leave();
    compositionRendererTargetBuffer = SDL_GetRenderTarget(renderer);
    compositionBuffer.leave();
    SDL_SetRenderTarget(renderer, compositionCanvas.get());
    compositionCanvas.leave();
}

void Object::_stop_composition_mode(SDL_Renderer * renderer){
    compositionBuffer.hold();
    size.hold();
    padding.hold();
    padding.data.right = compositionBuffer.data.top();
    compositionBuffer.data.pop();
    padding.data.left = compositionBuffer.data.top();
    compositionBuffer.data.pop();
    padding.data.bottom = compositionBuffer.data.top();
    compositionBuffer.data.pop();
    padding.data.top = compositionBuffer.data.top();
    compositionBuffer.data.pop();
    
    size.data.y = compositionBuffer.data.top();
    compositionBuffer.data.pop();
    size.data.x = compositionBuffer.data.top();
    compositionBuffer.data.pop();
    padding.leave();
    size.leave();
    compositionBuffer.leave();
    SDL_SetRenderTarget(renderer, compositionRendererTargetBuffer);
}

Object& Object::compose(const unsigned int id, SDL_Renderer * renderer, const float dpiK){
#ifdef LANUI_DEBUG_MODE
    Core::log(Core::Warning, ("Composing object [" + std::to_string((long long)this) + "]. (if you use the compositionMode all interactive objects embedded in Z from this object will be disabled).").c_str());
#endif
    
    this->renderer = renderer;
    _compile(renderer, dpiK);
    _start_composition_mode(renderer, dpiK);
    _lock_renderer_in_bounds(id, renderer, dpiK);
    _render(id, renderer, 0, 0, dpiK, true);
    _unlock_renderer_from_bounds(id, renderer);
    
    // reset x & y to renderer embedded objects correctly
    sizeBuffer.hold();
    sizeBuffer.data.x = sizeBuffer.data.y = 0;
    sizeBuffer.leave();
    size.hold();
    size.data.x = size.data.y = 0;
    size.leave();
    
    _renderEmbedded(id, renderer, 0, 0, dpiK, _RenderEmbeddedMode::_renderOnlyNextInZ);
    _stop_composition_mode(renderer);
    return (*this);
}

Object& Object::compose_canvas(const unsigned int id, SDL_Renderer * renderer, const float dpiK){
#ifdef LANUI_DEBUG_MODE
    Core::log(Core::Message, ("Composing canvas of object [" + std::to_string((long long)this) + "]. (make sure to use CanvasCompositionMode).").c_str());
#endif
    this->renderer = renderer;
    _start_composition_mode(renderer, dpiK);
    _compile(renderer, dpiK);
    _lock_renderer_in_bounds(id, renderer, dpiK);
    _render(id, renderer, 0, 0, dpiK, true);
    _unlock_renderer_from_bounds(id, renderer);
    // reset x & y to renderer embedded objects correctly
    sizeBuffer.hold();
    sizeBuffer.data.x = sizeBuffer.data.y = 0;
    sizeBuffer.leave();
    size.hold();
    size.data.x = size.data.y = 0;
    size.leave();
    _stop_composition_mode(renderer);
    return (*this);
}

Object& Object::export_composition_as_PNG(SDL_Renderer * renderer, const char * filename){
    compositionCanvas.hold();
    if(compositionCanvas.data){
        int width, height;

        SDL_QueryTexture(compositionCanvas.data, NULL, NULL, &width, &height);

        // Masks are needed to allow alfa channel
        Uint32 rmask, gmask, bmask, amask;
       #if SDL_BYTEORDER == SDL_BIG_ENDIAN
           rmask = 0xff000000;
           gmask = 0x00ff0000;
           bmask = 0x0000ff00;
           amask = 0x000000ff;
       #else
           rmask = 0x000000ff;
           gmask = 0x0000ff00;
           bmask = 0x00ff0000;
           amask = 0xff000000;
       #endif

        SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
                
        SDL_Texture* target = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, compositionCanvas.data);
        
        SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
        
        IMG_SavePNG(surface, filename);
        
        SDL_FreeSurface(surface);
        
        SDL_SetRenderTarget(renderer, target);
        
#ifdef LANUI_DEBUG_MODE
    Core::log(Core::Message, ("The composition [" + std::to_string((long long)compositionCanvas.data) + "] will be destroyed after exported to avoid renderer errors.").c_str());
#endif
        
        } else {
        Core::log(Core::Warning, "Object::export_composition_as_PNG: empty compositions cannot be exported");
    }
    compositionCanvas.leave();
    _free_canvas(compositionCanvas);
    return (*this);
}

void Object::_disable_renderer(SDL_Renderer * renderer){
    const SDL_Rect zeroRect = {0,0,0,0};
    SDL_RenderSetViewport(renderer, &zeroRect);
}

void Object::_enable_renderer(SDL_Renderer * renderer){
    SDL_RenderSetViewport(renderer, nullptr);
}
