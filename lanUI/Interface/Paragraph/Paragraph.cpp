//
//  Paragraph.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Paragraph.hpp"
#include <iostream>
#include <regex>

Paragraph& Paragraph::from_stringstream(std::stringstream& stream, Wrapper::Mode mode, int fileldsCount){
    Wrapper wrapper(Wrapper(mode, fileldsCount));
    from_stringstream(stream, wrapper);
    return(*this);
}

Paragraph& Paragraph::from_stringstream(std::stringstream& stream, Wrapper wraper){
    std::regex hintREG("^\\\\[[:alnum:](,)_:]+$");
    std::string line, buffer;
    int wordCount(0);
    Font::Style style = Font::Regular;
    unsigned int size = 18;
    
    while (!stream.eof()) {
        stream >> buffer;
        if(!std::regex_match(buffer, hintREG)){
            wordCount++;
            _add_word((line.empty() ? buffer : " "+buffer).c_str(), style, size);
            if(wraper.fieldsCount){
                line+=buffer+" ";
                switch (wraper.mode) {
                    case Wrapper::Char:
                        if(!(line.size() < wraper.fieldsCount)) {
                            _new_line(words);
                            words.clear();
                            line = "";
                        } break;
                    case Wrapper::Word:
                        if(!(wordCount < wraper.fieldsCount)) {
                            _new_line(words);
                            words.clear();
                            wordCount = 0;
                            line = "";
                        } break;
                    default:
                        break;
                }
            } else {
                _new_line(words);
                words.clear();
                line="";
            }
        } else {
            _parse_hint(buffer, line, style, size);
        }
    } if (!line.empty()) {
        _new_line(words);
    }
    
    fromList(lines);
    words.clear();
    lines.clear();
    return (*this);
}

void Paragraph::_parse_hint(const std::string src, std::string & line , Font::Style &style, unsigned int &size){
    std::regex sizeREG("^\\\\size:\\d+$");
    std::regex rgbREG("^\\\\color:rgb\\(\\d{1,3},\\d{1,3},\\d{1,3}\\)$");
    std::regex rgbaREG("^\\\\color:rgba\\(\\d{1,3},\\d{1,3},\\d{1,3},\\d{1,3}\\)$");
    std::regex newlnREG("^\\\\newln$");
    std::regex regularREG("^\\\\regular$");
    std::regex boldREG("^\\\\bold$");
    std::regex boldObliqueREG("^\\\\boldOblique$");
    std::regex extraLightREG("^\\\\extraLight$");
    std::regex obliqueREG("^\\\\oblique$");
    std::regex condensed_BoldREG("^\\\\condensed_Bold$");
    std::regex condensed_BoldObliqueREG("^\\\\condensed_BoldOblique$");
    std::regex condensed_ObliqueREG("^\\\\condensed_Oblique$");
    std::regex condensedREG("^\\\\condensed$");
    
    if(std::regex_match(src, sizeREG)){
        sscanf(src.c_str(), "\\size:%d", &size);
    } else if(std::regex_match(src, rgbREG)){
        short rgb[3];
        sscanf(src.c_str(), "\\color:rgb(%hd,%hd,%hd)", &rgb[0], &rgb[1],&rgb[2]);
        textColor = Colors::fromRGBA(rgb[0], rgb[1], rgb[2]);
    } else if(std::regex_match(src, rgbaREG)){
        short rgba[4];
        sscanf(src.c_str(), "\\color:rgba(%hd,%hd,%hd,%hd)", &rgba[0], &rgba[1],&rgba[2],&rgba[3]);
        textColor = Colors::fromRGBA(rgba[0], rgba[1],rgba[2],rgba[3]);
    } else if(std::regex_match(src, newlnREG)){
        if(!words.empty()) {
            _new_line(words);
            line.clear();
            words.clear();
        }
    } else if(std::regex_match(src, regularREG)){
        style = Font::Regular;
    } else if(std::regex_match(src, boldREG)){
        style = Font::Bold;
    } else if(std::regex_match(src, boldObliqueREG)){
        style = Font::BoldOblique;
    } else if(std::regex_match(src, extraLightREG)){
        style = Font::ExtraLight;
    } else if(std::regex_match(src, obliqueREG)){
        style = Font::Oblique;
    } else if(std::regex_match(src, condensed_BoldREG)){
        style = Font::Condensed_Bold;
    } else if(std::regex_match(src, condensed_BoldObliqueREG)){
        style = Font::Condensed_BoldOblique;
    } else if(std::regex_match(src, condensed_ObliqueREG)){
        style = Font::Condensed_Oblique;
    } else if(std::regex_match(src, condensedREG)){
        style = Font::Condensed;
    } else {
        Core::log(Core::Warning, (std::string("Unknow Paragraph hint: \"" + src + "\"")).c_str());
    }
}

void Paragraph::_add_word(const char * str, Font::Style style, const unsigned int size){
    Text * word = new Text(str);
    word->set_foreground_color(textColor);
    word->font.set_style(style);
    word->fontVirtualSize.set(size);
    word->tryCompile();
    words.push_back(word);
}

void Paragraph::_new_line(std::list<Object *> words){
    HStack * line = new HStack;
    line->fromList(words);
    lines.push_back(line);
}
