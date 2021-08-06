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
        
    void __renderEmbedded_routine(SDL_Renderer*, Object * embedded, const float x, const float y, const float dpiK);
    
public:
    /// SDL_FRect
    typedef SDL_FRect Rect;
    struct Padding {
        float top, bottom, left, right;
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
    
    typedef enum {
        OtherRoot,
        VStackRoot,
        HStackRoot,
        ZStackRoot,
    } RootType;
    
public:
    // base data
    Semaphore<bool> properties[Properties::totalProperties];
    Semaphore<Rect> size;
    Semaphore<Padding> padding;
    Semaphore<bool> requests[Requests::totalRequests];
    Semaphore<Alignment> aligment;
    Semaphore<bool> usingRootBounds;
    Semaphore<RootType> rootType;
    
    // embedded objects & root
    Semaphore<Object*> root;
    Semaphore<Object*> nextInX, nextInY, nextInZ;
    
    /// random access buffer
    Semaphore<Rect> rect_buffer;
    
public:
    virtual Object& operator=(Object&);
    
    void _sync_root_size(const float &wDif, const float & hDif);
    virtual Object& set_size(const float w, const float h);
    void _fix_size(const float w, const float h);
    Object& set_relative_size(const float w, const float h, const float w_correction = 0, const float h_corretion = 0);
    Object& set_padding(Padding padding);
    Object& set_alignment(Alignment alignment);
    
    Object& updateRoot(Object*);
    Object& embedInX(Object&);
    Object& embedInY(Object&);
    Object& embedInZ(Object&);
    
    Object& switchE(Object&);
    Object& replaceE(Object&);
    
    bool _inRootBounds(float x, float y);
    
    void _useRootBounds();
    
    /// alias to renderEmbedded(SDL_Renderer*, float x, float y).       ||      Fixes object-compatibility issues
    virtual void _render(SDL_Renderer*, float x, float y, float dpiK);
    
    void _align(float & x, float & y);
    
    void _renderEmbedded(SDL_Renderer*, const float x, const float y, float dpiK);
    
    Object();
};

class DrawableObject: public Object {
    
    /// images only
    bool withBorder;
        
    void _render__image(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render__colorScheme(SDL_Renderer*, float x, float y, const float dpiK);
        
    void _render__default(SDL_Renderer*, float x, float y, const float dpiK);
    
public:
    
    void _render__border(SDL_Renderer*, Rect*);

    typedef SDL_Texture Texture;
    typedef SDL_Surface Surface;
    typedef SDL_Renderer Renderer;
    typedef double Angle;
    
    /// Object behavior
    typedef enum {
        DefaultMode,
        ImageMode,
        ColorSchemeMode,
    } DrawMode;
    
public:
    
    Semaphore<DrawMode> drawMode;
    Semaphore<Texture*> image;
    Semaphore<Renderer*> renderer;
    Semaphore<Angle> angle;
    Semaphore<Color> primaryColor, secondaryColor;
    
    DrawableObject();
    
    ~DrawableObject();
    
    // deletes the current loaded image
    void _freeImage();
    
    DrawableObject& fromFile(const char *, Renderer *);
    
    DrawableObject& fromSurface(Surface*, Renderer *, const bool reset_secondaryColor = true);
    
    DrawableObject& fromColorScheme(const Color color = Colors::Primary, const Color border = Colors::Secondary);
    
    virtual DrawableObject& set_primary_color(const Color);
    
    virtual DrawableObject& set_secondary_color(const Color);
    
    /// set_secondary_color(...)
    DrawableObject& set_border_color(const Color);
    
    void _render(SDL_Renderer*, float x, float y, float dpiK) override;
    
};

class InterativeObject: public Object {
public:
    typedef std::function<void()> VoidCallback;
    typedef SDL_Event Event;
private:
    // callbacks
    VoidCallback on_start_callback;
    VoidCallback on_focus_gained_callback;
    VoidCallback on_focus_lost_callback;
    VoidCallback on_shown_callback;
    VoidCallback on_resized_callback;
    VoidCallback on_click_callback;
    VoidCallback on_double_click_callback;
    VoidCallback on_secondary_click_callback;
    VoidCallback on_mouse_button_down_callback;
    VoidCallback on_mouse_button_up_callback;
    VoidCallback on_key_down_callback;
    VoidCallback on_key_up_callback;
    VoidCallback on_scroll_callback;
    VoidCallback main_activity;
    
public:
    
    typedef enum {
        OnStart,
        OnFocusGained,
        OnFocusLost,
        OnShown,
        OnClick,
        OnDoubleClick,
        OnResized,
        OnMouseButtonDown,
        OnMouseButtonUp,
        OnKeyDown,
        OnKeyUp,
        OnScroll,
        totalCallBacks
    } CallBacks;
    
    Semaphore<bool> callbacks[CallBacks::totalCallBacks];
    
    InterativeObject(VoidCallback mainActivity);

    bool _has_focus();
    
    bool _has_focus(Object*);
    
    void _handle_others_routine(Event&, Object*);
    
    void _handle_others(Event&);
    
    void _handle(Event&);
    
    InterativeObject& on_start(VoidCallback);
    
    InterativeObject& on_focus_gained(VoidCallback);
    
    InterativeObject& on_focus_lost(VoidCallback);
    
    InterativeObject& on_shown(VoidCallback);
    
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
