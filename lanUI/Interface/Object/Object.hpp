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
    
public:
    // base data
    Semaphore<bool> properties[Properties::totalProperties];
    Semaphore<Rect> size;
    Semaphore<Padding> padding;
    Semaphore<bool> requests[Requests::totalRequests];
    Semaphore<Alignment> aligment;
    Semaphore<bool> usingRootBounds;
    
    // embedded objects & root
    Semaphore<Object*> root;
    Semaphore<Object*> nextInX, nextInY, nextInZ;
    
    /// random access buffer
    Semaphore<Rect> rect_buffer;
    
public:
    virtual Object& operator=(Object&);
    
    virtual Object& set_size(const float w, const float h);
    Object& set_relative_size(const float w, const float h, const float w_correction = 0, const float h_corretion = 0);
    Object& set_padding(Padding padding);
    Object& set_alignment(Alignment alignment);
    
    Object& updateRoot(Object*);
    Object& embedInX(Object&);
    Object& embedInY(Object&);
    Object& embedInZ(Object&);
    
    Object& switchE(Object&);
    Object& replaceE(Object&);
    
    bool inRootBounds(float x, float y);
    
    void useRootBounds();
    
    /// alias to renderEmbedded(SDL_Renderer*, float x, float y).       ||      Fixes object-compatibility issues
    virtual void render(SDL_Renderer*, float x, float y);
    
    void __align_center(float & x, float & y);
    
    void __align_top(float & x, float & y);

    void __align_bottom(float & x, float & y);

    void __align_left(float & x, float & y);
    
    void __align_right(float & x, float & y);
    
    void align(float & x, float & y);
    
    void __renderEmbedded_routine(SDL_Renderer*, Object * embedded, const float x, const float y);
    
    void renderEmbedded(SDL_Renderer*, const float x, const float y);
    
    Object();
};

class DrawableObject: public Object {
public:
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
    /// images only
    bool withBorder;
    
    DrawableObject(): image(nullptr), withBorder(false), primaryColor(Colors::Primary), secondaryColor(Colors::Secondary), drawMode(DrawMode::DefaultMode) {
        Object();
        properties[Properties::isDrawable].set(true);
    }
    
    DrawableObject& fromFile(const char *, Renderer *);
    
    DrawableObject& fromSurface(Surface*, Renderer *);
    
    DrawableObject& fromColorScheme(const Color color = Colors::Primary, const Color border = Colors::Secondary);
    
    DrawableObject& set_primary_color(const Color);
    
    DrawableObject& set_secondary_color(const Color);
    
    /// set_secondary_color(...)
    DrawableObject& set_border_color(const Color);
    
    void _render__border(SDL_Renderer*, Rect*);
    
    void render__colorScheme(SDL_Renderer*, float x, float y);
    
    void render__image(SDL_Renderer*, float x, float y);
    
    void render__default(SDL_Renderer*, float x, float y);
    
    void render(SDL_Renderer*, float x, float y) override;
    
};

class InterativeObject: public Object {
    
};

#endif /* Object_hpp */
