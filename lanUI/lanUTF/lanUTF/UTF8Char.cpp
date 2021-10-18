//
//  UTF8Char.cpp
//  lanUTF
//
//  Created by René Descartes Domingos Muala on 15/10/21.
//

#include "UTF8CharList.hpp"
#include <iostream>

UTF8Char * UTF8Char::_createNext(){
    return (_next = new UTF8Char);
}

UTF8Char * UTF8Char::_appendNext(){
    UTF8Char * buffer = _next;
    _next = new UTF8Char;
    _next->_next = buffer;
    return _next;
}

bool UTF8Char::_byte_match(const std::bitset<8> src, const std::bitset<8> fmt, size_t bitsCount){
    for (int i = 7 ; i > 7 - bitsCount ; i --){
        if(src[i] != fmt[i]) return false;
    } return true;
}

UTF8Char::UTF8_OctetType const UTF8Char::_get_octetType(const uint8_t value){
    if(_byte_match(value, UTF8_1o_format, 1)){
        return UTF8_1o;
    } else if (_byte_match(value, UTF8_2o_format, 3)) {
        return UTF8_2o;
    } else if (_byte_match(value, UTF8_3o_format, 4)) {
        return UTF8_3o;
    } else if (_byte_match(value, UTF8_4o_format, 5)) {
        return UTF8_4o;
    } else {
        return UTF8_Io;
    }
}

void UTF8Char::_readUTF8_4o(char *source){
    if(_byte_match(*source, UTF8_4o_format, 5)){
        data[0] = *source;
        source++;
    }
    if(_byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[1] = *source;
        source++;
    }
    if(_byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[2] = *source;
        source++;
    }
    if(_byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[3] = *source;
        source++;
    }
}

void UTF8Char::_readUTF8_3o(char *source){
    if(_byte_match(*source, UTF8_3o_format, 4)){
        data[0] = *source;
        source++;
    }
    if(_byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[1] = *source;
        source++;
    }
    if(_byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[2] = *source;
        source++;
    }
}

void UTF8Char::_readUTF8_2o(char *source){
    if(_byte_match(*source, UTF8_2o_format, 3)){
        data[0] = *source;
        source++;
    }
    if(_byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[1] = *source;
        source++;
    }
}

void UTF8Char::_readUTF8_1o(char *source){
    if(_byte_match(*source, UTF8_1o_format, 1)){
        data[0] = *source;
        source++;
    }
}

void UTF8Char::composeUTF8Char(char *source){
    clear();
    switch ((octetType = _get_octetType(source[0]))) {
        case UTF8_1o:
            _readUTF8_1o(source);
            break;
        case UTF8_2o:
            _readUTF8_2o(source);
            break;;
        case UTF8_3o:
            _readUTF8_3o(source);
            break;
        case UTF8_4o:
            _readUTF8_4o(source);
            break;
        default:
            _readUTF8_1o(source);
            break;
    }
}

void UTF8Char::copy(UTF8Char &other){
    clear();
    strncpy(this->data, other.data, 4);
    this->octetType = other.octetType;
}

char const * UTF8Char::c_char() const {
    return data;
}

UTF8Char::UTF8_OctetType const UTF8Char::octet() const {
    return octetType;
}

UTF8Char * UTF8Char::_at(size_t index) const {
    UTF8Char * last = (UTF8Char *)((void*)(this));
    size_t count_index = 0;
    while (last) {
        if(count_index == index)
            return last;
        last = last->_next;
        count_index++;
    } return nullptr;
}

UTF8Char * UTF8Char::_last() const {
    UTF8Char * last = (UTF8Char *)((void*)(this));
    while (last->_next) {
        last = last->_next;
    } return last;
}

UTF8Char::UTF8Char(): octetType(UTF8_1o), _next(NULL), data("\0") {
}

UTF8Char::UTF8Char(char * c_char){
    UTF8Char();
    composeUTF8Char(c_char);
}

UTF8Char::~UTF8Char(){
    clear();
}
