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

class BSTextField : public InterativeObject {
    Semaphore<bool> activated, hidden;
    Semaphore<UTF8CharList> inputBuffer;
    UTF8CharList hiddenInputBuffer;
    ssize_t input_size_change, input_size;
    int8_t cursor_change_flag;
    
    void _default_on_click();
    void _default_on_focus_lost();
    
    Semaphore<VoidCallback> on_empty_callback;
    Semaphore<VoidCallback> on_not_empty_callback;
    Semaphore<VoidCallback> on_changed_callback;
    Semaphore<VoidCallback> on_submit_callback;
    Semaphore<VoidCallback> on_tab_callback;
    Semaphore<VoidCallback> on_delete_callback;
    
    std::string _hidden_source;
    UTF8Char _hidden_source_mask;
    
public:
    Semaphore<std::string> _source;
    std::string _placeholder;
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
        
        void _render(SDL_Renderer*, float x, float y, const float dpiK, bool iComposition) override;
        
    } textSurface;
    
    void _init(const std::string plabeholder = "");
    
    void __default_on_selected_callback();
    void __default_on_unselected_callback();
    
    void _sync_strings();
    void _handle_events(Event & event, const float dpiK, const bool no_focus = false) override;
    void _refresh_cursor(const float dpiK);
    void _compute_cursor_position(Renderer* );
    void _compile(Renderer*, const float dpiK) override;
    void hide(bool const _hide, const UTF8Char mask = "•");
    BSTextField(const std::string placeholder);
    BSTextField(BSTextField const & other);
};

#endif /* TextField_hpp */
