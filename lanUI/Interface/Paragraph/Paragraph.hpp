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
#include <string>
#include <unordered_map>

/// Lorem ipsum dolor...
const static std::string Lorem = ("Lorem ipsum dolor sit amet consectetur adipisicing elit. Ullam, magni. Ad culpa dolores id, aspernatur soluta quidem distinctio architecto tempore magni eos odio autem a quibusdam! Delectus cum nam iusto.");

class BSParagraph : public BSVStack {
    struct Hints {
        Font * font;
        Font::Style style;
        unsigned int size;
        Color color;
        bool noSpace;
        bool space;
        /// Current element id
        std::string id;
    } hints;
    std::unordered_map<std::string, Text*> identifiableWords;
    bool empty;
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
    
    BSParagraph(): empty(true){}
    ~BSParagraph() {
        free();
    }
    
    Text * operator[] (std::string id);
    
    std::list<Object*> words;
    std::list<Object*> lines;
    
    BSParagraph& from_stringstream(std::stringstream& , Wrapper::Mode, int fileldsCount);
    
    BSParagraph& from_stringstream(std::stringstream& , Wrapper wraper = {Wrapper::Mode::Infty,0});
    
    void free() override;
    void _parse_hint(const std::string src, std::string & line);
    void _add_word(const char *);
    void _new_line(std::list<Object*> word);
};

#endif /* Paragraph_hpp */
