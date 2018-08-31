#pragma once

template <class T>
class SimpleCyclicIterator {
    T *_array;
    T *_current;
    size_t _count;

public:
    SimpleCyclicIterator(T *elements, size_t count);
    ~SimpleCyclicIterator();
    
    T *current() const;
    T *next();
    T *cArray() const;
    size_t count()const;
};

template<class T>
inline SimpleCyclicIterator<T>::SimpleCyclicIterator(T *array, size_t count)
:_array(array)
,_current(_array)
,_count(count)
{}

template<class T>
inline SimpleCyclicIterator<T>::~SimpleCyclicIterator()
{}

template <class T>
inline T* SimpleCyclicIterator<T>::current() const {
    return _current;
}

template <class T>
inline T* SimpleCyclicIterator<T>::next() {
    _current++;
    if(_current == _array + _count)
        _current = _array;
    return _current;
}

template <class T>
inline T* SimpleCyclicIterator<T>::cArray() const {
    return _array;
}

template <class T>
inline size_t SimpleCyclicIterator<T>::count() const {
    return _count;
}
