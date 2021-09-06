//
//  Paragraph.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Paragraph.hpp"
#include <iostream>

Paragraph& Paragraph::from_stringstream(std::stringstream& stream, Wrapper wraper){
    std::string line, buffer;
    int wordCount(0);
    bool shoudCompose(false);
    while (!stream.eof()) {
        stream >> buffer;
        wordCount++;
        if(wraper.fieldsCount){
            // TODO: IGNORE HINT BUFFERS (\n, \bold:$, \condesed, ...)
            line+=buffer+" ";
            switch (wraper.mode) {
                case Wrapper::Char:
                    if(!(line.size() < wraper.fieldsCount)) {
                        std::cout << line << std::endl;
                        line = "";
                    }
                    break;
                case Wrapper::Word:
                    if(!(wordCount < wraper.fieldsCount)) {
                        std::cout << line << std::endl;
                        wordCount = 0;
                        line = "";
                    }
                    break;
                default:
                    break;
            }
        } else {
            line+=buffer+" ";
            std::cout << ": " << line << "\n";
            line="";
        }
    } return (*this);
}
