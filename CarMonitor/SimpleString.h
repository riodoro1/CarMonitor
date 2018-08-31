#pragma once

#include <string.h>

class SimpleString {
    char *_strData = NULL;
public:
    SimpleString();
    SimpleString(size_t size);
    SimpleString(const char *strData);
    SimpleString(const SimpleString &source);
    
    ~SimpleString();
    
    const char *cString() const;
    
    const size_t length() const;
    const bool isEmpty() const;
    
    void concat(SimpleString s2);
    
private:
    void grow(size_t by);
};

// Constructors ========================================================

inline SimpleString::SimpleString()
:_strData(NULL)
{}

inline SimpleString::SimpleString(size_t size)
:SimpleString()
{
    if(size == 0) return;
    _strData = new char[size+1];
    _strData[0] = 0;
}

inline SimpleString::SimpleString(const char *strData)
:SimpleString()
{
    size_t length = strlen(strData);
    if(length == 0)
        return;
    
    _strData = strdup(strData);
}

inline SimpleString::SimpleString(const SimpleString &source)
:SimpleString(source.cString())
{
}

// Destructors ==========================================================
inline SimpleString::~SimpleString() {
    delete[] _strData;
}

// Public methods =======================================================

inline const char *SimpleString::cString() const {
    return _strData;
}

inline const size_t SimpleString::length() const {
    return strlen(_strData);
}

inline const bool SimpleString::isEmpty() const {
    return _strData == NULL || strlen(_strData) == 0;
}

inline void SimpleString::concat(SimpleString s2) {
    grow(s2.length());
    strcat(_strData, s2.cString());
}

// Private methods ======================================================

inline void SimpleString::grow(size_t amount) {
    size_t totalLength = length() + amount;
    
    const char *oldContent = _strData;
    _strData = new char[totalLength+1];
    _strData[0] = 0;
    
    strcat(_strData, oldContent);
    
    delete[] oldContent;
}
