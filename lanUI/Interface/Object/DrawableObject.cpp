//
//  Object.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Object.hpp"
#include "../Window/WindowManager/WindowManager.hpp"
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
        WindowManager::log(WindowManager::Warning, (std::string("Unable to load image source from path: ") + source).c_str());
    } DrawableObjectsData::surfacesCache.leave();
    return surfc;
}

void Object::_compose_embedded(SDL_Renderer* renderer, const float dpiK, const int32_t windowId){
    if(nextInX.get()) {
        nextInX.leave();
        nextInX.data->_compose(renderer, dpiK, windowId);
    } else nextInX.leave();
    
    if(nextInY.get()){
        nextInY.leave();
        nextInY.data->_compose(renderer, dpiK, windowId);
    } else nextInY.leave();
    
    if(nextInZ.get()) {
        nextInZ.leave();
        nextInZ.data->_compose(renderer, dpiK, windowId);
    } else nextInZ.leave();
}

void Object::_composer_routine_prepare_params(){
    size.hold();
    composerParams.w = size.data.w ? size.data.w : 1;
    composerParams.h = size.data.h ? size.data.h : 1;
    size.leave();
    
    if(composerParams.w < composerParams.h){
        composerParams.lowerSide = composerParams.w;
        composerParams.greaterSide = composerParams.h;
    } else {
        composerParams.lowerSide = composerParams.h;
        composerParams.greaterSide = composerParams.w;
    }
    
    composerParams.context = cairoContext;
}

void Object::_composer_prepare_masks(std::list<Mask*>& masks){
    for(Mask* mask : masks){
        cairo_push_group(cairoContext);
        mask->set_params(cairoContext, composerParams.w, composerParams.h, composerParams.scale_param, composerParams.lowerSide, composerParams.greaterSide);
    }
}

void Object::_composer_apply_masks(std::list<Mask*>& masks){
    for(Mask* mask : masks){
        cairo_pop_group_to_source(cairoContext);
        mask->apply();
    }
}

void Object::_composer_routine(SDL_Renderer* renderer){
    if(using_composer_callback){
        // generate surface
        if(!isSurfaceValid.get()){
            _generate_surface();
            isSurfaceValid.data = surface.data;
        } else {
            SDL_FillRect(surface.data, NULL, 0x000000);
        } isSurfaceValid.leave();
        
        // create cairo context
        if((cairoContext = cairosdl_create(surface.get()))){
            obj_dpiK.hold();
            _composer_routine_prepare_params();
            
            // scale cairo
            if(!composerManuallyScaleCairo.get())
                cairo_scale(cairoContext, composerParams.greaterSide*obj_dpiK.data, composerParams.greaterSide*obj_dpiK.data);
            composerManuallyScaleCairo.leave();
            obj_dpiK.leave();
            
            // prepare masks environment (cairo push groups)
            composerMasks.hold();
            _composer_prepare_masks(composerMasks.data);
            
            // draw to surface
            composer_callback.get()();
            composer_callback.leave();
            
            // apply masks (cairo pop groups to surface)
            _composer_apply_masks(composerMasks.data);
            composerMasks.leave();
            
            // destroy cairo and prepare object for renderization
            cairosdl_destroy(cairoContext);
            _free_canvas(canvas);
            canvas.set(SDL_CreateTextureFromSurface(renderer, surface.data));
            surface.leave();
            this->renderer.set(renderer);
        } else
            this->renderer.set(nullptr);
        set_render_mode(ImageMode);
    }
}

void Object::composer_crop_surface(const int x, const int y, const int nw, const int nh){
    if(surface.data){
        Surface * new_surface;
        if((new_surface = SDL_CreateRGBSurface(0, nw, nh, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, CAIROSDL_AMASK))){
            SDL_Rect rect = {x,y,nw,nh};
            SDL_BlitSurface(surface.data, &rect, new_surface, nullptr);
            SDL_FreeSurface(surface.data);
            surface.data = new_surface;
        }
    }
}

bool Object::composer_set_foreground_pattern_as_source(CairoContext * context){
    bool success = false;
    if(foregroundPattern.get()){
        cairo_set_source(context, foregroundPattern.data);
        success = true;
    } foregroundPattern.leave();
    return success;
}

