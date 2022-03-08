//
//  FontFace.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 22/02/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef FontFace_h
#define FontFace_h

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BBOX_H
#include "../Window/WindowManager/WindowManager.hpp"
#include <string>
#include "../../CairoSDL/Cairo.hpp"
#include "../../Types.hpp"

/**
 FontFace
 * Freetype2 font face wrapper, used in cairo text rendering routines.
 */
class FontFace {
protected:
    FT_Error error;
    FT_Face face;
    bool ready;
public:
    
    FontFace(): ready(false) {};
    
    bool open_face(FT_Library library, const char * filename){
        if((error = FT_New_Face(library, filename, 0, &face))){
            WindowManager::log(WindowManager::Error, (std::string("Unable to open font face, FT_error: ") + std::to_string(error)).c_str());
        } return (ready = true);
    }
    
    bool is_ready(){
        return ready;
    }
    
    FT_Face& get_face(){
        return face;
    }
    
    void set_size(const double size){
        if((error = FT_Set_Char_Size(face, 0, size * 64, 0, 0))){
            WindowManager::log(WindowManager::Error, (std::string("Unable set char size, FT_error: ") + std::to_string(error)).c_str());
        }
    }
    
    FT_UInt get_char_index(const unsigned long charcode){
        return ready ? FT_Get_Char_Index(face, charcode) : 0;
    }
    
    bool load_glyph(const unsigned long charcode, FT_Int32 load_flags = FT_LOAD_NO_BITMAP /* load metrics only*/){
        if((error = FT_Load_Glyph(face, get_char_index(charcode) , load_flags))){
            WindowManager::log(WindowManager::Error, (std::string("Unable load glyph, FT_error: ") + std::to_string(error)).c_str());
        } return (true);
    }
    
    CairoFontFace * new_cairo_font_face(){
        return cairo_ft_font_face_create_for_ft_face(face, 0);
    }
    
    bool done_face(){
        if(ready){
            if((error = FT_Done_Face(face))){
                WindowManager::log(WindowManager::Error, (std::string("Unable to done font face, FT_error: ") + std::to_string(error)).c_str());
            } ready = false;
        } return true;
    }
    
    ~FontFace(){
        done_face();
    }
};

#endif /* FontFace_h */
