//
//  UTF8CharList.hpp
//  lanUTF
//
//  Created by René Descartes Domingos Muala on 15/10/21.
//

#ifndef UTF8CharList_hpp
#define UTF8CharList_hpp

#include <stdio.h>
#include <stdlib.h>
#include <bitset>
#include <list>

#define lanUTFVersion "lanUTF 0.1.6"

#define LANUTF_DEBUG_MODE

class UTF8Char {
public:
        
    const std::bitset<8> UTF8_intermetidate_byte = 0x80;
    
    const std::bitset<8> UTF8_1o_format = 0x00;
    
    const std::bitset<8> UTF8_2o_format = 0xC0;
    
    const std::bitset<8> UTF8_3o_format = 0xE0;
    
    const std::bitset<8> UTF8_4o_format = 0xF0;
    
    typedef enum {
        /// 0XXXXXXX ; Simple ASCII
        UTF8_1o,
        /// 110YYYYY + 10XXXXXX
        UTF8_2o,
        /// 1110ZZZZ + 10YYYYYY + 10XXXXXX
        UTF8_3o,
        /// 11110UUU + 10UUZZZZ + 10YYYYYY + 10XXXXXX
        UTF8_4o,
        /// Invalid Octet, may be a result of bad encodig
        UTF8_Io,
    } UTF8_OctetType;
    
private:
    
    char data[5];
        
    UTF8_OctetType octetType;
                
public:
    
    UTF8Char * _createNext();
    
    UTF8Char * _appendNext();
    
    UTF8Char * _next;
    
    bool _byte_match(const std::bitset<8> src, const std::bitset<8> fmt, size_t n);
    
    UTF8_OctetType const _get_octetType(uint8_t const);
    
    void _readUTF8_4o(char * source);
    
    void _readUTF8_3o(char * source);
    
    void _readUTF8_2o(char * source);
    
    void _readUTF8_1o(char * source);
    
    static unsigned int _get_byte_as_intermediate(char byte);
    
    unsigned int get_UTF8_4o_charcode();
    
    unsigned int get_UTF8_3o_charcode();

    unsigned int get_UTF8_2o_charcode();

    unsigned int get_UTF8_1o_charcode();

    unsigned int get_charcode();
    
    /// composes a new  character from a UTF8 encoded source
    void composeUTF8Char(char * source);
    
    /// composes a new  character from a UTF8 charcode
    void composeUTF8Char(const unsigned int code);
    
    static bool is_charcode_valid(const unsigned int code);
    
    /// copies data from another UTF8Char
    void copy(UTF8Char & other);

    /// returns the c_char version of the UTF8 char
    char const * c_char() const;
    
    /// returns the UTF8 octet type of the character.
    UTF8_OctetType const octet() const;
    
    /// clears the character
    void clear(){
        for(int i = 0 ; i < 5 ; i++)
            data[i] = '\0';
    }
    
    UTF8Char * _at(size_t index) const;
    
    UTF8Char * _last() const;
    
#ifdef LANUTF_DEBUG_MODE
    
    bool dbg_test_UTF8Char_char(char *);
    
    bool dbg_test_UTF8Char_charcode(unsigned int);

    /// test all unicode characters
    void dbg_test_global(unsigned int beg = 0x0, unsigned int end = 0x1FFFFF);
    
#endif
    
    UTF8Char();
    
    UTF8Char(char *);
    
    UTF8Char(const char *);
    
    ~UTF8Char();
    
};

class UTF8CharList {
    UTF8Char * first;
public:
    
    /// returns the length of list
    size_t size() const;
    
    /// returns the length of a possible C string version of the list
    size_t c_str_size() const;
    
    /// returns the index of this UTF8Char in a possible C string
    size_t c_index_at(const size_t index) const;
    
    /// returns the index of the UTF8Char before this in a possible C string
    size_t c_index_before(const size_t index) const;
    
    /// returns true if the list is empty
    bool empty() const;
    
    const char * operator[](size_t index) const;
    
    /// returns the character at the given index (if possible)
    const char * char_at(size_t index) const;
    
    /// returns the charcode at the given index (if possible)
    unsigned int charcode_at(size_t index) const;
    
    /// removes a specified UTF8 character
    void remove(size_t index);
    
    /// removes the last UTF8 character
    void remove_last();
    
    /// allocates a C string version of the list
    char * alloc_c_str() const;
    
    /// composes a C string version of the list
    void composeCStr(char * str, size_t size) const;

    /// frees an allocated C string
    static void free_c_str(char * str);
    
    /// adds characters to the end of the list
    size_t append(const char * str);
    
    /// adds characters to the end of the list
    size_t append(std::list<const unsigned int> codes);
    
    /// adds characters to a specified position of the list or to the end if the index isn't valid
    size_t append(const char * str, size_t index);
    
    /// adds characters to a specified position of the list or to the end if the index isn't valid
    size_t append(std::list<const unsigned int> codes, size_t index);
    
    /// clears the list
    void clear();
    
    UTF8CharList();
    
    UTF8CharList(char *);
    
    ~UTF8CharList();
};

#endif /* UTF8CharList_hpp */