bool Object::composer_set_background_pattern_as_source(CairoContext * context){
    bool success = false;
    if(backgroundPattern.get()){
        cairo_set_source(context, backgroundPattern.data);
        success = true;
    } backgroundPattern.leave();
    return success;
}

double Object::composer_get_cairo_pixel_size(){
    double dx{1}, dy{1};
    cairo_device_to_user_distance(cairoContext, &dx, &dy);    
    return dx < dy ? dy : dx;
}

void Object::_compose(SDL_Renderer* renderer, const float dpiK, const int32_t windowId){
    if(wasCompiled.get())
        wasCompiled.data = dpiK == obj_dpiK.get_copy();
    wasCompiled.leave();
    obj_dpiK.set(dpiK);
    if(!wasCompiled.get()){
        _composer_routine(renderer);
        wasCompiled.data = true;
    } wasCompiled.leave();
    _compose_embedded(renderer, dpiK, windowId);
}

Object& Object::composer_push_mask(Mask* mask){
    composerMasks.get().push_back(mask);
    composerMasks.leave();
    return (*this);
}

Object& Object::composer_push_front_mask(Mask* mask){
    composerMasks.get().push_front(mask);
    composerMasks.leave();
    return (*this);
}

Object& Object::composer_pop_last_mask(){
    if(!composerMasks.get().empty()){
        composerMasks.data.pop_back();
    } composerMasks.leave();
    return (*this);
}

Object& Object::composer_pop_first_mask(){
    if(!composerMasks.get().empty()){
        composerMasks.data.pop_front();
    } composerMasks.leave();
    return (*this);
}

Object& Object::composer_manually_scale_cairo(){
    composerManuallyScaleCairo.set(true);
    return (*this);
}

Object& Object::set_composer_callback(VoidCallback callback){
    wasCompiled.set(false);
    using_composer_callback = true;
    composer_callback.set(callback);
    return (*this);
}

void Object::_render_precomposition(SDL_Renderer * renderer, float x, float y, const float dpiK){
    sizeBuffer.hold();
    SDL_RenderCopyExF(renderer, precompositionCanvas.get(), NULL, &sizeBuffer.data, angle.get(), NULL, SDL_FLIP_NONE);
    precompositionCanvas.leave();
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
    SDL_RenderCopyExF(renderer, canvas.get(), NULL, &sizeBuffer.data, angle.get(), NULL, SDL_FLIP_NONE);
    canvas.leave();
    angle.leave();
    sizeBuffer.leave();
}

void Object::_render_default(SDL_Renderer * renderer, float x, float y, const float dpiK) {
    sizeBuffer.hold();
    SDL_SetRenderDrawColor(renderer,
                           Colors::Primary.r,
                           Colors::Primary.g,
                           Colors::Primary.b,
                           Colors::Primary.a);
    SDL_RenderFillRectF(renderer, &sizeBuffer.data);
    SDL_SetRenderDrawColor(renderer,
                           Colors::Secondary.r,
                           Colors::Secondary.g,
                           Colors::Secondary.b,
                           Colors::Secondary.a);
    SDL_RenderDrawRectF(renderer, &sizeBuffer.data);
    SDL_RenderDrawLineF(renderer, sizeBuffer.data.x, sizeBuffer.data.y, sizeBuffer.data.x + sizeBuffer.data.w, sizeBuffer.data.y + sizeBuffer.data.h);
    SDL_RenderDrawLineF(renderer, sizeBuffer.data.x, sizeBuffer.data.y + sizeBuffer.data.h, sizeBuffer.data.x + sizeBuffer.data.w, sizeBuffer.data.y);
    sizeBuffer.leave();
}

Object& Object::set_render_mode(RenderMode mode){
    renderMode.set(mode);
    return (*this);
}

void Object::_free_surface(Semaphore<Surface *> & surfc){
    if (surfc.get()){
        SDL_FreeSurface(surfc.data);
    }
    surfc.data = nullptr;
    surfc.leave();
}

void Object::_free_canvas(Semaphore<Texture *>& canvas){
    if (canvas.get()){
        SDL_DestroyTexture(canvas.data);
    }
    canvas.data = nullptr;
    canvas.leave();
}

