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
    std::stringstream stream;
    Semaphore<UTF8CharList> inputBuffer;
    Semaphore<std::string> source;
    Semaphore<std::string> inputStyle, placeholderStyle;
    std::string placeholder;
    int8_t cursor_change_flag;
    size_t input_size_change, input_size;
    struct TextSurface : public Paragraph{
    public:
        struct Cursor {
            ssize_t line,colummn;
            Rect size;
            Color color;
            bool empty;
            bool hidden;
            bool active;
            Cursor();
        };
        Semaphore<Cursor> cursor;
        void _render(SDL_Renderer*, float x, float y, float dpiK, bool inComposition = false) override;
    } textSurface;
    void _init(Semaphore<std::string>&source, const std::string plabeholder = "");
    
public:
    
    TextField(Semaphore<std::string>&source, const std::string plabeholder = "");
    TextField(TextField const & other);
    void _sync_strings();
    void _compile_source();
    void _compile(Renderer * renderer, const float dpiK) override;
    void _refresh_cursor( const float dpiK);
    void _handle_events(Event & event, const float dpiK, const bool no_focus) override;
    TextField& set_input_style(const std::string);
    TextField& set_placeholder_style(const std::string);
};

#endif /* TextField_hpp */
