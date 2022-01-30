//
//  Paragraph.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Paragraph.hpp"
#include "../Theme/Theme.hpp"
#include <iostream>
#include <regex>

Text * BSParagraph::operator[](std::string id){
    if(identifiableWords.empty())
        Core::log(Core::Warning, "Using Paragraph::operator[] in Paragraph without any id.");
    for(auto idfyWord : identifiableWords){
        if(idfyWord.first == id){
            return idfyWord.second;
        }
    } return nullptr;
}

BSParagraph& BSParagraph::from_string(const std::string streamstr, Wrapper::Mode mode, int fileldsCount){
    std::stringstream stream;
    stream << streamstr;
    Wrapper wrapper(Wrapper(mode, fileldsCount));
    from_stringstream(stream, wrapper);
    return(*this);
}

BSParagraph& BSParagraph::from_stringstream(std::stringstream& stream, Wrapper::Mode mode, int fileldsCount){
    Wrapper wrapper(Wrapper(mode, fileldsCount));
    from_stringstream(stream, wrapper);
    return(*this);
}

BSParagraph& BSParagraph::from_stringstream(std::stringstream& stream, Wrapper wraper){
    static const std::regex hintREG("^%[[:alnum:]{}(,)\\[\\]#_:]+$");
    std::string line, buffer;
    int wordCount(0);
    hints.font = new Font();
    *hints.font =  FontsSharedData::DefaultFonts;
    hints.style = Font::Regular;
    hints.size = TextStyles::Default.size;
    hints.color = Themes::_default.colors.get_text_color<Primary>();
    
    hints.noSpace = false;
    hints.space = false;
    
    while (!stream.eof()) {
        stream >> buffer;
        if(!std::regex_match(buffer, hintREG)){
            if(empty) empty = false;
            wordCount++;
        __line_creation_begin:
            if(!hints.noSpace && !words.empty() && !line.empty()) line+=" ";
            _add_word((line.empty() ? buffer : (!hints.noSpace ? " " : "")+buffer).c_str());
            hints.noSpace = false;
            hints.space = false;
            if(wraper.fieldsCount || wraper.mode == Wrapper::Infty){
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
            _parse_hint(buffer, line);
            if(hints.space){
                buffer = " ";
                goto __line_creation_begin;
            }
        }
    } if (!line.empty()) {
        _new_line(words);
    }
    free();
    fromList(lines);
    words.clear();
    lines.clear();
    delete hints.font;
    return (*this);
}

void BSParagraph::free(){
    BSStack::free();
}

void BSParagraph::_parse_hint(const std::string src, std::string & line){
    static const std::regex idREG("^%\\[[[:alnum:]]+\\]$", std::regex_constants::icase);
    static const std::regex sizeREG("^%(size):\\d+$", std::regex_constants::icase);
    static const std::regex colorRgbREG("^%color:rgb\\(\\d{1,3},\\d{1,3},\\d{1,3}\\)$", std::regex_constants::icase);
    static const std::regex colorRgbaREG("^%color:rgba\\(\\d{1,3},\\d{1,3},\\d{1,3},\\d{1,3}\\)$", std::regex_constants::icase);
    
    static const std::regex colorPrimaryREG("^%color:(Default|Primary|1)$", std::regex_constants::icase);
    static const std::regex colorSecondaryREG("^%color:(Secondary|2)$", std::regex_constants::icase);
    static const std::regex colorTertiaryREG("^%color:(Tertiary|3)$", std::regex_constants::icase);
    static const std::regex colorQuaternaryREG("^%color:(Quaternary|4)$", std::regex_constants::icase);
    static const std::regex colorQuinaryREG("^%color:(Quinary|5)$", std::regex_constants::icase);
    static const std::regex colorSenaryREG("^%color:(Senary|6)$", std::regex_constants::icase);
    static const std::regex colorSeptenaryREG("^%color:(Septenary|7)$", std::regex_constants::icase);
    static const std::regex colorOctonaryREG("^%color:(Octonary|8)$", std::regex_constants::icase);
    static const std::regex colorNonaryREG("^%color:(Nonary|9)$", std::regex_constants::icase);
    static const std::regex colorDenaryREG("^%color:(Denary|10)$", std::regex_constants::icase);
    
    static const std::regex newlnREG("^%(newln|n)$", std::regex_constants::icase);
    
    static const std::regex regularREG("^%(regular|r)$", std::regex_constants::icase);
    static const std::regex boldREG("^%(bold|b)$", std::regex_constants::icase);
    static const std::regex boldObliqueREG("^%(boldOblique|bo)$", std::regex_constants::icase);
    static const std::regex extraLightREG("^%(extraLight|el)$", std::regex_constants::icase);
    static const std::regex obliqueREG("^%(oblique|o)$", std::regex_constants::icase);
    static const std::regex condensed_BoldREG("^%(condensed_Bold|cb)$", std::regex_constants::icase);
    static const std::regex condensed_BoldObliqueREG("^%(condensed_BoldOblique|cbo)$", std::regex_constants::icase);
    static const std::regex condensed_ObliqueREG("^%(condensed_Oblique|co)$", std::regex_constants::icase);
    static const std::regex condensedREG("^%(condensed|c)$", std::regex_constants::icase);
    
    static const std::regex blackREG("^%(black|bk|b2)$", std::regex_constants::icase);
    static const std::regex blackItalicREG("^%(blackItalic|bki|b2i)$", std::regex_constants::icase);
    static const std::regex boldItalicREG("^%(boldItalic|bi)$", std::regex_constants::icase);
    static const std::regex extraLightItalicREG("^%(extraLightItalic|eli)$", std::regex_constants::icase);
    static const std::regex extraBoldREG("^%(extraBold|eb)$", std::regex_constants::icase);
    static const std::regex extraBoldItalicREG("^%(extraBoldItalic|ebi)$", std::regex_constants::icase);
    static const std::regex italicREG("^%(italic|i)$", std::regex_constants::icase);
    static const std::regex lightREG("^%(light|l)$", std::regex_constants::icase);
    static const std::regex lightItalicREG("^%(lightItalic|li)$", std::regex_constants::icase);
    static const std::regex mediumREG("^%(medium|m)$", std::regex_constants::icase);
    static const std::regex mediumItalicREG("^%(mediumItalic|mi)$", std::regex_constants::icase);
    static const std::regex semiboldREG("^%(semiBold|sb)$", std::regex_constants::icase);
    static const std::regex semiboldItalicREG("^%(semiBoldItalic|sbi)$", std::regex_constants::icase);
    static const std::regex thinREG("^%(thin|t)$", std::regex_constants::icase);
    static const std::regex thinItalicREG("^%(thinItalic|ti)$", std::regex_constants::icase);
    
    static const std::regex nospaceREG("^%(nospace|noSpace|ns)$", std::regex_constants::icase);
    static const std::regex spaceREG("^%(space|s)$", std::regex_constants::icase);
    static const std::regex fontREG("^%(font)\\:[a-zA-Z]+$", std::regex_constants::icase);
    
    static const std::regex DisplayREG("^%(Display)$", std::regex_constants::icase);
    static const std::regex Headline1REG("^%(Headline1|H1)$", std::regex_constants::icase);
    static const std::regex Headline2REG("^%(Headline2|H2)$", std::regex_constants::icase);
    static const std::regex Headline3REG("^%(Headline3|H3)$", std::regex_constants::icase);
    static const std::regex Headline4REG("^%(Headline4|H4)$", std::regex_constants::icase);
    static const std::regex Headline5REG("^%(Headline5|H5)$", std::regex_constants::icase);
    static const std::regex CaptionCapsREG("^%(CaptionCaps)$", std::regex_constants::icase);
    static const std::regex CaptionREG("^%(Caption)$", std::regex_constants::icase);
    static const std::regex BodyLargeREG("^%(BodyLarge)$", std::regex_constants::icase);
    static const std::regex BodyMediumREG("^%(BodyMedium)$", std::regex_constants::icase);
    static const std::regex BodySmallREG("^%(BodySmall)$", std::regex_constants::icase);
    static const std::regex BodyArticleREG("^%(BodyArticle)$", std::regex_constants::icase);
    
    if(std::regex_match(src, idREG)){
        hints.id = src.substr(2, src.length()-3);
    } else if(std::regex_match(src, sizeREG)){
        sscanf(src.substr(6, src.size()).c_str(), "%d", &hints.size);
    } else if(std::regex_match(src, colorRgbREG)){
        short rgb[3];
        sscanf(src.substr(10, src.size()).c_str(), "(%hd,%hd,%hd)", &rgb[0], &rgb[1],&rgb[2]);
        hints.color = Color(rgb[0], rgb[1], rgb[2]);
    } else if(std::regex_match(src, colorRgbaREG)){
        short rgba[4];
        sscanf(src.substr(11, src.size()).c_str(), "(%hd,%hd,%hd,%hd)", &rgba[0], &rgba[1],&rgba[2],&rgba[3]);
        hints.color = Color(rgba[0], rgba[1],rgba[2],rgba[3]);
    } else if(std::regex_match(src, colorPrimaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Primary>();
    } else if(std::regex_match(src, colorSecondaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Secondary>();
    } else if(std::regex_match(src, colorTertiaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Tertiary>();
    } else if(std::regex_match(src, colorQuaternaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Quaternary>();
    } else if(std::regex_match(src, colorQuinaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Quinary>();
    } else if(std::regex_match(src, colorSenaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Senary>();
    } else if(std::regex_match(src, colorSeptenaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Septenary>();
    } else if(std::regex_match(src, colorOctonaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Octonary>();
    } else if(std::regex_match(src, colorNonaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Nonary>();
    } else if(std::regex_match(src, colorDenaryREG)){
        hints.color = Themes::_default.colors.get_text_color<Denary>();
    } else if(std::regex_match(src, newlnREG)){
        if(!words.empty()) {
            _new_line(words);
            line.clear();
            words.clear();
        }
    } else if(std::regex_match(src, regularREG)){
        hints.style = Font::Regular;
    } else if(std::regex_match(src, boldREG)){
        hints.style = Font::Bold;
    } else if(std::regex_match(src, boldObliqueREG)){
        hints.style = Font::BoldOblique;
    } else if(std::regex_match(src, extraLightREG)){
        hints.style = Font::ExtraLight;
    } else if(std::regex_match(src, obliqueREG)){
        hints.style = Font::Oblique;
    } else if(std::regex_match(src, condensed_BoldREG)){
        hints.style = Font::Condensed_Bold;
    } else if(std::regex_match(src, condensed_BoldObliqueREG)){
        hints.style = Font::Condensed_BoldOblique;
    } else if(std::regex_match(src, condensed_ObliqueREG)){
        hints.style = Font::Condensed_Oblique;
    } else if(std::regex_match(src, condensedREG)){
        hints.style = Font::Condensed;
    } else if(std::regex_match(src, blackREG)){
        hints.style = Font::Black;
    } else if(std::regex_match(src, blackItalicREG)){
        hints.style = Font::BlackItalic;
    } else if(std::regex_match(src, boldItalicREG)){
        hints.style = Font::BoldItalic;
    } else if(std::regex_match(src, extraLightItalicREG)){
        hints.style = Font::ExtraLightItalic;
    } else if(std::regex_match(src, extraBoldREG)){
        hints.style = Font::ExtraBold;
    } else if(std::regex_match(src, extraBoldItalicREG)){
        hints.style = Font::ExtraBoldItalic;
    } else if(std::regex_match(src, italicREG)){
        hints.style = Font::Italic;
    } else if(std::regex_match(src, lightREG)){
        hints.style = Font::Light;
    } else if(std::regex_match(src, lightItalicREG)){
        hints.style = Font::LightItalic;
    } else if(std::regex_match(src, mediumREG)){
        hints.style = Font::Medium;
    } else if(std::regex_match(src, mediumItalicREG)){
        hints.style = Font::MediumItalic;
    } else if(std::regex_match(src, semiboldREG)){
        hints.style = Font::SemiBold;
    } else if(std::regex_match(src, semiboldItalicREG)){
        hints.style = Font::SemiBoldItalic;
    } else if(std::regex_match(src, thinREG)){
        hints.style = Font::Thin;
    } else if(std::regex_match(src, thinItalicREG)){
        hints.style = Font::ThinItalic;
    } else if(std::regex_match(src, nospaceREG)){
        hints.noSpace = true;
    } else if(std::regex_match(src, spaceREG)){
        hints.noSpace = true;
        hints.space = true;
    } else if(std::regex_match(src, fontREG)){
        Font * font_buffer  = nullptr;
        const std::string global_name = src.substr(6, src.size());
        if((font_buffer = Font::_get_font_by_global_name(global_name.c_str()))){
            *hints.font = (*font_buffer);
        } else {
            Core::log(Core::Warning, std::string("(Paragraph::_parse_hint): Unable to find a global font with the name \"" + global_name + "\".").c_str());
        }
    } else if(std::regex_match(src, DisplayREG)){
        hints.style = TextStyles::Display.font_style;
        hints.size = TextStyles::Display.size;
    } else if(std::regex_match(src, Headline1REG)){
        hints.style = TextStyles::Headline1.font_style;
        hints.size = TextStyles::Headline1.size;
    } else if(std::regex_match(src, Headline2REG)){
        hints.style = TextStyles::Headline2.font_style;
        hints.size = TextStyles::Headline2.size;
    } else if(std::regex_match(src, Headline3REG)){
        hints.style = TextStyles::Headline3.font_style;
        hints.size = TextStyles::Headline3.size;
    } else if(std::regex_match(src, Headline4REG)){
        hints.style = TextStyles::Headline4.font_style;
        hints.size = TextStyles::Headline4.size;
    } else if(std::regex_match(src, Headline5REG)){
        hints.style = TextStyles::Headline5.font_style;
        hints.size = TextStyles::Headline5.size;
    } else if(std::regex_match(src, CaptionCapsREG)){
        hints.style = TextStyles::CaptionCaps.font_style;
        hints.size = TextStyles::CaptionCaps.size;
    } else if(std::regex_match(src, CaptionREG)){
        hints.style = TextStyles::Caption.font_style;
        hints.size = TextStyles::Caption.size;
    } else if(std::regex_match(src, BodyLargeREG)){
        hints.style = TextStyles::BodyLarge.font_style;
        hints.size = TextStyles::BodyLarge.size;
    } else if(std::regex_match(src, BodyMediumREG)){
        hints.style = TextStyles::BodyMedium.font_style;
        hints.size = TextStyles::BodyMedium.size;
    } else if(std::regex_match(src, BodySmallREG)){
        hints.style = TextStyles::BodySmall.font_style;
        hints.size = TextStyles::BodySmall.size;
    } else if(std::regex_match(src, BodyArticleREG)){
        hints.style = TextStyles::BodyArticle.font_style;
        hints.size = TextStyles::BodyArticle.size;
    } else {
        Core::log(Core::Warning, (std::string("(Paragraph::_parse_hint): Unknow Paragraph hint: \"" + src + "\"")).c_str());
    }
}

void BSParagraph::_add_word(const char * str){
    Text * word = new Text(str);
    if(!hints.id.empty()) {
        identifiableWords.insert(std::pair<std::string, Text*>{hints.id, word});
        hints.id.clear();
    }
    word->set_foreground_color(hints.color);
    if(word->font) {
        *word->font = *hints.font;
    } word->set_font_style(hints.style, hints.size);
    words.push_back(word);
}

void BSParagraph::_new_line(std::list<Object *> words){
    HStack * line = new HStack;
    line->fromList(words);
    lines.push_back(line);
}
