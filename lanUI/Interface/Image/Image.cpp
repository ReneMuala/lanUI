//
//  Image.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 04/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Image.hpp"
#include "../Window/WindowManager/WindowManager.hpp"
#include <thread>
#include <math.h>

BSImage::BSImage(): request(ImageRequestNone){
    set_composer_callback(CallbackExpr(default_composer_callback();));
}

BSImage& BSImage::from_linear_gradient(Point start, Point end, std::list<GradientElement> list){
    semaphore.hold();
    request = LinearGradient;
    ref_points[0] = start;
    ref_points[1] = end;
    this->list = list;
    wasCompiled.set(false);
    semaphore.leave();
    return (*this);
}

BSImage& BSImage::from_radial_gradient(Point c1, double radius1, Point c2, double radius2, std::list<GradientElement> list){
    semaphore.hold();
    request = RadialGradient;
    ref_points[0] = c1;
    ref_points[1] = c2;
    radius[0] = radius1;
    radius[1] = radius2;
    this->list = list;
    wasCompiled.set(false);
    semaphore.leave();
    return (*this);
}
