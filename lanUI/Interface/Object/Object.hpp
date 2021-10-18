//
//  Object.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 26/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "../../Semaphore/Semaphore.hpp"
#include "../Color/Color.hpp"

#include "../../Core/Graphics/SDL2_rotozoom.hpp"
#include "../../Core/Graphics/SDL2_gfxPrimitives.hpp"

#include <SDL2/SDL.h>
#include <list>
#include <stack>
#include <functional>

class Object {
    
    /*
     
     -------------------------------------------------------------------------------
     
     SECTION:       Object.
     
     FILE:          Object.cpp
     
     DESCRIPTION:   Handles basic object's data and operations.
     
     -------------------------------------------------------------------------------

     */
    
public:
    
    typedef SDL_FRect Rect;
    typedef SDL_Event Event;
    typedef SDL_Texture Texture;
    typedef SDL_Surface Surface;
    typedef SDL_Renderer Renderer;
    typedef double Angle;
    typedef long FrameCount;
    typedef std::function<void()> VoidCallback;
    typedef std::function<bool()> BoolCallback;
    
private:
    
    void __align_center(float & x, float & y);
    
    void __align_top(float & x, float & y);

    void __align_bottom(float & x, float & y);

    void __align_left(float & x, float & y);
    
    void __align_right(float & x, float & y);
    
public:
    
    struct Padding {
        float top, bottom, left, right;
        Padding(){}
        Padding(const float all): top(all),bottom(all),left(all),right(all){}
        Padding(const float top_bottom, const float left_right): top(top_bottom), bottom(top_bottom), left(left_right), right(left_right){}
        Padding(const float top, const float left_right, const float bottom): top(top), bottom(bottom), left(left_right), right(left_right){}
        Padding(const float top, const float right, const float bottom, const float left): top(top), bottom(bottom), left(left), right(right){}
        Padding(const Padding &other){
            (*this) = other;
        }
    };
    
    struct ScrollingFactor {
        float horizontal, vertical;
    };
    
    typedef enum {
        None,
        Center,
        Top,
        Bottom,
        Left,
        Right,
    } Alignment;
    
    typedef enum {
        SetSize,
        SetPadding,
        totalRequests
    } Requests;
    
public:
    
    // base data
    Semaphore<Rect> size;
    Semaphore<Rect> crop;
    Semaphore<Padding> padding;
    Semaphore<ScrollingFactor> scrollingFactor;
    Semaphore<bool> requests[Requests::totalRequests];
    Semaphore<Alignment> aligment;
    Semaphore<bool> usingRootBounds;
    Semaphore<bool> inRootBoundsBuffer;
    Semaphore<bool> isVericallyAfterRootBeginning, isVericallyBeforeRootEnding;
    Semaphore<bool> reloadingDisabled;
    
    /// Object index inside of the root
    Semaphore<size_t> index;

    // embedded objects & root
    Semaphore<Object*> root;
    Semaphore<Object*> nextInX, nextInY, nextInZ;
    
    /// random access buffer
    Semaphore<Rect> sizeBuffer;
    
    // [From interactive object] to allow no_focus events
    bool no_focus_repeated;
    
public:
    
    virtual Object& operator=(Object&);
    
    /**Reload this object to match with the children size.
     */
    virtual Object& reload(){
        return (*this);
    }
    
    /**Enables Reload.
     Reloading is enabled by default on supported objects.
     */
    Object& enable_reloading();
    
    /**Disables Reload.
     */
    Object& disable_reloading();
    
    virtual Object& set_size(const float w, const float h);
    void _fix_size(const float w, const float h);
    Object& set_relative_size(const float w, const float h, const float w_correction = 0, const float h_corretion = 0);
    Object& set_padding(const Padding padding);
    
    /** Sets Scrolling Factor.
     Scrolling Factor changes the original position of an object, it's used in lists to create a scrolling animation.
     */
    Object& set_scrollingFactor(const ScrollingFactor scrollingFactor);
    Object& set_alignment(const Alignment alignment);
    
    Object& updateRoot(Object*);
    Object& embedInX(Object&);
    Object& embedInY(Object&);
    Object& embedInZ(Object&);
    
