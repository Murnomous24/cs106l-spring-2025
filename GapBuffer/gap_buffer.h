
#ifndef GAPBUFFER_H
#define GAPBUFFER_H
#include <vector>
#include <iostream> // for cout in debug
#include <algorithm>
#include <sstream> // for stringstreams
#include <memory> // for unique_ptr

const int kDefaultSize = 10;

// forward declaration for the GapBufferIterator class
template <typename T>
class GapBufferIterator;

// declaration for the GapBuffer class
template <typename T>
class GapBuffer {
public:
    friend class GapBufferIterator<T>;

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using iterator = GapBufferIterator<T>;

    explicit GapBuffer();
    explicit GapBuffer(size_type count, const value_type& val = value_type());
    ~GapBuffer();
    GapBuffer(std::initializer_list<T> init);
    GapBuffer(const GapBuffer& other);
    GapBuffer(GapBuffer&& other);
    GapBuffer& operator=(const GapBuffer& rhs);
    GapBuffer& operator=(GapBuffer&& rhs);

    void insert_at_cursor(const_reference element);
    void insert_at_cursor(value_type&& element);
    template <typename... Args>
    void emplace_at_cursor(Args&&... args); // optional
    void delete_at_cursor();
    reference get_at_cursor();
    const_reference get_at_cursor() const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    void move_cursor(int num);
    void reserve(size_type new_size);
    size_type size() const;
    size_type cursor_index() const;
    bool empty() const;
    void debug() const;

    iterator begin();
    iterator end();
    iterator cursor();

private:
    size_type _logical_size; // uses external_index
    size_type _buffer_size;  // uses array_index
    size_type _cursor_index; // uses array_index
    size_type _gap_size;
    // pointer _elems;          // uses array_index // raii place
    std::unique_ptr<T[]> _elems;

    size_type to_external_index(size_type array_index) const;
    size_type to_array_index(size_type external_index) const;
    void move_to_left_of_buffer(size_type num);
};

// Class declaration of the GapBufferIterator class
template <typename T>
class GapBufferIterator : public std::iterator<std::random_access_iterator_tag, T> {
public:
    friend class GapBuffer<T>;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using iterator = GapBufferIterator<T>;

    reference operator*();
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);

    // the operators below are implemented as non-friend non-members
    // iterator operator+(const iterator& lhs, size_type diff);
    // iterator operator-(const iterator& lhs, size_type diff);
    // iterator operator+(size_type diff, const iterator& rhs);

    /* we implemented these for you. don't change these */
    friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs._index == rhs._index; }
    friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
    friend difference_type operator-(const iterator& lhs, const iterator& rhs) { return lhs._index - rhs._index; }
    iterator& operator+=(size_type diff) { _index += diff; return *this; }
    iterator& operator-=(size_type diff) { _index -= diff; return *this; }
    friend bool operator>(const iterator& lhs, const iterator& rhs)  { return lhs._index < rhs._index; }
    friend bool operator<(const iterator& lhs, const iterator& rhs)  { return lhs._index > rhs._index; }
    friend bool operator<=(const iterator& lhs, const iterator& rhs)  { return !(lhs > rhs); }
    friend bool operator>=(const iterator& lhs, const iterator& rhs)  { return !(lhs < rhs); }
    reference operator[](size_t index) { return *(*this + index); }

private:
    GapBuffer<T>* _pointee;
    size_t _index;
    GapBufferIterator(GapBuffer<T>* pointee, size_t index) : _pointee(pointee), _index(index) {}
};

// Part 1: basic functions
template <typename T>
GapBuffer<T>::GapBuffer(): 
    _logical_size(0),
    _buffer_size(kDefaultSize),
    _cursor_index(0),
    _gap_size(kDefaultSize),
    // _elems(new T[kDefaultSize]),
    _elems(std::make_unique<T[]>(kDefaultSize)) {
    // TODO: Implement the default constructor (~5 lines long)
    // use member initialization list
}

