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
    Semaphore<bool> activated, isSecret;
    Semaphore<UTF8CharList> inputBuffer;
    UTF8CharList secretStrBuffer;
    ssize_t input_size_change, input_size;
    int8_t cursor_change_flag;
    
    void _default_on_click();
    
    void _default_on_focus_lost();
    
    VoidCallback on_empty_callback;
    VoidCallback on_not_empty_callback;
    VoidCallback on_change_callback;
    VoidCallback on_submit_callback;
    VoidCallback on_delete_callback;
    
    typedef enum {
        OnEmpty,
        OnNotEmpty,
        OnChange,
        OnSubmit,
        OnDelete,
        BSTextFieldCallbackTotal
    } BSTextFieldCallback;
    
    struct BSTextFieldCallbackWrapper {
        bool all[BSTextFieldCallbackTotal];
        bool onEmptyTriggered{false}, onNotEmptyTriggered{false};
    };
    
    Semaphore<BSTextFieldCallbackWrapper> BSTextFieldCallbacks;
    
    std::string secretStr;
    UTF8Char secretStrMask;
    Semaphore<std::string> data;
    std::string placeholder;
    
public:
    
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
        
        void _renderEmbedded(const unsigned int, SDL_Renderer*, const float x, const float y, float dpiK, _RenderEmbeddedMode mode = _RenderEmbeddedMode::_renderAllNexts) override;
        
        void _render(const unsigned int, SDL_Renderer*, float x, float y, const float dpiK, bool iComposition) override;
        
    } textSurface;
    
    void _init(const std::string plabeholder = "");
    
    void __default_on_selected_callback();
    
    void __default_on_unselected_callback();
    
    void __default_on_copy_callback();

    void __default_on_cut_callback();
    
    void __default_on_paste_callback();
    
    void _sync_strings();
    
    void _handle_events(Event & event, const float dpiK, const bool no_focus = false) override;
    
    void _refresh_cursor(const float dpiK);
    
    void _compute_cursor_position(Renderer* );
    
    void _compile(Renderer*, const float dpiK) override;
    
    std::string get_data();
    
    size_t get_data_length();
    
    BSTextField& secret(bool const isSecret, const UTF8Char mask = "•");
    
    BSTextField& on_empty(VoidCallback);
    
    BSTextField& on_not_empty(VoidCallback);
    
    BSTextField& on_change(VoidCallback);
    
    BSTextField& on_submit(VoidCallback);
        
    BSTextField& on_delete(VoidCallback);
    
    BSTextField(const std::string placeholder);
    
    BSTextField(BSTextField const & other);
};

#endif /* TextField_hpp */
