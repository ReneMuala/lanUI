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
class Image : public DrawableObject {
    Uint32 * pixels;
    int pich;
public:
    Image(){
        DrawableObject();
    }
    typedef SDL_FPoint Point;
    typedef std::pair<Color, double /* start (in 0...100) */> GradientElement;
    /// Linear Gradient Orietantion
    typedef enum {
        Vertical,
        Horizontal
    } LGOrietantion;
    const Color _getGradientFrameColor(GradientElement first, GradientElement second, double progress, double stages);
    Image& fromLinearGradient(LGOrietantion orientation, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window);
    const double _getDistance(Point point1, Point point2);
    Image& fromRadialGradient(Point, double radius, GradientElement first, GradientElement second, Renderer * renderer, SDL_Window* window);
    Image& set_angle(const Angle);
};

#endif /* Image_hpp */
