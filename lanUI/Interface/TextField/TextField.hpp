//
//  TextField.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef TextField_hpp
#define TextField_hpp

#include <stdio.h>
#include <stack>
#include <string>
#include <sstream>
#include "../Object/Object.hpp"
#include "../Paragraph/Paragraph.hpp"
#include "../../lanUTF/lanUTF/UTF8CharList.hpp"

class TextField : public InterativeObject {
    Semaphore<bool> activated;
    Semaphore<UTF8CharList> inputBuffer;
    Semaphore<std::string>* source;
    std::string placeholder;
    size_t input_size_change, input_size;
    int8_t cursor_change_flag;
    struct TextSurface : public Text {
        float horizontalScrollPading;
    public:
        struct Cursor {
            ssize_t line, colummn;
            Rect size;
            float weight, xBuffer, hBuffer;
            Color color;
            bool empty;
            bool hidden;
            bool active;
                        
            Cursor();
            void _render(SDL_Renderer*, float x, float y, const float dpiK);
        };
        
        Semaphore<Cursor> cursor;
                
        TextSurface();
        
        void _renderEmbedded(SDL_Renderer*, const float x, const float y, float dpiK, _RenderEmbeddedMode mode = _RenderEmbeddedMode::_renderAllNexts) override;
        
        void _render(SDL_Renderer*, float x, float y, const float dpiK, bool isComposition) override;
        
    } textSurface;
    
    void _init(Semaphore<std::string>&source, const std::string plabeholder = "");
    
public:
    void _sync_strings();
    void _handle_events(Event & event, const float dpiK, const bool no_focus = false) override;
    void _refresh_cursor(const float dpiK);
    void _compute_cursor_position(Renderer* );
    void _compile(Renderer*, const float dpiK) override;
    TextField(Semaphore<std::string>&source, const std::string placeholder = "");
    TextField(TextField const & other);
};

#endif /* TextField_hpp */
