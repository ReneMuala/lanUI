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
    Font::Style hint_style = Font::Regular;
    unsigned int hint_size = TextStyles::Default.size;
    bool hint_noSpace = false;
    bool hint_space = false;
    
    while (!stream.eof()) {
        stream >> buffer;
        if(!std::regex_match(buffer, hintREG)){
            wordCount++;
            __line_creation_begin:
            if(!hint_noSpace) line+=" ";
            _add_word((line.empty() ? buffer : (!hint_noSpace ? " " : "")+buffer).c_str(), hint_style, hint_size);
            hint_noSpace = false;
            hint_space = false;
            if(wraper.fieldsCount){
                line+=buffer;
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
            _parse_hint(buffer, line, hint_style, hint_size, hint_noSpace, hint_space);
            if(hint_space){
                buffer = " ";
                goto __line_creation_begin;
            }
        }
    } if (!line.empty()) {
        _new_line(words);
    }
    
    fromList(lines);
    words.clear();
    lines.clear();
    return (*this);
}

void Paragraph::_parse_hint(const std::string src, std::string & line , Font::Style &style, unsigned int &size, bool &noSpace, bool &space){
    std::regex sizeREG("^\\\\size:\\d+$");
    std::regex rgbREG("^\\\\color:rgb\\(\\d{1,3},\\d{1,3},\\d{1,3}\\)$");
    std::regex rgbaREG("^\\\\color:rgba\\(\\d{1,3},\\d{1,3},\\d{1,3},\\d{1,3}\\)$");
    std::regex newlnREG("^\\\\(newln|n)$");
    std::regex regularREG("^\\\\regular$");
    std::regex boldREG("^\\\\(bold|b)$");
    std::regex boldObliqueREG("^\\\\(boldOblique|bo)$");
    std::regex extraLightREG("^\\\\(extraLight|el)$");
    std::regex obliqueREG("^\\\\(oblique|o)$");
    std::regex condensed_BoldREG("^\\\\(condensed_Bold|cb)$");
    std::regex condensed_BoldObliqueREG("^\\\\(condensed_BoldOblique|cbo)$");
    std::regex condensed_ObliqueREG("^\\\\(condensed_Oblique|co)$");
    std::regex condensedREG("^\\\\(condensed|c)$");
    std::regex nospaceREG("^\\\\(nospace|noSpace|ns)$");
    std::regex spaceREG("^\\\\(space|s)$");

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
    } else if(std::regex_match(src, nospaceREG)){
        noSpace = true;
    } else if(std::regex_match(src, spaceREG)){
        noSpace = true;
        space = true;
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
