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

class TextField : public InterativeObject {
    bool activated;
    Semaphore<std::string>& source;
    std::string placeholder;
    std::stringstream sourceStream;
    
    struct Cursor {
        unsigned long line, colummn;
        std::stack<unsigned long> multipleSelectionElemnts;
        
        bool show;
        Semaphore<Color> color;
        Cursor(): line(0), colummn(0), show(true), color(Colors::Blue){}
    } cusor;
    
public:
    TextField(Semaphore<std::string>& source, const std::string placeholder = "");
};

#endif /* TextField_hpp */
