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
#include <unicode/unistr.h>
#include "../Object/Object.hpp"
#include "../Paragraph/Paragraph.hpp"

class TextField : public InterativeObject {
    Semaphore<bool> activated;
    std::stringstream stream;
    icu::UnicodeString uniSource;
    Semaphore<std::wstring> wideSource;
    Semaphore<std::string> source, inputBuffer;
    Semaphore<std::string> inputStyle, placeholderStyle;
    std::string placeholder;
    class TextSurface : public Paragraph{
    } textSurface;
    void _init(Semaphore<std::string>&source, const std::string plabeholder = "");
    public:
    TextField(Semaphore<std::string>&source, const std::string plabeholder = "");
    TextField(TextField const & other);
    void _sync_strings();
    void _compile_source();
    void _compile(Renderer * renderer, const float dpiK) override;
    void _handle_events(Event & event, const float dpiK, const bool no_focus) override;
    TextField& set_input_style(const std::string);
    TextField& set_placeholder_style(const std::string);
    ~TextField();
};

#endif /* TextField_hpp */
