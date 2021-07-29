//
//  Color.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 26/06/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Color_h
#define Color_h

#include <SDL2/SDL.h>
/// SDL_Color
typedef SDL_Color Color;
/// A basic color scheme
/// Special thanks to https://www.rapidtables.com/web/color/RGB_Color.html
// true colors <3
namespace Colors {
const Color fromRGA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);
const Color fromColorA(const Color, uint8_t a);
const Color Transparent{0,0,0,0};
const Color White = {255,255,255,255};
const Color Black = {0,0,0,255};
const Color Red = {255,0,0,255};
const Color Lime = {0,255,0,255};
const Color Blue = {0,0,255,255};
const Color Yellow = {255,255,0,255};
const Color Cyan_Aqua = {0,255,255,255};
const Color Magenta_Fuchsia = {255,0,255,255};
const Color Silver = {192,192,192,255};
const Color Gray = {128,128,128,255};
const Color Maroon = {128,0,0,255};
const Color Olive = {128,128,0,255};
const Color Green = {0,128,0,255};
const Color Purple = {128,0,128,255};
const Color Teal = {0,128,128,255};
const Color Navy = {0,0,128,255};
const Color Dark_Red = {139,0,0,255};
const Color Brown = {165,42,42,255};
const Color Firebrick = {178,34,34,255};
const Color Crimson = {220,20,60,255};
const Color Tomato = {255,99,71,255};
const Color Coral = {255,127,80,255};
const Color Indian_red = {205,92,92,255};
const Color Light_coral = {240,128,128,255};
const Color Dark_salmon = {233,150,122,255};
const Color Salmon = {250,128,114,255};
const Color Light_salmon = {255,160,122,255};
const Color Orange_red = {255,69,0,255};
const Color Dark_orange = {255,140,0,255};
const Color Orange = {255,165,0,255};
const Color Gold = {255,215,0,255};
const Color Dark_golden_rod = {184,134,11,255};
const Color Golden_rod = {218,165,32,255};
const Color Pale_golden_rod = {238,232,170,255};
const Color Dark_khaki = {189,183,107,255};
const Color Khaki = {240,230,140,255};
const Color Olive2 = {128,128,0,255};
const Color Yellow2 = {255,255,0,255};
const Color Yellow_green = {154,205,50,255};
const Color Dark_olive_green = {85,107,47,255};
const Color Olive_drab = {107,142,35,255};
const Color Lawn_green = {124,252,0,255};
const Color Chart_reuse = {127,255,0,255};
const Color Green_yellow = {173,255,47,255};
const Color Dark_green = {0,100,0,255};
const Color Forest_green = {34,139,34,255};
const Color Lime_green = {50,205,50,255};
const Color Light_green = {144,238,144,255};
const Color Pale_green = {152,251,152,255};
const Color Dark_sea_green = {143,188,143,255};
const Color Medium_spring_green = {0,250,154,255};
const Color Spring_green = {0,255,127,255};
const Color Sea_green = {46,139,87,255};
const Color Medium_aqua_marine = {102,205,170,255};
const Color Medium_sea_green = {60,179,113,255};
const Color Light_sea_green = {32,178,170,255};
const Color Dark_slate_gray = {47,79,79,255};
const Color Teal2 = {0,128,128,255};
const Color Dark_cyan = {0,139,139,255};
const Color Aqua = {0,255,255,255};
const Color Cyan = {0,255,255,255};
const Color Light_cyan = {224,255,255,255};
const Color Dark_turquoise = {0,206,209,255};
const Color Turquoise = {64,224,208,255};
const Color Medium_turquoise = {72,209,204,255};
const Color Pale_turquoise = {175,238,238,255};
const Color Aqua_marine = {127,255,212,255};
const Color Powder_blue = {176,224,230,255};
const Color Cadet_blue = {95,158,160,255};
const Color Steel_blue = {70,130,180,255};
const Color Corn_flower_blue = {100,149,237,255};
const Color Deep_sky_blue = {0,191,255,255};
const Color Dodger_blue = {30,144,255,255};
const Color Light_blue = {173,216,230,255};
const Color Sky_blue = {135,206,235,255};
const Color Light_sky_blue = {135,206,250,255};
const Color Midnight_blue = {25,25,112,255};
const Color Dark_blue = {0,0,139,255};
const Color Medium_blue = {0,0,205,255};
const Color Royal_blue = {65,105,225,255};
const Color Blue_violet = {138,43,226,255};
const Color Indigo = {75,0,130,255};
const Color Dark_slate_blue = {72,61,139,255};
const Color Slate_blue = {106,90,205,255};
const Color Medium_slate_blue = {123,104,238,255};
const Color Medium_purple = {147,112,219,255};
const Color Dark_magenta = {139,0,139,255};
const Color Dark_violet = {148,0,211,255};
const Color Dark_orchid = {153,50,204,255};
const Color Medium_orchid = {186,85,211,255};
const Color Thistle = {216,191,216,255};
const Color Plum = {221,160,221,255};
const Color Violet = {238,130,238,255};
const Color Magenta = {255,0,255,255};
const Color Orchid = {218,112,214,255};
const Color Medium_violet_red = {199,21,133,255};
const Color Pale_violet_red = {219,112,147,255};
const Color Deep_pink = {255,20,147,255};
const Color Hot_pink = {255,105,180,255};
const Color Light_pink = {255,182,193,255};
const Color Pink = {255,192,203,255};
const Color Antique_white = {250,235,215,255};
const Color Beige = {245,245,220,255};
const Color Bisque = {255,228,196,255};
const Color Blanched_almond = {255,235,205,255};
const Color Wheat = {245,222,179,255};
const Color Corn_silk = {255,248,220,255};
const Color Lemon_chiffon = {255,250,205,255};
const Color Light_golden_rod_yellow = {250,250,210,255};
const Color Light_yellow = {255,255,224,255};
const Color Saddle_brown = {139,69,19,255};
const Color Sienna = {160,82,45,255};
const Color Chocolate = {210,105,30,255};
const Color Peru = {205,133,63,255};
const Color Sandy_brown = {244,164,96,255};
const Color Burly_wood = {222,184,135,255};
const Color Tan = {210,180,140,255};
const Color Rosy_brown = {188,143,143,255};
const Color Moccasin = {255,228,181,255};
const Color Navajo_white = {255,222,173,255};
const Color Peach_puff = {255,218,185,255};
const Color Misty_rose = {255,228,225,255};
const Color Lavender_blush = {255,240,245,255};
const Color Linen = {250,240,230,255};
const Color Old_lace = {253,245,230,255};
const Color Papaya_whip = {255,239,213,255};
const Color Sea_shell = {255,245,238,255};
const Color Mint_cream = {245,255,250,255};
const Color Slate_gray = {112,128,144,255};
const Color Light_slate_gray = {119,136,153,255};
const Color Light_steel_blue = {176,196,222,255};
const Color Lavender = {230,230,250,255};
const Color Floral_white = {255,250,240,255};
const Color Alice_blue = {240,248,255,255};
const Color Ghost_white = {248,248,255,255};
const Color Honeydew = {240,255,240,255};
const Color Ivory = {255,255,240,255};
const Color Azure = {240,255,255,255};
const Color Snow = {255,250,250,255};
const Color Dim_gray = {105,105,105,255};
const Color Dark_gray = {169,169,169,255};
const Color Light_gray = {211,211,211,255};
const Color Gainsboro = {220,220,220,255};
const Color White_smoke = {245,245,245,255};
// You may want to change those:
const Color Primary = White;
const Color Secondary = Gray;
}

#endif /* Color_h */