void Object::_generate_canvas(Renderer * renderer, Semaphore<Texture *>& canvas, const float dpiK){
    canvas.hold();
    size.hold();
    canvas.data = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.data.w * dpiK, size.data.h * dpiK);
    SDL_SetTextureBlendMode(canvas.data, SDL_BLENDMODE_BLEND);
    canvas.leave();
    size.leave();
}

void Object::_generate_surface(){
    _free_surface(surface);
    size.hold();
    obj_dpiK.hold();
    surface.set(SDL_CreateRGBSurface(0, size.data.w * obj_dpiK.data, size.data.h * obj_dpiK.data, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, CAIROSDL_AMASK));
    obj_dpiK.leave();
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
        _free_canvas(canvas);
        this->canvas.set(SDL_CreateTextureFromSurface(renderer, surfc));
        this->renderer.set(renderer);
        size.set({0,0,(float)surfc->w, (float)surfc->h});
        renderMode.set(RenderMode::ImageMode);
        // ocultes the border / secondary color
        if(reset_backgroundColor)
            backgroundColor.set(Colors::Transparent);
    } else {
        WindowManager::log(WindowManager::Warning, "Unable to load canvas from image source (nullptr)");
    }
    return (*this);
}

Object& Object::set_color_scheme(const Color foreground, const Color background){
    foregroundColor.set(foreground);
    backgroundColor.set(background);
    return (*this);
}

Object& Object::set_foreground_pattern(CairoPattern* pattern){
    foregroundPattern.set(pattern);
    return (*this);
}

Object& Object::set_background_pattern(CairoPattern* pattern){
    backgroundPattern.set(pattern);
    return (*this);
}

Object& Object::set_foreground_color(const Color color){
    foregroundColor.set(color);
    return (*this);
}

Object& Object::set_background_color(const Color color){
    backgroundColor.set(color);
    return (*this);
}

void Object::_render_using_callback(const unsigned int id, SDL_Renderer * renderer, float x, float y, float dpiK){
    rendererParams.renderer = renderer;
    rendererParams.dpik = dpiK;
    rendererParams.window_id = id;
    
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
                        _render_using_callback(id,renderer, x, y, dpiK);
                    } else {
                        _render_default(renderer, x, y, dpiK);
                    } break;
                    
                case RenderMode::PrecompositionMode:
                    if(renderer == this->renderer.get()){
                        this->renderer.leave();
                        _render_precomposition(renderer, x, y, dpiK);
                    } else {
                        this->renderer.leave();
                        _render_default(renderer, x, y, dpiK);
                    } break;
                    
                case RenderMode::CanvasPrecompositionMode:
                    if(renderer == this->renderer.get()){
                        this->renderer.leave();
                        _render_precomposition(renderer, x, y, dpiK);
                    } else {
                        this->renderer.leave();
                        _render_default(renderer, x, y, dpiK);
                    } break;
                    
                default:
                    _render_default(renderer, x, y, dpiK);
                    break;
            }
        if(renderMode.data != PrecompositionMode && !inComposition)
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
    if(!inComposition)
        _renderEmbedded(id, renderer, x, y, dpiK, _RenderEmbeddedMode::_renderOnlyNextInX_Y);
}

Object& Object::set_renderer_callback(VoidCallback callback){
    using_renderer_callback = true;
    renderMode.set(RenderMode::CallbackMode);
    renderer_callback.set(callback);
    return (*this);
}
 
void Object::_run_default_animation(){
    if(renderMode.get() != PrecompositionMode){
        renderMode.leave();
        if(isAnimationBeingUsed){
            if(delay >= animationDelay.get()){
                animationDelay.leave();
                delay = 0;
                isAnimationBeingUsed =
                animationCallback.get()();
                animationCallback.leave();
            } else
                animationDelay.leave();
                delay++;
        }
    } else
        renderMode.leave();
    _run_others_default_animation();
}

Object& Object::set_default_animation(const FrameCount delay, BoolCallback callback){
    isAnimationBeingUsed = (true);
    if(delay<0) WindowManager::log(WindowManager::Error, "animation::delay must to be >= 0.");
    set_animation(delay, callback);
    return (*this);
}