template <typename T>
GapBuffer<T>::GapBuffer(size_type count, const value_type& val):
    _logical_size(count),
    _buffer_size(2 * count),
    _cursor_index(count),
    _gap_size(count),
    // _elems(new T[2 * count])
    _elems(std::make_unique<T[]>(2 * count)) {
    // TODO: Implement the fill constructor (~6 lines long)
    // use member initialization list

    for(size_type index = 0; index < count; index ++) { // NOTE: use size_type not int
        _elems[index] = val;
    }
}

template <typename T>
void GapBuffer<T>::insert_at_cursor(const_reference element) {
    // TODO: implement this function (~7 lines long)
    // Hint: call reserve() to resize

    if(_gap_size == 0) reserve(2 * _buffer_size);

    _elems[_cursor_index] = element;
    _cursor_index ++;
    _logical_size ++;
    _gap_size --;
}

template <typename T>
void GapBuffer<T>::delete_at_cursor() {
    // TODO: implement this function (~4 lines long)

    if(_cursor_index == 0) return ;
    
    _cursor_index --;
    _logical_size --;
    _gap_size ++;
}

template <typename T>
typename GapBuffer<T>::reference GapBuffer<T>::get_at_cursor() {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick here after implementing the const-version.

    if(_cursor_index == _logical_size) throw std::out_of_range("get_at_cursor: cursor index out of bound");
    return _elems[to_array_index(_cursor_index)]; // TODO: from the gap of the 'i-1'st word to 'i'st word?
}

template <typename T>
typename GapBuffer<T>::reference GapBuffer<T>::at(size_type pos) {
    // TODO: implement this function (~1 line long)
    // Hint: at should do error-checking!

    if(pos >= _logical_size) throw std::out_of_range("at: cursor index out of bound");
    
    return _elems[to_array_index(pos)];
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::size() const {
    // TODO: implement this function (~1 line long)
    return _logical_size;
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::cursor_index() const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide

    return _cursor_index;
}

template <typename T>
bool GapBuffer<T>::empty() const {
    // TODO: implement this function (~1 line long)

    return _logical_size == 0;
}

// Part 2: const-correctness

template <typename T>
typename GapBuffer<T>::const_reference GapBuffer<T>::get_at_cursor() const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick in the non-const version.

    return static_cast<const_reference>(const_cast<GapBuffer<T>*>(this)->get_at_cursor()); // NOTE: const_cast to remove const GapBuffer's const constraint, static_cast convert non-const return val to const
}

template <typename T>
typename GapBuffer<T> ::const_reference GapBuffer<T>::at(size_type pos) const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick in the non-const version.

    return static_cast<const_reference>(const_cast<GapBuffer<T>*>(this)->at(pos));
}

// Part 3: operator overloading
template <typename T>
typename GapBuffer<T>::reference GapBuffer<T>::operator[](size_type pos) {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick here after implementing the const-version.

    return at(pos);
}

template <typename T>
typename GapBuffer<T>::const_reference GapBuffer<T>::operator[](size_type pos) const {
    // TODO: implement this function (~1 line long)
    // Hint: check out the indexing helper functions we provide
    // Be sure to use the static_cast/const_cast trick in the non-const version.

    return at(pos);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const GapBuffer<T>& buf) {
    // TODO: implement this operator (~18 lines long)

    os << "{";

    for(size_t index = 0; index < buf.size(); index ++) {
        if(index != 0) os << " ";

        if(index == buf.cursor_index()) os << "^";
        os << buf[index];

        if(index != buf.size() - 1) os << ",";
    }
    if(buf.cursor_index() == buf.size()) os << "^";

    os << "}";

    return os; // NOTE
}

template <typename T>
bool operator==(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)
    // Hint: std::equal can be used after you implement iterators

    if(lhs.size() != rhs.size()) return 0;

    for(size_t index = 0; index < lhs.size(); index ++) {
        if(lhs[index] != rhs[index]) return 0;
    }

    return 1;
}

