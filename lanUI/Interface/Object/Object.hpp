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

#include "../../Primitives/Primitives.hpp"

#include <SDL2/SDL.h>
#include "../../CairoSDL/Cairo.hpp"
#include "../../Types.hpp"
#include <list>
#include <stack>
#include <queue>
#include <functional>
#include "../Mask/Mask.hpp"

#include "../Extensions/Inheritable/Border.hpp"
#include "../Extensions/Padding.hpp"
#include "../Extensions/ScrollingFactor.hpp"
#include "../Extensions/Inheritable/Animation.hpp"


class Object : public
/*
Extensions:
*/ Animated // default animaition

{
    
    /*
     
     -------------------------------------------------------------------------------
     
     SECTION:       Object.
     
     FILE:          Object.cpp
     
     DESCRIPTION:   Handles basic object's data and operations.
     
     -------------------------------------------------------------------------------
     
     */
    
protected:
    
    void __align_center(float & x, float & y);
    
    void __align_top(float & x, float & y);
    
    void __align_bottom(float & x, float & y);
    
    void __align_left(float & x, float & y);
    
    void __align_right(float & x, float & y);
    
public:
    
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
    Semaphore<std::pair<double /* vertical */, double /* horizontal*/>> sizeCompensation;
    Semaphore<Padding> padding;
    Semaphore<ScrollingFactor> scrollingFactor;
    Semaphore<bool> requests[Requests::totalRequests];
    Semaphore<Alignment> aligment;
    Semaphore<bool> usingRootBounds;
    Semaphore<bool> inRootBoundsBuffer;
    Semaphore<bool> hasFocusBuffer;
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
    
    Semaphore<double> obj_dpiK;
    
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
    
    /**
     Sets sizeCompensation values.
     Must to be used before Object::set_size(...)
     */
    Object& set_size_compensation(const float w, const float h);
    
    void _fix_size(const float w, const float h, bool require_composition = true);
    
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
    
    void __renderEmbedded_routine(const unsigned int, SDL_Renderer*, Object * embedded, const float x, const float y, const float dpiK);
    
    virtual void _renderEmbedded(const unsigned int , SDL_Renderer*, const float x, const float y, float dpiK, _RenderEmbeddedMode mode = _RenderEmbeddedMode::_renderAllNexts);
    
    void _lock_renderer_in_bounds(const unsigned int, SDL_Renderer*, float dpiK);
    
    void _unlock_renderer_from_bounds(const unsigned int, SDL_Renderer*);
    
    // prepare rect_buffer
    void _render_routine(float);
    

    bool _has_focus(const float dpiK);
    
    void _handle_others_routine(Event&, Object*, const float dpiK, const bool no_focus);
    
    /// Handle events for nextInX & nextInY
    void _handle_others(Event&, const float dpiK, const bool no_focus = false);
    
    virtual void _on_unselected_default_callback(){};
    
    // Fixes object-compatibility issues
    virtual void _handle_events(Event&, const float dpiK, const bool no_focus = false);
    
    void _run_others_default_animation();
    
    // Fixes object-compatibility issues
    virtual void _run_default_animation();
    
    void _align(float & x, float & y);
    
    void _set_position(const float x, const float y);
    
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
        CallbackMode,
        PrecompositionMode,
        CanvasPrecompositionMode,
    } RenderMode;
    
protected:
    
    FrameCount delay;
    bool using_renderer_callback;
    Semaphore<VoidCallback> renderer_callback;
    
    bool using_composer_callback;
    Semaphore<VoidCallback> composer_callback;
    
public:
    
    CairoContext* cairoContext;
    Semaphore<std::list<Mask*>> composerMasks;
    Semaphore<RenderMode> renderMode;
    Semaphore<bool> wasCompiled, isSurfaceValid, composerManuallyScaleCairo;
    Semaphore<Surface*> surface;
    Semaphore<Texture*> canvas;
    Semaphore<Texture*> precompositionCanvas;
    Semaphore<Renderer*> renderer;
    Semaphore<Angle> angle;
    Semaphore<CairoPattern*> foregroundPattern, backgroundPattern;
    Semaphore<Color> foregroundColor, backgroundColor;
    
    /// Used to save || protect important data during precomposition mode
    Semaphore<std::stack<float>> precompositionBuffer;
    Texture* precompositionRendererTargetBuffer;
    