    bool _inRootBounds(float x, float y);
    
    void _useRootBounds();
    
    typedef enum {
        _renderAllNexts = 0x0,
        _renderOnlyNextInZ = 0x2,
        _renderOnlyNextInX_Y = 0x4,
    } _RenderEmbeddedMode;
    
    void __renderEmbedded_routine(SDL_Renderer*, Object * embedded, const float x, const float y, const float dpiK);
    
    virtual void _renderEmbedded(SDL_Renderer*, const float x, const float y, float dpiK, _RenderEmbeddedMode mode = _RenderEmbeddedMode::_renderAllNexts);
    
    void _lock_renderer_in_bounds(SDL_Renderer*, float dpiK);
    
    void _unlock_renderer_from_bounds(SDL_Renderer*);
    
    // prepare rect_buffer
    void _render_routine(float);
        
    bool _has_focus(const float dpiK);
        
    void _handle_others_routine(Event&, Object*, const float dpiK, const bool no_focus);
    
    /// Handle events for nextInX & nextInY
    void _handle_others(Event&, const float dpiK, const bool no_focus);
    
    // Fixes object-compatibility issues
    virtual void _handle_events(Event&, const float dpiK, const bool no_focus);
    
    void _run_others_default_animation();
    
    // Fixes object-compatibility issues
    virtual void _run_default_animation();
    
    void _align(float & x, float & y);
            
    /*
     
     -------------------------------------------------------------------------------
     
     SECTION:       Object, Drawable.
     
     FILE:          DrawableObject.cpp
     
     DESCRIPTION:   Adds support for renderering and drawing operations.
     
     -------------------------------------------------------------------------------
     
     */
    
public:
    
    /// Object behavior
    typedef enum {
        DefaultMode,
        ImageMode,
        ColorSchemeMode,
        CallbackMode,
        CompositionMode,
        CanvasCompositionMode,
    } DrawMode;
    
    struct Animation {
        bool _using;
        FrameCount delay;
        BoolCallback callback;
    };
    
private:
    
    /// images only
    bool withBorder;
    bool withBackground;
    FrameCount delay;
    bool using_renderer_callback;
    Semaphore<VoidCallback> renderer_callback;
    
public:
    
    Semaphore<DrawMode> drawMode;
    Semaphore<bool> wasCompiled;
    Semaphore<Texture*> canvas;
    Semaphore<Texture*> compositionCanvas;
    Semaphore<Renderer*> renderer;
    Semaphore<Angle> angle;
    Semaphore<Color> foregroundColor, backgroundColor, borderColor;
    Semaphore<Animation> default_animation;
    
    /// Used to save || protect important data during composition mode
    Semaphore<std::stack<float>> compositionBuffer;
    Texture* compositionRendererTargetBuffer;
    
public:
    
    Object& set_draw_mode(DrawMode mode);
    
    void _free_canvas(Semaphore<Texture*>&);
    void _free_canvas();
    void _generate_canvas(Renderer *, Semaphore<Texture*>&, const float dpiK = 1);
    void _clear_canvas(Renderer *, Semaphore<Texture*>&);
    
    Object& fromFile(const char *, Renderer *);
    
    Object& fromSurface(Surface*, Renderer *, const bool reset_secondaryColor = true);
    
    Object& fromColorScheme(const Color color = Colors::Primary, const Color color2 = Colors::Secondary);
    
    virtual Object& set_foreground_color(const Color);
    
    virtual Object& set_background_color(const Color);
    
    /// set_secondary_color(...)
    Object& set_border_color(const Color);
    
    void _compile_embedded(SDL_Renderer* renderer, const float dpiK);
    
    virtual void _compile(SDL_Renderer* renderer, const float dpiK);
    