void Object::_start_precomposition_mode(SDL_Renderer* renderer, const float dpiK){
    _generate_canvas(renderer, precompositionCanvas, dpiK);
    _clear_canvas(renderer, precompositionCanvas);
    precompositionBuffer.hold();
    size.hold();
    padding.hold();
    precompositionBuffer.data.push(size.data.x);
    precompositionBuffer.data.push(size.data.y);
    size.data.x = size.data.y = 0;
    precompositionBuffer.data.push(padding.data.top);
    precompositionBuffer.data.push(padding.data.bottom);
    precompositionBuffer.data.push(padding.data.left);
    precompositionBuffer.data.push(padding.data.right);
    padding.data = {0,0,0,0};
    padding.leave();
    size.leave();
    precompositionRendererTargetBuffer = SDL_GetRenderTarget(renderer);
    precompositionBuffer.leave();
    SDL_SetRenderTarget(renderer, precompositionCanvas.get());
    precompositionCanvas.leave();
}

void Object::_stop_precomposition_mode(SDL_Renderer * renderer){
    precompositionBuffer.hold();
    size.hold();
    padding.hold();
    padding.data.right = precompositionBuffer.data.top();
    precompositionBuffer.data.pop();
    padding.data.left = precompositionBuffer.data.top();
    precompositionBuffer.data.pop();
    padding.data.bottom = precompositionBuffer.data.top();
    precompositionBuffer.data.pop();
    padding.data.top = precompositionBuffer.data.top();
    precompositionBuffer.data.pop();
    
    size.data.y = precompositionBuffer.data.top();
    precompositionBuffer.data.pop();
    size.data.x = precompositionBuffer.data.top();
    precompositionBuffer.data.pop();
    padding.leave();
    size.leave();
    precompositionBuffer.leave();
    SDL_SetRenderTarget(renderer, precompositionRendererTargetBuffer);
}

Object& Object::precompose(const unsigned int id, SDL_Renderer * renderer, const float dpiK){
#ifdef LANUI_DEBUG_MODE
    WindowManager::log(WindowManager::Warning, ("Precomposing object [" + std::to_string((long long)this) + "]. (if you use the PrecompositionMode all interactive objects embedded in Z from this object will be disabled).").c_str());
#endif
    
    this->renderer = renderer;
    _compose(renderer, dpiK, 0);
    _start_precomposition_mode(renderer, dpiK);
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
    _stop_precomposition_mode(renderer);
    return (*this);
}

Object& Object::precompose_canvas(const unsigned int id, SDL_Renderer * renderer, const float dpiK){
#ifdef LANUI_DEBUG_MODE
    WindowManager::log(WindowManager::Message, ("Precomposing canvas of object [" + std::to_string((long long)this) + "]. (make sure to use CanvasPrecompositionMode).").c_str());
#endif
    this->renderer = renderer;
    _start_precomposition_mode(renderer, dpiK);
    _compose(renderer, dpiK, 0);
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
    _stop_precomposition_mode(renderer);
    return (*this);
}

Object& Object::export_precomposition_as_PNG(SDL_Renderer * renderer, const char * filename){
    precompositionCanvas.hold();
    if(precompositionCanvas.data){
        int width, height;

        SDL_QueryTexture(precompositionCanvas.data, NULL, NULL, &width, &height);

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
        SDL_SetRenderTarget(renderer, precompositionCanvas.data);
        
        SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
        
        IMG_SavePNG(surface, filename);
        
        SDL_FreeSurface(surface);
        
        SDL_SetRenderTarget(renderer, target);
        
#ifdef LANUI_DEBUG_MODE
    WindowManager::log(WindowManager::Message, ("The precomposition [" + std::to_string((long long)precompositionCanvas.data) + "] will be destroyed after exported to avoid renderer errors.").c_str());
#endif
        
        } else {
        WindowManager::log(WindowManager::Warning, "Object::export_precomposition_as_PNG: empty precompositions cannot be exported");
    }
    precompositionCanvas.leave();
    _free_canvas(precompositionCanvas);
    return (*this);
}

void Object::_disable_renderer(SDL_Renderer * renderer){
    const SDL_Rect zeroRect = {0,0,0,0};
    SDL_RenderSetViewport(renderer, &zeroRect);
}

void Object::_enable_renderer(SDL_Renderer * renderer){
    SDL_RenderSetViewport(renderer, nullptr);
}