public:
    
    Object& set_render_mode(RenderMode mode);
    
    void _free_surface(Semaphore<Surface*>&);
    
    void _free_canvas(Semaphore<Texture*>&);
    
    void _generate_canvas(Renderer *, Semaphore<Texture*>&, const float dpiK = 1);
    
    void _generate_surface();
    
    void _clear_canvas(Renderer *, Semaphore<Texture*>&);
    
    static void clear_surfaces_cache();
    
    Object& fromFile(const char *, Renderer *);
    
    Object& fromSurface(Surface*, Renderer *, const bool reset_secondaryColor = true);
    
    Object& set_color_scheme(const Color = Colors::Primary, const Color = Colors::Secondary);
    
    /**
     Sets the pattern to be used as the foreground source by the composer.
     * Does not work with images.
     */
    virtual Object& set_foreground_pattern(CairoPattern*);
    
    /**
     Sets the pattern to be used as the background source by the composer.
     * Does not work with images.
     */
    virtual Object& set_background_pattern(CairoPattern*);
    
    virtual Object& set_foreground_color(const Color);
    
    virtual Object& set_background_color(const Color);
        
    void _compose_embedded(SDL_Renderer* renderer, const float dpiK, const int32_t windowId);
    
    void _composer_routine_prepare_params();
    
    void _composer_prepare_masks(std::list<Mask*>&);
    
    void _composer_apply_masks(std::list<Mask*>&);
    
    virtual void _composer_routine(SDL_Renderer* renderer);
    
    /**
     Crops the actual composer surface.
     
     Note:
     * Must to be used during composition, in a possivle override of _composer_routine(...).
     * The surface must to be Busy (by calling hold(...)).
     */
    void composer_crop_surface(const int x, const int y, const int nw, const int nh);
    
    bool composer_set_foreground_pattern_as_source(CairoContext*);
    
    bool composer_set_background_pattern_as_source(CairoContext*);
    
    double composer_get_cairo_pixel_size();
    
    virtual void _compose(SDL_Renderer* renderer, const float dpiK, const int32_t windowId);
        
    struct ComposerCallbackParams {
        CairoContext* context;
        double w, h, scale_param, lowerSide, greaterSide;
    } composerParams;
        
    Object& composer_push_mask(Mask*);
    
    Object& composer_push_front_mask(Mask*);
    
    Object& composer_pop_last_mask();
    
    Object& composer_pop_first_mask();
    
    Object& composer_manually_scale_cairo();
    
    Object& set_composer_callback(VoidCallback);
    
    void _render_precomposition(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render_image(SDL_Renderer*, float x, float y, const float dpiK);
        
    void _render_default(SDL_Renderer*, float x, float y, const float dpiK);
    
    void _render_using_callback(const unsigned int, SDL_Renderer*, float x, float y, const float dpiK);
    
    virtual void _render(const unsigned int, SDL_Renderer*, float x, float y, float dpiK, bool inComposition = false);
    
    struct RendererCallbackParams {
        SDL_Renderer * renderer;
        float dpik;
        int window_id;
    } rendererParams;
    
    Object& set_renderer_callback(VoidCallback);
    
    Object& set_default_animation(const FrameCount delay, BoolCallback);
    
    void _start_precomposition_mode(SDL_Renderer*, const float dpiK);
    
    void _stop_precomposition_mode(SDL_Renderer*);
    
    Object& precompose(const unsigned int, SDL_Renderer*, const float dpiK);
    
    Object& precompose_canvas(const unsigned int, SDL_Renderer*, const float dpiK);
    
    Object& export_precomposition_as_PNG(SDL_Renderer*, const char * filename);
    
    void _disable_renderer(SDL_Renderer*);
    
    void _enable_renderer(SDL_Renderer*);
    
    Object();
    
    void _delete_tree();
    
    virtual void _delete_custom_data(){};
    
    ~Object();
};

