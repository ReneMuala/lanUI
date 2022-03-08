//
//  Loader.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 14/01/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Loader_hpp
#define Loader_hpp

#include <stdio.h>
#include <math.h>
#include "../Object/Object.hpp"
#include "../../Semaphore/Semaphore.hpp"

/*
 Loader
 A loader alerts a user to wait for an activity to complete. (https://semantic-ui.com/elements/loader.html#text-loader)
 */
class BSLoader : public Object {
    
public:
        
    typedef enum {
        Active,
        Indeterminate,
        /// hidden
        Disabled,
    } State;
    
    Semaphore<State> state;
    
protected:
    
    Semaphore<double>
    weight, interval,
    active_speed, indeterminate_speed,
    radius;
    double stage;
    
    virtual void default_composer_callback(){
        pixel_size = composer_get_cairo_pixel_size();
        weight.hold();
        cairo_set_line_width(composerParams.context, weight.data*pixel_size);
        interval.hold();
        cairo_arc_negative(composerParams.context, 0.5, 0.5, 0.4, (180.0+stage)*(M_PI/180.0), (180.0+stage+interval.data)*(M_PI/180.0));
        if(!composer_set_background_pattern_as_source(cairoContext)){
            backgroundColor.hold();
            cairo_set_source_rgba (composerParams.context, backgroundColor.data.get_premultiplied_r(), backgroundColor.data.get_premultiplied_g(), backgroundColor.data.get_premultiplied_b(), backgroundColor.data.get_premultiplied_a());
            backgroundColor.leave();
        }
        cairo_stroke(composerParams.context);
        
        cairo_set_line_width(composerParams.context, weight.data*pixel_size);
        weight.leave();
        cairo_arc(composerParams.context, 0.5, 0.5, 0.4, (180.0+stage)*(M_PI/180.0), (180.0+stage+interval.data)*(M_PI/180.0));
        if(!composer_set_foreground_pattern_as_source(cairoContext)){
            foregroundColor.hold();
            cairo_set_source_rgba (composerParams.context, foregroundColor.data.get_premultiplied_r(), foregroundColor.data.get_premultiplied_g(), foregroundColor.data.get_premultiplied_b(), foregroundColor.data.get_premultiplied_a());
            foregroundColor.leave();
        }
        cairo_stroke(composerParams.context);
        interval.leave();
    }

    
    virtual bool default_animation(){
        angle.hold();
        switch (this->state.get()) {
            case Active:
                angle.data+=active_speed.get();
                active_speed.leave();
                break;
            case Indeterminate:
                angle.data+=indeterminate_speed.get();
                indeterminate_speed.leave();
                break;
            default:
                break;
        }
        this->state.leave();
        if(angle.data >360 || angle.data < -360)
            angle.data = 0;
        
        angle.leave();
        return true;
    }
    
private:
    double pixel_size;
public:
    
    BSLoader(State state = Active, double radius = 20, double active = 5, double indeterminate = -2);
    
    BSLoader& set_indeterminate_speed(const double speed);
    
    BSLoader& set_active_speed(const double speed);

    BSLoader& set_weight(const double weight);
    
    BSLoader& set_interval(const double interval);
    
    BSLoader& set_state(const State);
    
    BSLoader& set_radius(const double);
    
};

#endif /* Loader_hpp */