template <typename T>
bool operator!=(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)
    // Hint: how are == and != related?

    return !(lhs == rhs);
}

template <typename T>
bool operator<(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~3 lines long)
    // Hint: std::lexicographical_compare can be used after you implement iterators.

    // Hint: if you get warnings about const_iterator, you can try using const_cast
    // as a hack to cast away the const-ness of your parameter.
    // auto& lhs_nonconst = const_cast<GapBuffer<T>&>(lhs);
    // auto& rhs_nonconst = const_cast<GapBuffer<T>&>(lhs);
    // use lhs_nonconst.begin(), etc.

    for(size_t index = 0; index < std::min(lhs.size(), rhs.size()); index ++) {
        if(lhs[index] != rhs[index]) return lhs[index] < rhs[index];
    }

    return lhs.size() < rhs.size();
}

template <typename T>
bool operator>(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)

    return !((lhs < rhs) | (lhs == rhs));
}

template <typename T>
bool operator<=(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)

    return (lhs < rhs) | (lhs == rhs);
}

template <typename T>
bool operator>=(const GapBuffer<T>& lhs, const GapBuffer<T>& rhs) {
    // TODO: implement this operator (~1 line long)

    return (lhs > rhs) | (lhs == rhs);
}

// Part 4: turn everything into a template!


// Part 5: Implement iterators
template <typename T>
typename GapBufferIterator<T>::reference GapBufferIterator<T>::operator*() {
    // TODO: implement this operator (~1 line long)
    return _pointee->at(_index);
}

template <typename T>
GapBufferIterator<T>& GapBufferIterator<T>::operator++() {
    // TODO: implement this prefix operator (~2 lines long)
    _index ++;
    return *this;
}

template <typename T>
GapBufferIterator<T> GapBufferIterator<T>::operator++(int) {
    // TODO: implement this postfix operator (~3 lines long)

    iterator old_iter = *this;
    _index ++;
    return old_iter;
}

template <typename T>
GapBufferIterator<T>& GapBufferIterator<T>::operator--() {
    // TODO: implement this prefix operator (~2 lines long)

    _index --;
    return *this;
}

template <typename T>
GapBufferIterator<T> GapBufferIterator<T>::operator--(int) {
    // TODO: implement this postfix operator (~3 lines long)

    iterator old_iter = *this;
    _index --;
    return old_iter;
}

template <typename T>
GapBufferIterator<T> operator+(const GapBufferIterator<T>& lhs,
                               typename GapBufferIterator<T>::size_type diff) {
    // TODO: implement this operator (~3 lines long)
    // Note: this operator is not a friend of the GapBufferIterator class
    // Hint: write the operator in terms of +=

    GapBufferIterator<T> temp = lhs;
    temp += diff;
    return temp;
}

template <typename T>
GapBufferIterator<T> operator+(typename GapBufferIterator<T>::size_type diff,
                               const GapBufferIterator<T>& rhs) {
    // TODO: implement this operator (~1 line long)
    // Note: this operator is not a friend of the GapBufferIterator class
    // Hint: write the operator in terms of the operator+ you wrote above.

    return rhs + diff;
}

template <typename T>
GapBufferIterator<T> operator-(const GapBufferIterator<T>& lhs,
                               typename GapBufferIterator<T>::size_type diff) {
    // TODO: implement this operator (~3 lines long)
    // Note: this operator is not a friend of the GapBufferIterator class
    // Hint: write the operator in terms of -=

    GapBufferIterator<T> temp = lhs;
    temp -= diff;
    return temp;
}

// The functions that are part of the GapBuffer class is provided for you!
template <typename T>
typename GapBuffer<T>::iterator GapBuffer<T>::begin() {
    return iterator(this, 0);
}

template <typename T>
typename GapBuffer<T>::iterator GapBuffer<T>::end() {
    return iterator(this, _logical_size);
}

