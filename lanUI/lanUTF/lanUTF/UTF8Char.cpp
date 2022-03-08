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

unsigned int UTF8Char::_get_byte_as_intermediate(char byte){
    return (uint8_t)(byte <<= 2)>>2;
}

unsigned int UTF8Char::get_UTF8_4o_charcode(){
    unsigned int scalar = 0;
    uint8_t temp;
    return (((scalar = ((scalar = ((scalar = (temp = data[0] << 5) >> 5) << 6) + _get_byte_as_intermediate(data[1])) << 6) + _get_byte_as_intermediate(data[2])) << 6) + _get_byte_as_intermediate(data[3]));
}

unsigned int UTF8Char::get_UTF8_3o_charcode(){
    unsigned int scalar = 0;
    uint8_t temp;
    return (((scalar = ((scalar = (temp = data[0] << 4) >> 4) << 6) + _get_byte_as_intermediate(data[1])) << 6) + _get_byte_as_intermediate(data[2]));
}

unsigned int UTF8Char::get_UTF8_2o_charcode(){    
    unsigned int scalar = 0;
    uint8_t temp;
    return (((scalar = (temp = data[0] << 3) >> 3) << 6) + _get_byte_as_intermediate(data[1]));
}

unsigned int UTF8Char::get_UTF8_1o_charcode(){
    return data[0];
}