    void _render_composition(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render_image(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render_color_scheme(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render_default(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render_background(SDL_Renderer*, Rect*);
    
    void _render_border(SDL_Renderer*, Rect*);
    
    void _render_using_callback(SDL_Renderer*, float x, float y, const float dpiK);
    
    virtual void _render(SDL_Renderer*, float x, float y, float dpiK, bool inComposition = false);
    
    SDL_Renderer * param_renderer;
    float param_dpiK;
    
    Object& set_renderer_callback(VoidCallback);
        
    Object& set_default_animation(const FrameCount delay, BoolCallback);
        
    void _start_composition_mode(SDL_Renderer*, const float dpiK);
    
    void _stop_composition_mode(SDL_Renderer*);
    
    Object& compose(SDL_Renderer*, const float dpiK);
    
    Object& compose_canvas(SDL_Renderer*, const float dpiK);
    
    Object& export_composition_as_PNG(SDL_Renderer*, const char * filename);
        
    Object();
    
    ~Object();
};

/// __InteractiveObject_Container
class __IOContainer : public Object {
public:
    
    __IOContainer& reload() override{
        static float width(0), height(0);
        if(!reloadingDisabled.get() && nextInZ.data){
            width = (nextInZ.data->size.get().w + nextInZ.data->padding.get().left + nextInZ.data->padding.data.right);
            height = (nextInZ.data->size.data.h + nextInZ.data->padding.data.top + nextInZ.data->padding.data.bottom);
            nextInZ.data->size.leave();
            nextInZ.data->padding.leave();
            set_size(width, height);
        } //size.set({0,0,width, height});
        reloadingDisabled.leave();
        return (*this);
    }
    
    void set_content(Object& object){
        set_size(
                 object.size.get().w + object.padding.get().left + object.padding.data.right,
                 object.size.data.h + object.padding.data.top + object.padding.data.bottom
                 );
        object.size.leave();
        object.padding.leave();
        embedInZ(object);
        object._useRootBounds();
    };
};

class InterativeObject: public __IOContainer {
    
    /*
     
     -------------------------------------------------------------------------------
     
     SUMMARY:       InteractiveObject.
     
     FILE:          InteractiveObject.cpp
     
     DESCRIPTION:   Object that suports events.
     
     -------------------------------------------------------------------------------
     
     */
    
    bool focus_repeated;
    Semaphore<bool> was_resized;
//     bool no_focus_repeated; [became a Object:: property]
    
    // callbacks
    VoidCallback on_focus_gained_callback;
    VoidCallback on_focus_lost_callback;
    VoidCallback on_click_callback;
    VoidCallback on_double_click_callback;
    VoidCallback on_secondary_click_callback;
    VoidCallback on_resized_callback;
    VoidCallback on_mouse_button_down_callback;
    VoidCallback on_mouse_button_up_callback;
    VoidCallback on_key_down_callback;
    VoidCallback on_key_up_callback;
    VoidCallback on_scroll_callback;
    VoidCallback main_activity_callback;
    
    // may handle events?
    Semaphore<bool> isActive;
   
public:
    
    typedef enum {
        OnFocusGained,
        OnFocusLost,
        OnClick,
        OnDoubleClick,
        OnSecondaryClick,
        OnResized,
        OnMouseButtonDown,
        OnMouseButtonUp,
        OnKeyDown,
        OnKeyUp,
        OnScroll,
        totalCallBacks
    } CallBacks;
    
    struct ScrollGain {
        Sint32 vertical, horizontal;
    };
    
    Semaphore<bool> callbacks[CallBacks::totalCallBacks];
    
    ScrollGain scrollGain;
    
    InterativeObject();
    
    void _handle_events(Event&, const float dpiK, const bool no_focus) override;
    
    InterativeObject& set_size(const float w, const float h) override;

    InterativeObject& main_actiivity(VoidCallback);
    
    InterativeObject& on_focus_gained(VoidCallback);
    
    InterativeObject& on_focus_lost(VoidCallback);
        
    InterativeObject& on_resized(VoidCallback);
    
    InterativeObject& on_click(VoidCallback);
    
    InterativeObject& on_double_click(VoidCallback);
    
    InterativeObject& on_secondary_click(VoidCallback);
    
    InterativeObject& on_mouse_button_down(VoidCallback);
    
    InterativeObject& on_mouse_button_up(VoidCallback);
    
    InterativeObject& on_key_down(VoidCallback);
    
    InterativeObject& on_key_up(VoidCallback);
    
    InterativeObject& on_scroll(VoidCallback);
    
};

#endif /* Object_hpp */