template <typename T>
typename GapBuffer<T>::iterator GapBuffer<T>::cursor() {
    return iterator(this, _cursor_index);
}

// Part 6: Constructors and assignment

template <typename T>
GapBuffer<T>::~GapBuffer() {
    // TODO: implement this destructor (~1 line long)
    // delete[] _elems; 
}
template <typename T>
GapBuffer<T>::GapBuffer(std::initializer_list<T> init):
    _logical_size(init.size()),
    _buffer_size(2 * init.size()),
    _cursor_index(init.size()),
    _gap_size(init.size()),
    // _elems(new T[2 * init.size()])
    _elems(std::make_unique<T[]>(2 * init.size())) {
    // TODO: implement this initializer list constructor (~2 lines long)
    // NOTE: 'init' contains the content gapbuffer need store

    std::copy(init.begin(), init.end(), _elems.get());
}

template <typename T>
GapBuffer<T>::GapBuffer(const GapBuffer& other):
    _logical_size(other._logical_size),
    _buffer_size(other._buffer_size),
    _cursor_index(other._cursor_index),
    _gap_size(other._gap_size),
    // _elems(new T[_buffer_size]) 
    _elems(std::make_unique<T[]>(other._buffer_size)){
    // TODO: implement this copy constructor (~4 lines long)
    // use member initialization list!
    // NOTE: how to access private member?

    std::copy(other._elems.get(), other._elems.get() + other._buffer_size, _elems.get());
}

template <typename T>
GapBuffer<T>& GapBuffer<T>::operator=(const GapBuffer& rhs) {
    // TODO: implement this copy assignment operator (~8 lines long)

    if(this == &rhs) return *this;

    auto new_elems = std::make_unique<T[]>(rhs._buffer_size);
    std::copy(rhs._elems.get(), rhs._elems.get() + rhs._buffer_size, new_elems.get());

    // NOTE: in raii version, no need to delete, use 'std::move' to avoid delete
    
    _elems = std::move(new_elems);
    _logical_size = rhs._logical_size;
    _buffer_size = rhs._buffer_size;
    _cursor_index = rhs._cursor_index;
    _gap_size = rhs._gap_size;

    return *this;
}

// Part 7: Move semantics
template <typename T>
GapBuffer<T>::GapBuffer(GapBuffer&& other):
    _logical_size(other._logical_size),
    _buffer_size(other._buffer_size),
    _cursor_index(other._cursor_index),
    _gap_size(other._gap_size),
    _elems(std::move(other._elems)) {
    // TODO: implement this move constructor (~4 lines long)
    // use initializer list!

    // Hint: if you get warnings about const_iterator, you can try using const_cast
    // as a hack to cast away the const-ness of your parameter.
    // auto& other_nonconst = const_cast<GapBuffer<T>&>(other);
    // use other.begin(), etc.

    // other._elems = nullptr; // after std::move, no need to change r-value's pointer NOTE: why
    other._logical_size = 0;
    other._buffer_size = 0;
    other._cursor_index = 0;
    other._gap_size = 0;
}

template <typename T>
GapBuffer<T>& GapBuffer<T>::operator=(GapBuffer&& rhs) {
    // TODO: implement this move assignment operator (~7 lines long)

    // Hint: if you get warnings about const_iterator, you can try using const_cast
    // as a hack to cast away the const-ness of your parameter.
    // auto& rhs_nonconst = const_cast<GapBuffer<T>&>(rhs);
    // use rhs.begin(), etc.

    if(this == &rhs) return *this;

    // NOTE: delete old '_elems'
    // delete[] _elems; // in raii no need to delete

    _logical_size = rhs._logical_size;
    _buffer_size = rhs._buffer_size;
    _cursor_index = rhs._cursor_index;
    _gap_size = rhs._gap_size;
    _elems = std::move(rhs._elems);

    // rhs._elems = nullptr; // after std::move, no need to change r-value's pointer NOTE: why
    rhs._logical_size = 0;
    rhs._buffer_size = 0;
    rhs._cursor_index = 0;
    rhs._gap_size = 0;

    return *this;
}

