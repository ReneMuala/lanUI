# LANUTF
###### `0.1.5 stable`

#### A library to deal with UTF8 encoded characters in C++.

# `UTF8Char`

A **UTF8 character** is a unicode character encoded in UTF8 format, as “ü” or “😊”.

### Important methods:

#### `void composeUTF8Char(char * source)`

   Composes a new  character from UTF8 encoded source.

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

