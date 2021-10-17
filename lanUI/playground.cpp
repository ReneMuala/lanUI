//
//  playground.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"
#include <regex>

#include "Interface/TextField/TextField.hpp"


/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */


#include <unicode/unistr.h>
#include <iostream>

int main(){
    
    
    /*
     The text that the user see isn't the text that he edits;
     
     he should edit the wstring version to avoid utf-8 errors, and that wstring version shoud be syncronized with the string that is the source of the TextField.
     */
    
    
    std::wstring buffer;

    icu::UnicodeString unistring = icu::UnicodeString::fromUTF8("rené");
    
    setlocale(LC_ALL, ".UTF8");
    
    std::wstring wtext = L"ãeńné";
    
    char * text;
    
    size_t len;
           
    len = wcstombs(text, wtext.c_str(), wtext.length()*2);
    
    std::cout << "\'"  <<  text << "\' :: " << len << "," << wtext.length() <<  std::endl;
    
    // --------
    
    return 0;
    
    for(int i = 0 ; i < unistring.length() ; i++){
        buffer += (wchar_t)unistring[i];
    }

    for (auto test : buffer) {
        std::cout << test << ",";
    }

    return 0;
    
    Core olamundo;
    
    Window janela("hello", 400,500);
    
    janela.set_window_clear_color(Colors::White_smoke);
    
    Semaphore<std::string> fieldData;

    auto field = TextField(fieldData, "Username");

    field.set_foreground_color(Colors::White);

    field.set_border_color(Colors::Blue.from_a(100));

    field.set_alignment(Object::Center);

    field.set_size(150, 25);
    
    janela.embedInZ(field);
    
    olamundo.events();
    
    Paragraph test;
    
    std::stringstream stream;
    
    stream << "\\regular \\size:12 \\color:rgb(255,0,0) " << "Hello world";
    
    test.from_stringstream(stream);
    
    janela.embedInZ(test);

    olamundo.events();
}
