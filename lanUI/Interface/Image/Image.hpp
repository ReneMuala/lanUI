//
//  Image.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 04/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include <list>
#include <utility>
#include "../Object/Object.hpp"

/** A bitmap image.
 */
class BSImage : public Object {
public:
    
    typedef enum {
        ImageRequestNone,
        LinearGradient,
        RadialGradient,
        AllImageRequests
    } ImageRequest;
    
    typedef SDL_FPoint Point;
    
    typedef std::pair<Color, double /* from 0.0 to 100.0 */> GradientElement;
    
protected:
    
    cairo_pattern_t * pattern;
    
    BSemaphore semaphore;
    Point ref_points[2];
    double radius[2];
    std::list<GradientElement> list;
    ImageRequest request;
    
    virtual void default_composer_callback(){
        switch (request) {
            case LinearGradient:
                pattern = cairo_pattern_create_linear
                (ref_points[0].x/composerParams.greaterSide,
                 ref_points[0].y/composerParams.greaterSide,
                 ref_points[1].x/composerParams.greaterSide,
                 ref_points[1].y/composerParams.greaterSide);
                
                goto gradient_color_add;
                
            case RadialGradient:

                pattern = cairo_pattern_create_radial
                (ref_points[0].x/composerParams.greaterSide,
                 ref_points[0].y/composerParams.greaterSide,
                 radius[0]/composerParams.greaterSide,
                 ref_points[1].x/composerParams.greaterSide,
                 ref_points[1].y/composerParams.greaterSide,
                 radius[1]/composerParams.greaterSide);
                                
                gradient_color_add:
                
                for(GradientElement element : list){
                    cairo_pattern_add_color_stop_rgba
                    (pattern,
                     element.second/100.0,
                     element.first.get_premultiplied_r(),
                     element.first.get_premultiplied_g(),
                     element.first.get_premultiplied_b(),
                     element.first.get_premultiplied_a());
                }
                
                cairo_set_source(composerParams.context, pattern);
                
                cairo_rectangle(composerParams.context, 0, 0, 1, 1);
                                
                cairo_fill(composerParams.context);
                
                break;
                
            default: break;
        }
    }
    
public:
    
    BSImage();
    
    BSImage& from_linear_gradient(Point, Point, std::list<GradientElement>);
    
    BSImage& from_radial_gradient(Point, double , Point, double , std::list<GradientElement>);
    
};

#endif /* Image_hpp */
