# LANUTF
###### `0.1.6 stable`

#### A library to handle Unicode (UTF-8) in C++.

## Whats new in version 0.1.6

- Debugging methods added
    - <strong>(UTF8Char)</strong> tests passed for all <strong>2097023</strong> unicode characters.

# `UTF8Char`

A **UTF8 character** is a unicode character encoded in UTF8 format, as “ü” or “😊”.

### Important methods:

#### `unsigned int get_charcode();`

 Returns the unicode scalar value aka charcode of the current character.

#### `void composeUTF8Char(char * source)`

Composes a new  character from UTF8 encoded source.

#### `void composeUTF8Char(const unsigned int code);`

Composes a new  character from a unicode scalar value aka  charcode.

#### `void copy(UTF8Char & other)`

   Copies data from another UTF8Char.

#### `char const * c_char() const`

   Returns the c_char encoded version of the UTF8 char.

#### `UTF8_OctetType const octet() const`

   Returns the UTF8 octet type of the character.

#### `void clear()`

   Clears the character.

# `UTF8CharList`

A **UTF8 character list** is a string of UTF8 encoded characters, as “rené” or “âėīøü”.

### Important methods:

#### `size_t size() const`

   Returns the length of list.

#### `size_t c_str_size() const`

   Returns the length of a possible C string version of the list.

#### `size_t c_index_at(const size_t index) const`

   Returns the index of this UTF8Char in a possible C string.

#### `size_t c_index_before(const size_t index) const`

   Returns the index of the UTF8Char before this in a possible C string.

#### `bool empty() const`

   Returns true if the list is empty.

#### `void remove(size_t index)`

   Removes a specified UTF8 character.

#### `char * alloc_c_str() const`

   Allocates a C string version of the list.

#### `void composeCStr(char * str, size_t size) const`

   Composes a C string version of the list.

#### `static void free_c_str(char * str)`

   Frees an allocated C string.

#### `size_t append(const char * str)`

   Adds characters to the end of the list.

#### `size_t append(const char * str, size_t index)`

   adds characters to a specified position of the list or to the end if the index isn't valid.

#### `void clear()`

   Clears the list.

# Projets using LanUTF
- [LanUI](github.com/renemuala/lanui) a library to create user interfaces in c++.
    - LanUTF is being user do handle with unicode characters in [Freetype2](https://freetype.org/index.html) and text input routines.
