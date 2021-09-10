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
#include <vector>
#include <sstream>

class Paragraph : public List {
public:
    struct Wrapper {
        typedef enum {
            Char,
            Word,
        } Mode;
        Mode mode;
        int fieldsCount;
    };
    Paragraph(){
        Core::log(Core::Warning, "The object type \"Paragraph\" is not ready for usage.");
    }
    std::vector<Text*> lines;
    Paragraph& from_stringstream(std::stringstream& , Wrapper wraper = {Wrapper::Mode::Char,0});
};

#endif /* Paragraph_hpp */
