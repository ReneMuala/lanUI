//
//  Pallete.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 08/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Pallete_h
#define Pallete_h

#include "../Color/Color.hpp"
#include "../../Settings.hpp"

enum PalleteElement {
    Primary,
    Secondary,
    Tertiary,
    Quaternary,
    Quinary,
    Senary,
    Septenary,
    Octonary,
    Nonary,
    Denary,
};

template<unsigned size>
class Pallete {
    
    Color colors[2][size];
    PalleteMode mode;
    
public:
    
    Pallete(): mode(PalleteMode::Light){
        
    }
    
    void set_mode(PalleteMode mode){
        this->mode = mode;
    }
    
    void _set_color(PalleteElement index, PalleteMode mode, const Color color){
        if(mode != All)
            colors[mode][index] = color;
        else {
            colors[Light][index] = color;
            colors[Dark][index] = color;
        }
    }
    
    void set_all(PalleteMode mode, const Color color){
        for(int i = 0 ; i < size ; i++)
            _set_color((PalleteElement)i, mode, color);
    }
    
    void set_element(PalleteElement index, PalleteMode mode, const Color color){
        if(index >= size) {
            throw std::runtime_error("[" + std::string(LUIVersion) + " ERROR #" + "]: \"" + "(Pallete::set_element) invalid index, unsafe operation..." + "\"");
        }
        _set_color(index, mode, color);
    }
    
    Color operator [](PalleteElement index) const {
        if(index >= size) {
            fprintf(stderr, "[%s WARNING #]: \"%s\"\n", LUIVersion, "(Pallete::operator []) invalid index, assuming Primary | 0.");
            return colors[mode][Primary];
        }
        return colors[mode][index];
    }
};

#endif /* Pallete_h */
