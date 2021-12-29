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
    Uint32 * pixels;
    int pich;
public:
    
    typedef SDL_FPoint Point;
    typedef std::pair<Color, double /* start (in 0...100) */> GradientElement;
    /// Linear Gradient Orietantion
    typedef enum {
        Vertical,
        Horizontal
    } LGOrietantion;
    const Color _getGradientFrameColor(GradientElement first, GradientElement second, double progress, double stages);
    BSImage& fromLinearGradient(LGOrietantion orientation, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window, uint32_t weight = 1, uint32_t gap = 0);
    const double _getDistance(Point point1, Point point2);
    BSImage& fromRadialGradient(Point, double radius, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window);
    BSImage& set_angle(const Angle);
    void _text_surface_operation();
};

#endif /* Image_hpp */
