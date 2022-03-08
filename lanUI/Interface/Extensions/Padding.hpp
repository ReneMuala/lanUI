//
//  Padding.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 20/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Padding_h
#define Padding_h

struct Padding {
    
    float top, bottom, left, right;
    
    Padding(){}
    
    Padding(const float all): top(all),bottom(all),left(all),right(all){}
    
    Padding(const float top_bottom, const float left_right): top(top_bottom), bottom(top_bottom), left(left_right), right(left_right){}
    
    Padding(const float top, const float left_right, const float bottom): top(top), bottom(bottom), left(left_right), right(left_right){}
    
    Padding(const float top, const float right, const float bottom, const float left): top(top), bottom(bottom), left(left), right(right){}
    
    Padding(const Padding &other){
        (*this) = other;
    }
};

typedef Padding Padded;

#endif /* Padding_h */