/// __InteractiveObject_Container
class BSIOContainer : public Object {
public:
    
    BSIOContainer& reload() override{
        static float width(0), height(0);
        if(!reloadingDisabled.get() && nextInZ.data){
            width = (nextInZ.data->size.get().w + nextInZ.data->padding.get().left + nextInZ.data->padding.data.right);
            height = (nextInZ.data->size.data.h + nextInZ.data->padding.data.top + nextInZ.data->padding.data.bottom);
            nextInZ.data->size.leave();
            nextInZ.data->padding.leave();
            set_size(width, height);
        }
        reloadingDisabled.leave();
        return (*this);
    }
    
    Object* get_content(){
        Object* content = nullptr;
        
        if(nextInZ.get()){
            content = nextInZ.data;
        } nextInZ.leave();
        return content;
    }
    
    void set_content(Object& object){
        if(!reloadingDisabled.get()){
            reloadingDisabled.leave();
            set_size(
                     object.size.get().w + object.padding.get().left + object.padding.data.right,
                     object.size.data.h + object.padding.data.top + object.padding.data.bottom
                     );
            object.size.leave();
            object.padding.leave();
        } else
            reloadingDisabled.leave();
        embedInZ(object);
        object._useRootBounds();
    };
    
    BSIOContainer(){};
    
    BSIOContainer(Object& any){
        set_content(any);
    };
};

class InteractiveObject: public BSIOContainer {
    
    /*
     
     -------------------------------------------------------------------------------
     
     SUMMARY:       InteractiveObject.
     
     FILE:          InteractiveObject.cpp
     
     DESCRIPTION:   Object that suports events.
     
     -------------------------------------------------------------------------------
     
     */
    
    bool focus_repeated, is_selected;
    Semaphore<bool> was_resized;
    //     bool no_focus_repeated; [became a Object:: property]
    
    // callbacks
    VoidCallback on_focus_gained_callback;
    VoidCallback on_focus_lost_callback;
    VoidCallback on_selected_callback;
    VoidCallback on_unselected_callback;
    VoidCallback on_click_callback;
    VoidCallback on_double_click_callback;
    VoidCallback on_secondary_click_callback;
    VoidCallback on_resized_callback;
    VoidCallback on_mouse_button_down_callback;
    VoidCallback on_mouse_button_up_callback;
    VoidCallback on_key_down_callback;
    VoidCallback on_key_up_callback;
    VoidCallback on_scroll_callback;
    VoidCallback on_drop_begin_callback;
    VoidCallback on_drop_end_callback;
    VoidCallback on_drop_file_callback;
    VoidCallback on_drop_text_callback;
    VoidCallback on_cut_callback;
    VoidCallback on_copy_callback;
    VoidCallback on_paste_callback;
    VoidCallback on_audio_play_callback;
    VoidCallback on_audio_stop_callback;
    VoidCallback on_audio_next_callback;
    VoidCallback on_audio_prev_callback;
    VoidCallback on_audio_mute_callback;
    VoidCallback on_audio_rewind_callback;
    VoidCallback on_audio_fast_forward_callback;
    VoidCallback on_f1_callback;
    VoidCallback on_f2_callback;
    VoidCallback on_f3_callback;
    VoidCallback on_f4_callback;
    VoidCallback on_f5_callback;
    VoidCallback on_f6_callback;
    VoidCallback on_f7_callback;
    VoidCallback on_f8_callback;
    VoidCallback on_f9_callback;
    VoidCallback on_f10_callback;
    VoidCallback on_f11_callback;
    VoidCallback on_f12_callback;
    VoidCallback custom_event_callback;
    
public:
    
