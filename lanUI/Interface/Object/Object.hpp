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
    Semaphore<Color> primaryColor, secondaryColor;
    
    DrawableObject();
    
    ~DrawableObject();
    
    // deletes the current loaded image
    void _freeImage();
    
    DrawableObject& fromFile(const char *, Renderer *);
    
    DrawableObject& fromSurface(Surface*, Renderer *);
    
    DrawableObject& fromColorScheme(const Color color = Colors::Primary, const Color border = Colors::Secondary);
    
    virtual DrawableObject& set_primary_color(const Color);
    
    virtual DrawableObject& set_secondary_color(const Color);
    
    /// set_secondary_color(...)
    DrawableObject& set_border_color(const Color);
    
    void _render(SDL_Renderer*, float x, float y, float dpiK) override;
    
};

class InterativeObject: public Object {
    
};

#endif /* Object_hpp */