template <typename T>
void GapBuffer<T>::insert_at_cursor(value_type&& element) {
    // TODO: implement this insert function (takes in an r-value) (~7 lines long)
    // insert_at_cursor(element); // by default, calls the l-value version above
    // when you are ready to implement, remove the insert_at_cursor call.

    if(_gap_size == 0) reserve(2 * _buffer_size);

    _elems[_cursor_index] = std::move(element);
    _cursor_index ++;
    _logical_size ++;
    _gap_size --;
}

// Part 8: Make your code RAII-compliant - change the code throughout

// optional:
template <typename T>
template <typename... Args>
void GapBuffer<T>::emplace_at_cursor(Args&&... args) {
    // TODO: optional: implement function
    // remember to perfectly forward the arguments to the constructor of T.

    if (_gap_size == 0) {
        reserve(2 * _buffer_size);
    }

    _elems[_cursor_index] = T(std::forward<Args>(args)...);
    _cursor_index++;
    _logical_size++;
    _gap_size--;
}


// We've implemented the following functions for you.
// However...they do use raw pointers, so you might want to turn them into smart pointers!
template <typename T>
void GapBuffer<T>::move_cursor(int delta) {
    int new_index = _cursor_index + delta;
    if (new_index < 0 || new_index > static_cast<int>(_buffer_size)) {
        throw std::string("move_cursor: delta moves cursor out of bounds");
    }

    T* raw_elems = _elems.get();
    if (delta > 0) {
        auto begin_move = raw_elems + _cursor_index + _gap_size;
        auto end_move = begin_move + delta;
        auto destination = raw_elems + _cursor_index;
        std::move(begin_move, end_move, destination);
    } else {
        auto end_move = raw_elems + _cursor_index;
        auto begin_move = end_move + delta;
        auto* destination = raw_elems + _cursor_index + _gap_size + delta;
        std::move(begin_move, end_move, destination);
    }
    _cursor_index += delta;
}

template <typename T>
void GapBuffer<T>::reserve(size_type new_size) {
    if (_logical_size >= new_size) return;
    auto new_elems = std::make_unique<T[]>(new_size);

    std::move(_elems.get(), _elems.get() + _cursor_index, new_elems.get());
    
    size_t new_gap_size = new_size - _logical_size;
    std::move(_elems.get() + _buffer_size - _logical_size + _cursor_index,
              _elems.get() + _buffer_size,
              new_elems.get() + _cursor_index + new_gap_size);
    
              _buffer_size = new_size;
    // delete [] _elems; // in raii, no need to delete, NOTE: why ?
    _elems = std::move(new_elems);
    _gap_size = new_gap_size;
}

template <typename T>
void GapBuffer<T>::debug() const {
    std::cout << "[";
    for (size_t i = 0; i < _buffer_size; ++i) {
        if (i == _cursor_index) {
            std::cout << "|";
        } else {
            std::cout << " ";
        }
        if (i >= _cursor_index && i < _cursor_index + _gap_size) {
            std::cout << "*";
        } else {
            std::cout << _elems[i];
        }
    }
    std::cout << (_cursor_index == _buffer_size ? "|" : " ");
    std::cout << "]" << std::endl;
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::to_external_index(size_type array_index) const {
    if (array_index < _cursor_index) {
        return array_index;
    } else if (array_index >= _cursor_index + _gap_size){
        return array_index - _cursor_index;
    } else {
        throw ("to_external_index: array_index is out of bounds!");
    }
}

template <typename T>
typename GapBuffer<T>::size_type GapBuffer<T>::to_array_index(size_type external_index) const {
    if (external_index < _cursor_index) {
        return external_index;
    } else {
        return external_index + _gap_size;
    }
}


#endif // GAPBUFFER_H