    typedef enum {
        OnFocusGained,
        OnFocusLost,
        OnSelected,
        OnUnselected,
        OnClick,
        OnDoubleClick,
        OnSecondaryClick,
        OnResized,
        OnMouseButtonDown,
        OnMouseButtonUp,
        OnKeyDown,
        OnKeyUp,
        OnScroll,
        OnDropBegin,
        OnDropEnd,
        OnDropFile,
        OnDropText,
        OnCut,
        OnCopy,
        OnPaste,
        OnAudioPlay,
        OnAudioStop,
        OnAudioNext,
        OnAudioPrev,
        OnAudioMute,
        OnAudioRewind,
        OnAudioFastForward,
        OnF1,
        OnF2,
        OnF3,
        OnF4,
        OnF5,
        OnF6,
        OnF7,
        OnF8,
        OnF9,
        OnF10,
        OnF11,
        OnF12,
        CustomEvent,
        totalCallBacks,
    } CallBacks;
    
    struct ScrollGain {
        Sint32 vertical, horizontal;
    };
        
    struct CallbacksWrapper {
        bool all[CallBacks::totalCallBacks];
    };
    
    /// may handle events?
    Semaphore<bool> isActive;
    
    Semaphore<CallbacksWrapper> callbacks;
    
    ScrollGain scrollGain;
    
    SDL_Event* this_event;
    
    InteractiveObject();
    
    void _handle_events(Event&, const float dpiK, const bool no_focus) override;
    
    void _on_unselected_default_callback() override;
    
    InteractiveObject& set_size(const float w, const float h) override;
        
    InteractiveObject& on_focus_gained(VoidCallback);
    
    InteractiveObject& on_focus_lost(VoidCallback);
    
    InteractiveObject& on_selected(VoidCallback);
    
    InteractiveObject& on_unselected(VoidCallback);
    
    InteractiveObject& on_resized(VoidCallback);
    
    InteractiveObject& on_click(VoidCallback);
    
    InteractiveObject& on_double_click(VoidCallback);
    
    InteractiveObject& on_secondary_click(VoidCallback);
    
    InteractiveObject& on_mouse_button_down(VoidCallback);
    
    InteractiveObject& on_mouse_button_up(VoidCallback);
    
    InteractiveObject& on_key_down(VoidCallback);
    
    InteractiveObject& on_key_up(VoidCallback);
    
    InteractiveObject& on_scroll(VoidCallback);
    
    InteractiveObject& on_drop_begin(VoidCallback);
    
    InteractiveObject& on_drop_end(VoidCallback);
    
    InteractiveObject& on_drop_file(VoidCallback);
    
    InteractiveObject& on_drop_text(VoidCallback);

    InteractiveObject& on_cut(VoidCallback);

    InteractiveObject& on_copy(VoidCallback);

    InteractiveObject& on_paste(VoidCallback);
    
    InteractiveObject& on_audio_play(VoidCallback);
        
    InteractiveObject& on_audio_stop(VoidCallback);
    
    InteractiveObject& on_audio_next(VoidCallback);
    
    InteractiveObject& on_audio_prev(VoidCallback);
    
    InteractiveObject& on_audio_mute(VoidCallback);
    
    InteractiveObject& on_audio_rewind(VoidCallback);
    
    InteractiveObject& on_audio_fast_forward(VoidCallback);
    
    InteractiveObject& on_f1(VoidCallback);
    
    InteractiveObject& on_f2(VoidCallback);
    
    InteractiveObject& on_f3(VoidCallback);
    
    InteractiveObject& on_f4(VoidCallback);
    
    InteractiveObject& on_f5(VoidCallback);
    
    InteractiveObject& on_f6(VoidCallback);
    
    InteractiveObject& on_f7(VoidCallback);
    
    InteractiveObject& on_f8(VoidCallback);
    
    InteractiveObject& on_f9(VoidCallback);
    
    InteractiveObject& on_f10(VoidCallback);
    
    InteractiveObject& on_f11(VoidCallback);
    
    InteractiveObject& on_f12(VoidCallback);

    InteractiveObject& custom_event(VoidCallback);
    
};

#endif /* Object_hpp */
