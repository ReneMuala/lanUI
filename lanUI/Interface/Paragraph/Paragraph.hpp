//
//  Paragraph.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Paragraph_hpp
#define Paragraph_hpp

#include "../Text/Text.hpp"
#include "../List/List.hpp"
#include <list>
#include <regex>
#include <sstream>

/// Lorem ipsum dolor...
const static std::string Lorem = ("Lorem ipsum dolor sit amet consectetur adipisicing elit. Ullam, magni. Ad culpa dolores id, aspernatur soluta quidem distinctio architecto tempore magni eos odio autem a quibusdam! Delectus cum nam iusto.");

class Paragraph : public VStack {
    Color textColor;
public:
    
    struct Wrapper {
        typedef enum {
            Char,
            Word,
            Infty,
        } Mode;
        Mode mode;
        int fieldsCount;
        Wrapper(Mode mode, int fields): mode(mode), fieldsCount(fields){}
    };
    
    Paragraph(){
        textColor = Colors::Black;
        //Core::log(Core::Warning, "The object type \"Paragraph\" is not ready for usage.");
    }
    
    std::list<Object*> words;
    std::list<Object*> lines;
    
    Paragraph& from_stringstream(std::stringstream& , Wrapper::Mode, int fileldsCount);
    
    Paragraph& from_stringstream(std::stringstream& , Wrapper wraper = {Wrapper::Mode::Infty,0});
    
    void _parse_hint(const std::string src, std::string & line ,Font::Style &style, unsigned int &size, bool &noSpace,bool &space);
    void _add_word(const char *, Font::Style style, const unsigned int size);
    void _new_line(std::list<Object*> word);
};

#endif /* Paragraph_hpp */
