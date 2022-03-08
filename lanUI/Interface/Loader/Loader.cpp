//
//  Loader.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 14/01/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include "Loader.hpp"

BSLoader::BSLoader(BSLoader::State state, double radius, double active, double indeterminate): state(state),stage(0),active_speed(active), indeterminate_speed(indeterminate){
    Object();
    set_radius(radius);
    set_interval(90);
    set_weight(5);
    foregroundColor.set(Colors::Primary);
    backgroundColor.set(Colors::Secondary);
    
    set_composer_callback(CallbackExpr(default_composer_callback();));
    set_default_animation(0, CallbackExpr(return default_animation();));
    
    set_render_mode(CallbackMode);
}

BSLoader& BSLoader::set_indeterminate_speed(const double speed){
    this->indeterminate_speed.set(speed);
    return (*this);
}

BSLoader& BSLoader::set_active_speed(const double speed){
    this->active_speed.set(speed);
    return (*this);
}

BSLoader& BSLoader::set_weight(const double weight){
    this->weight.set(weight);
    return (*this);
}

BSLoader& BSLoader::set_interval(const double interval){
    this->interval.set(interval);
    return (*this);
}

BSLoader& BSLoader::set_state(const State state){
    this->state.set(state);
    return (*this);
}

BSLoader& BSLoader::set_radius(const double radius){
    this->radius.set(radius);
    const double diam = (radius*2)+1;
    set_size(diam, diam);
    return (*this);
}
