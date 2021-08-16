//
//  Object.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 26/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "../../Semaphore.hpp"
#include "../Color/Color.hpp"
#include <SDL2/SDL.h>
#include <list>
#include <functional>

class Object {
    
    void __align_center(float & x, float & y);
    
    void __align_top(float & x, float & y);

    void __align_bottom(float & x, float & y);

    void __align_left(float & x, float & y);
    
    void __align_right(float & x, float & y);
            
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
    
    struct Padding {
        float top, bottom, left, right;
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
        isWindow,
        isDrawable,
        isInteractive,
        totalProperties
    } Properties;
    
    typedef enum {
        SetSize,
        SetPadding,
        totalRequests
    } Requests;
    
//    typedef enum {
//        OtherRoot,
//        VStackRoot,
//        HStackRoot,
//        ZStackRoot,
//    } RootType;
    
//    typedef enum {
//        OpaqueObject,
//        ContainerObject,
//        VStackObject,
//        HStackObject,
//        ZStackObject,
//    } Type;
    
public:
    // base data
    Semaphore<bool> properties[Properties::totalProperties];
    Semaphore<Rect> size;
    Semaphore<Padding> padding;
    Semaphore<ScrollingFactor> scrollingFactor;
    Semaphore<bool> requests[Requests::totalRequests];
    Semaphore<Alignment> aligment;
    Semaphore<bool> usingRootBounds;
    Semaphore<bool> inRootBounds_buffer;
    Semaphore<bool> reloading_disabled;
//    Semaphore<RootType> rootType;
//    Semaphore<Type> type;
    
    // embedded objects & root
    Semaphore<Object*> root;
    Semaphore<Object*> nextInX, nextInY, nextInZ;
    
    /// random access buffer
    Semaphore<Rect> rect_buffer;
    
public:
    virtual Object& operator=(Object&);
    
    /**Reload this object to match with the children size.
     */
    virtual Object& reload(){return (*this);}
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
    /**Sets Scrolling Factor.
     Scrolling Factor changes the original position of an object, it's used in lists to create a scrolling animation.
     */
    Object& set_scrollingFactor(const ScrollingFactor scrollingFactor);
    Object& set_alignment(const Alignment alignment);
    
    Object& updateRoot(Object*);
    Object& embedInX(Object&);
    Object& embedInY(Object&);
    Object& embedInZ(Object&);
    
    
//    Object& switchE(Object&);
//    Object& replaceE(Object&);
    
    bool _inRootBounds(float x, float y);
    
    void _useRootBounds();
    
    typedef enum {
        _renderAllNexts = 0x0,
        _renderOnlyNextInZ = 0x2,
        _renderOnlyNextInX_Y = 0x4,
    } _RenderEmbeddedMode;
    
    void __renderEmbedded_routine(SDL_Renderer*, Object * embedded, const float x, const float y, const float dpiK);
    
    virtual void _renderEmbedded(SDL_Renderer*, const float x, const float y, float dpiK, _RenderEmbeddedMode mode = _RenderEmbeddedMode::_renderAllNexts);
    
    // Fixes object-compatibility issues
    virtual void _render(SDL_Renderer*, float x, float y, float dpiK);
    
    bool _has_focus(const float dpiK);
        
    void _handle_others_routine(Event&, Object*, const float dpiK);
    
    void _handle_others(Event&, const float dpiK);
    
    // Fixes object-compatibility issues
    virtual void _handle(Event&, const float dpiK);
    
    void _run_others_default_animation();
    
    // Fixes object-compatibility issues
    virtual void _run_default_animation();
    
    void _align(float & x, float & y);
    
    void _clear_properties();
    
    Object();
};

class DrawableObject: public Object {
    
    /// images only
    bool withBorder;
    bool withBackground;
        
public:

    /// Object behavior
    typedef enum {
        DefaultMode,
        ImageMode,
        ColorSchemeMode,
    } DrawMode;
    
    struct Animation {
        bool _using;
        FrameCount delay;
        BoolCallback callback;
    };
    
public:
    
    Semaphore<DrawMode> drawMode;
    Semaphore<Texture*> image;
    Semaphore<Renderer*> renderer;
    Semaphore<Angle> angle;
    Semaphore<Color> foregroundColor, backgroundColor, borderColor;
    Semaphore<Animation> default_animation;
    
    DrawableObject();
    
    ~DrawableObject();
    
    // deletes the current loaded image
    void _freeImage();
    
    DrawableObject& fromFile(const char *, Renderer *);
    
    DrawableObject& fromSurface(Surface*, Renderer *, const bool reset_secondaryColor = true);
    
    DrawableObject& fromColorScheme(const Color color = Colors::Primary, const Color color2 = Colors::Secondary);
    
    virtual DrawableObject& set_foreground_color(const Color);
    
    virtual DrawableObject& set_background_color(const Color);
    
    /// set_secondary_color(...)
    DrawableObject& set_border_color(const Color);
    
    void _render__image(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render__colorScheme(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render__default(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render__background(SDL_Renderer*, Rect*);
    
    void _render__border(SDL_Renderer*, Rect*);
    
    // prepare rect_buffer
    void _render_routine(float dpiK);
    
    void _render(SDL_Renderer*, float x, float y, float dpiK) override;
        
    void _run_default_animation() override;
    
    DrawableObject& set_default_animation(const FrameCount delay, BoolCallback);
    
};

/// __InteractiveObject_Container
class __IOContainer : public Object {
public:
    __IOContainer& reload() override{
        static float width(0), height(0);
        if(!reloading_disabled.get() && nextInZ.data){
            width = (nextInZ.data->size.get().w + nextInZ.data->padding.get().left + nextInZ.data->padding.data.right);
            height = (nextInZ.data->size.data.h + nextInZ.data->padding.data.top + nextInZ.data->padding.data.top);
            nextInZ.data->size.leave();
            nextInZ.data->padding.leave();
        } //size.set({0,0,width, height});
        reloading_disabled.leave();
        set_size(width, height);
        return (*this);
    }
    
    void set_content(Object& object){
        embedInZ(object);
        set_size(
                 object.size.get().w + object.padding.get().left + object.padding.data.right,
                 object.size.data.h + object.padding.data.top + object.padding.data.bottom
                 );
        object.size.leave();
        object.padding.leave();
    };
};

class InterativeObject: public __IOContainer {
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
    
    /// MUST BE USED ONLY INSIDE OF CALLBACKS
    ScrollGain scrollGain;
    
    InterativeObject();
    
    void _handle(Event&, const float dpiK) override;
    
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