unsigned int UTF8Char::get_charcode(){
    switch (octetType) {
        case UTF8_1o:
            return get_UTF8_1o_charcode();
        case UTF8_2o:
            return get_UTF8_2o_charcode();
        case UTF8_3o:
            return get_UTF8_3o_charcode();
        case UTF8_4o:
            return get_UTF8_4o_charcode();
        default:
            return 0;
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

void UTF8Char::composeUTF8Char(const unsigned int code){
    unsigned int codebuf;
    /// ordered bytes
    uint8_t ob[4];
    /*
     Clear bytes before (<< N) and then move (N >> 24) the actual byte to the correct position.
     */
#if BYTE_ORDER == LITTLE_ENDIAN
    ob[3] = (codebuf = code << 24) >> 24;
    ob[2] = (codebuf = code << 16) >> 24;
    ob[1] = (codebuf = code << 8) >> 24;
    ob[0] = (codebuf = code) >> 24;
#elif BYTE_ORDER == BIG_ENDIAN
    ob[0] = (codebuf = code << 24) >> 24;
    ob[1] = (codebuf = code << 16) >> 24;
    ob[2] = (codebuf = code << 8) >> 24;
    ob[3] = (codebuf = code) >> 24;
#elif BYTE_ORDER == PDP_ENDIAN
    ob[1] = (codebuf = code << 24) >> 24;
    ob[0] = (codebuf = code << 16) >> 24;
    ob[3] = (codebuf = code << 8) >> 24;
    ob[2] = (codebuf = code) >> 24;
#endif
    
    clear();
    
    // byte buffer
    uint8_t bb;
    
    if(code >= 0x0 && code <= 0x7F){
        octetType = UTF8_1o;
        // [0] = 0XXX'XXXX
        data[0] = ((ob[3] <<= 1) >> 1);
        
    } else if (code >= 0x80 && code <= 0x77F) {
        octetType = UTF8_2o;
        // [0] = 0011'0000
        bb = 0xC0 >> 2;
        // 0000'0YYY = YYY0'0000 = 0000'0YYY
        ob[2] = (ob[2] << 5) >> 5;
        // [0] = 0011'0YYY
        bb += ob[2];
        // [0] = 110Y'YY00
        bb = bb << 2;
        // [0] = 110Y'YYYY
        bb += ob[3] >> 6;
        data[0] = bb;
        
        // ? bte 1
        
        // [1] = 1000'0000
        bb = (1 << 7);
        // [1] = 10XX'XXXX
        bb += (ob[3] <<= 2) >> 2;
        data[1] = bb;
        
        // ? byte 2
        
        // [0][1] == 110Y'YYYY'10XX'XXXX
        
    } else if (code >= 0x800 && code <= 0xFFFF) {
        octetType = UTF8_3o;
        // [0] = 1110'0000
        bb = 0xE0;
        // [0] = 1110'ZZZZ
        bb += (ob[2] >> 4);
        data[0] = bb;
        // ? byte 1
        
        // ZZZZ'YYYY = 0000'YYYY
        ob[2] = (ob[2] <<= 4) >> 4;
        // [1] = 0010'0000
        bb = 1 << 5;
        // [1] = 0010'YYYY
        bb += ob[2];
        // [1] = 10YY'YY00
        bb <<= 2;

        // [1] = 10YY'YYYY
        bb += ob[3] >> 6;
        data[1] = bb;
        // ? byte 2
        
        // [2] = 1000'0000
        bb = 1 << 7;
        // [2] = 10XX'XXXX
        bb += (ob[3] <<= 2) >> 2;
        data[2] = bb;
        // ? byte 3
        
        // [0][1][2] == 1110'ZZZZ'10YY'YYYY'10XX'XXXX
        
    } else if(code >= 0x10000 && code <= 0x1FFFFF) {
        octetType = UTF8_4o;
        // [0] = 1111'0000
        bb = 0xF0;
        
        // [0] = 1111'0UUU
        bb += ob[1] >> 2;
        data[0] = bb;
        
        // ? byte 1
        
        // [1] = 0000'1000
        bb = 1 << 3;
        // [1] = 0000'10UU
        bb += (ob[1] <<= 6) >> 6;
        // [1] = 10UU'0000
        bb <<= 4;
        // [1] = 10UU'ZZZZ
        bb += ob[2] >> 4;
        data[1] = bb;
        
        // ? byte 2
        
        // ZZZZ'YYYY = 0000'YYYY
        ob[2] = (ob[2] <<= 4) >> 4;
        // [2] = 0010'0000
        bb = 1 << 5;
        // [2] = 0010'YYYY
        bb += ob[2];
        // [2] = 10YY'YY00
        bb <<= 2;
        // [2] = 10YY'YYYY
        bb += ob[3] >> 6;
        data[2] = bb;
        
        // ? byte 3
        
        // YYXX'XXXX = XXXX'XX00 = 00XX'XXXX
        ob[3] = (ob[3] <<= 2) >> 2;
        // [3] = 1000'0000
        bb = 1 << 7;
        // [3] = 10XX'XXXX
        bb += ob[3];
        data[3] = bb;
        
        // ? byte 4
        
        // [0][1][2][3] = 1111'0UUU'10UU'ZZZZ'10YY'YYYY'10XX'XXXX
    }
}

bool UTF8Char::is_charcode_valid(const unsigned int code){
    return (code >= 0x0 && code <= 0x7F) | (code >= 0x80 && code <= 0x77F) | (code >= 0x800 && code <= 0xFFFF) | (code >= 0x10000 && code <= 0x1FFFFF);
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

#ifdef LANUTF_DEBUG_MODE

bool UTF8Char::dbg_test_UTF8Char_char(char * __char){
    char copy[5] = "\0";
    bzero(copy, 5);
    strncpy(copy, __char,4);
    composeUTF8Char(__char);
    return strncmp(copy, c_char(), 4) == 0;
}

bool UTF8Char::dbg_test_UTF8Char_charcode(unsigned int code){
    composeUTF8Char(code);
    return get_charcode() == code;
}

void UTF8Char::dbg_test_global(unsigned int beg, unsigned int end){
    if(beg > end)
        return;
    char buffer[5];
    unsigned total_tests = end - beg, passed = 0;
    
    for(unsigned int i = beg ; i < end ; i++){
        if(dbg_test_UTF8Char_charcode(i)){
            passed++;
            printf("test::charcode_match_char for UTF8char \"%s\" | %u  [PASSED]\n", c_char(), get_charcode());
        } else if(!UTF8Char::is_charcode_valid(i)){
            total_tests--;
        } else {
            printf("test::charcode_match_char for UTF8char \"%s\" | %u  [FAILED]\n", c_char(), get_charcode());
        }
        
        bzero(buffer, 5);
        
        strncpy(buffer, c_char(), 4);
        
        if(dbg_test_UTF8Char_char(buffer)){
            printf("recursive_test::dbg_test_UTF8Char_char_match_charcode for UTF8char \"%s\" | %u  [PASSED]\n", c_char(), get_charcode());
        } else {
            passed--;
            printf("recursive_test::dbg_test_UTF8Char_char_match_charcode for UTF8char \"%s\" | %u  [FAILED]\n", c_char(), get_charcode());
        }
        
        printf("\n");
    }
    
    printf("%u tests done.\n%u passed.\n", total_tests, passed);
}

#endif


UTF8Char::UTF8Char(): octetType(UTF8_1o), _next(NULL), data("\0") {
}

UTF8Char::UTF8Char(char * c_char){
    UTF8Char();
    composeUTF8Char(c_char);
}

UTF8Char::UTF8Char(const char * cc_char){
    char _c_char[5];
    bzero(_c_char, 5);
    strcpy(_c_char, cc_char);
    UTF8Char();
    composeUTF8Char(_c_char);
}

UTF8Char::~UTF8Char(){
    clear();
}
