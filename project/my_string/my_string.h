//
// Created by nigo on 2021/1/21.
//

#ifndef MY_STRING__MY_STRING_H_
#define MY_STRING__MY_STRING_H_

#include <cstring>
#include "my_iterator.h"
#include "my_allocator.h"

#define DEFAULT_CAPACITY 20

class my_string {
 public:

  typedef char value_type;
  typedef std::char_traits<char> traits_type;
  typedef my_allocator<char> allocator_type;
  typedef char& reference;
  typedef const char& const_reference;
  typedef char* pointer;
  typedef const char* const_pointer;
  typedef random_access_iterator<char> iterator;
  typedef random_access_iterator<const char> const_iterator;
  typedef reverse_random_access_iterator<char> reverse_iterator;
  typedef reverse_random_access_iterator<const char> const_reverse_iterator;
  typedef ptrdiff_t diffence_type;
  typedef size_t size_type;

  static const size_type npos = -1;

 private:

  size_type _capacity = DEFAULT_CAPACITY;
  size_type _size = 0;
  my_allocator<char> _alloc;
  pointer _buffer;

 public:

  my_string() {
    _buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
  }

  my_string(const my_string &str) {
    _capacity = str._capacity;
    _size = str._size;
    _alloc.deallocate(_buffer);
    _buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = *(str._buffer + i);
	* (_buffer + _capacity - 1) = '\0';
  }

  my_string(const my_string &str, size_type pos, size_type len = npos) {
    if (pos >= str._size) {
	  _buffer = _alloc.allocate(_capacity);
	  memset(_buffer, 0, _capacity);
    }
    else {
      len = pos + len < str._size ?
      	len : str._size - pos;
      for (size_type i = 0; i * DEFAULT_CAPACITY <= len; i++)
      	_capacity = (i+1) * DEFAULT_CAPACITY;
      _size = len;
      _buffer = _alloc.allocate(_capacity);
      memset(_buffer, 0, _capacity);
	  for (size_type i = 0; i < _size; i++)
		*(_buffer + i) = *(str._buffer + i + pos);
	  * (_buffer + _capacity - 1) = '\0';
	}
  }

  my_string(const char *s) {
	_size = strlen(s);
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size; i++)
	  _capacity = (i+1) * DEFAULT_CAPACITY;
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = *(s + i);
	* (_buffer + _capacity - 1) = '\0';
  }

  my_string(const char *s, size_type n) {
    _size = n * strlen(s);
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size; i++)
	  _capacity = (i+1) * DEFAULT_CAPACITY;
	_size = strlen(s);
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0, j = 0; i < _size; i++) {
	  if (j == strlen(s) - 1)
	    j = 0;
	  *(_buffer + i) = *(s + j);
	}
	* (_buffer + _capacity - 1) = '\0';
  }

  my_string(size_type n, char c) {
	_size = n;
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size; i++)
	  _capacity = (i+1) * DEFAULT_CAPACITY;
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = c;
	* (_buffer + _capacity - 1) = '\0';
  }

  template <class InputIterator>
  my_string(InputIterator first, InputIterator last) {
    _size = last - first;
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size; i++)
	  _capacity = (i+1) * DEFAULT_CAPACITY;
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = *(first++);
	* (_buffer + _capacity - 1) = '\0';
  }

  my_string (my_string &&str) noexcept {
	_capacity = str._capacity;
	_size = str._size;
	_alloc.deallocate(_buffer);
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = *(str._buffer + i);
	* (_buffer + _capacity - 1) = '\0';
  }

  ~my_string() {
    _size = 0;
    _capacity = 0;
    _alloc.deallocate(_buffer);
  }

  my_string &operator=(const my_string &str) {
	//必须考虑是否为本身
	if ((_capacity == str._capacity) && (_buffer == str._buffer) && (_size == str._size))
	  return *this;
	_capacity = str._capacity;
	_size = str._size;
	_alloc.deallocate(_buffer);
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = *(str._buffer + i);
	* (_buffer + _capacity - 1) = '\0';
  }

  my_string &operator=(const char *s) {
	_size = strlen(s);
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size; i++)
	  _capacity = (i+1) * DEFAULT_CAPACITY;
	_alloc.deallocate(_buffer);
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = *(s + i);
	* (_buffer + _capacity - 1) = '\0';
  }

  my_string &operator=(char c) {
    _size = 1;
    _capacity = DEFAULT_CAPACITY;
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	*_buffer = c;
  }

  my_string &operator=(std::initializer_list<char> il) {
    _size = il.end() - il.begin();
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size; i++)
	  _capacity = (i+1) * DEFAULT_CAPACITY;
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	int i = 0;
    for (auto &c: il) {
	  *(_buffer + i) = c;
	  i++;
	}
	* (_buffer + _capacity - 1) = '\0';
  }

  my_string &operator=(my_string &&str) noexcept {
	_capacity = str._capacity;
	_size = str._size;
	_alloc.deallocate(_buffer);
	_buffer = _alloc.allocate(_capacity);
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < _size; i++)
	  *(_buffer + i) = *(str._buffer + i);
	* (_buffer + _capacity - 1) = '\0';
  }

  iterator begin() noexcept {
    return iterator(_buffer, _size);
  }

  const_iterator begin() const noexcept {
	return const_iterator(_buffer, _size);
  }

  iterator end() noexcept {
	return iterator(_buffer, _size, _size);
  }

  const_iterator end() const noexcept {
	return const_iterator(_buffer, _size, _size);
  }

  reverse_iterator rbegin() noexcept {
	return reverse_iterator(_buffer + _size - 1, _size);
  }

  const_reverse_iterator rbegin() const noexcept {
	return const_reverse_iterator(_buffer + _size - 1, _size);
  }

  reverse_iterator rend() noexcept {
	return reverse_iterator(_buffer + _size - 1, _size, _size);
  }

  const_reverse_iterator rend() const noexcept {
	return const_reverse_iterator(_buffer + _size - 1, _size, _size);
  }

  const_iterator cbegin() const noexcept {
	return const_iterator(_buffer, _size);
  }

  const_iterator cend() const noexcept {
	return const_iterator(_buffer, _size, _size);
  }

  const_reverse_iterator crbegin() const noexcept {
	return const_reverse_iterator(_buffer + _size - 1, _size);
  }

  const_reverse_iterator crend() const noexcept {
	return const_reverse_iterator(_buffer + _size - 1, _size, _size);
  }

  size_type size() const noexcept {
    return _size;
  }

  size_type length() const noexcept {
    return _size;
  }

  size_type max_size() const noexcept {
	return npos;
  }

  void resize (size_type n) {
    do_resize(n, '\0');
  }

  void resize (size_type n, value_type c) {
    do_resize(n, c);
  }

  size_t capacity() const noexcept {
    return _capacity;
  }

  void reserve (size_t n = 0) {
    if (n > max_size())
	  throw std::length_error("maximum exceeded");
    pointer new_buffer = _alloc.allocate(n);
    if (new_buffer == nullptr)
      throw std::bad_alloc();
    pointer old_buffer = _buffer;
    _buffer = new_buffer;
    _capacity = n;
    memset(_buffer, 0, _capacity);
    for (size_type i = 0; i < _capacity; i++)
      *(new_buffer + i) = *(old_buffer + i);
	* (_buffer + _capacity - 1) = '\0';
	if (n < size())
	  _size = n;
  }

  void clear() noexcept {
    memset(_buffer, 0, _capacity);
    _size = 0;
  }

  bool empty() const noexcept {
    return _size == 0;
  }

  void shrink_to_fit() {
    size_type n;
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size; i++)
	  n = (i+1) * DEFAULT_CAPACITY;
    this->resize(n);
  }

  reference operator[] (size_t pos) {
    return *(_buffer + pos);
  }

  const_reference operator[] (size_t pos) const {
	return *(_buffer + pos);
  }

  reference at (size_t pos) {
	if (pos >= _capacity)
	  throw std::out_of_range("cross the border");
	return *(_buffer + pos);
  }

  const_reference at (size_t pos) const {
	if (pos >= _capacity)
	  throw std::out_of_range("cross the border");
	return *(_buffer + pos);
  }

  reference back() {
	if (_size == 0)
	  throw std::logic_error("empty my_string");
	return *(_buffer + _size - 1);
  }

  const_reference back() const {
	if (_size == 0)
	  throw std::logic_error("empty my_string");
	return *(_buffer + _size - 1);
  }

  reference front() {
	if (_size == 0)
	  throw std::logic_error("empty my_string");
	return *_buffer;
  }

  const_reference front() const {
	if (_size == 0)
	  throw std::logic_error("empty my_string");
	return *_buffer;
  }

  my_string &operator+= (const my_string &str) {
	return *this = do_addition(*this, str);
  }

  my_string &operator+= (const_pointer str) {
	return *this = do_addition(*this, str);
  }

  my_string &operator+= (value_type str) {
    char temp[] = {str, '\0'};
    return *this = do_addition(*this, temp);
  }

  my_string &operator+= (std::initializer_list<char> il) {
    my_string temp(il.begin(), il.end());
    return *this = do_addition(*this, temp);
  }

  my_string &append (const my_string &str) {
    return *this = do_addition(*this, str);
  }
  
  my_string &append (const my_string &str, size_t pos, size_t len) {
    if (pos >= str._size)
	  throw std::out_of_range("cross the border");
    return append(my_string(str, pos, len));
  }

  my_string &append (const char* s) {
    return append(s);
  }

  my_string &append (const char* s, size_t n) {
	return append(my_string(s, n));
  }

  my_string &append (size_t n, char c) {
	return append(my_string(n, c));
  }

  void push_back(value_type c) {
    append(my_string(1, c));
  }
  
  template <class InputIterator>
  my_string &append (InputIterator first, InputIterator last) {
    return append(my_string(first, last));
  }

  my_string &append (std::initializer_list<char> il) {
    return append(my_string(il.begin(), il.end()));
  }

  my_string& assign (const my_string& str) {
    return *this = my_string(str);
  }

  my_string& assign (const my_string& str, size_t pos, size_t len) {
	return *this = my_string(str, pos, len);
  }

  my_string& assign (const char* s) {
	return *this = my_string(s);
  }

  my_string& assign (const char* s, size_t n) {
	return *this = my_string(s, n);
  }

  my_string& assign (size_t n, char c) {
	return *this = my_string(n, c);
  }

  template <class InputIterator>
  my_string& assign (InputIterator first, InputIterator last) {
	return *this = my_string(first, last);
  }

  my_string& assign (std::initializer_list<char> il) {
	return *this = my_string(il.begin(), il.end());
  }

  my_string& assign (my_string&& str) noexcept {
	return *this = my_string(str);
  }

  my_string& insert (size_t pos, const my_string& str) {
    if (pos >= _size)
      throw std::out_of_range("cross the border");
	for (size_type i = 0; i * DEFAULT_CAPACITY <= _size + str._size; i++)
	  _capacity = (i+1) * DEFAULT_CAPACITY;
	pointer new_buffer = _alloc.allocate(_capacity);
	if (new_buffer == nullptr)
	  throw std::bad_alloc();
	pointer old_buffer = _buffer;
	_buffer = new_buffer;
	memset(_buffer, 0, _capacity);
	for (size_type i = 0; i < pos; i++)
	  *(_buffer + i) = *(old_buffer + i);
	for (size_type i = 0; i < str._size; i++)
	  *(_buffer + i + pos) = *(str._buffer + i);
	for (size_type i = 0; i < _size - pos; i++)
	  *(_buffer + i + pos + str._size) = *(old_buffer + i + pos);
	_size += str._size;
	return *this;
  }

  my_string& insert (size_t pos, const my_string& str, size_t subpos, size_t len) {
    return insert(pos, my_string(str, subpos, len));
  }

  my_string& insert (size_t pos, const char* s) {
	return insert(pos, s);
  }

  my_string& insert (size_t pos, const char* s, size_t n) {
	return insert(pos, my_string(s, n));
  }

  my_string& insert (size_t pos, size_t n, char c) {
	return insert(pos, my_string(n, c));
  }

  iterator insert (iterator p, size_t n, char c) {
	insert(p - begin(), my_string(n, c));
	return p + n;
  }

  iterator insert (iterator p, char c) {
	insert(p - begin(), my_string(1, c));
	return p+1;
  }

  template <class InputIterator>
  iterator insert (iterator p, InputIterator first, InputIterator last) {
	insert(p - begin(), my_string(first, last));
	return p + (last - first);
  }

  my_string& erase (size_t pos = 0, size_t len = npos) {
	if (pos >= _size)
	  throw std::out_of_range("cross the border");
	if (len == 0)
	  return *this;
	if (len + pos > _size)
	  len = _size - pos - 1;
	for (size_t i = 0; i < _size; i++)
		_buffer[i + pos] = _buffer[i + pos + len];
	_size -= len;
	_buffer[_size] = '\0';
	return *this;
  }

  iterator erase (iterator p) {
    erase(p - begin(), 1);
    return p;
  }

  iterator erase (iterator first, iterator last) {
    erase(first - begin(), last - first);
    return first;
  }
  my_string& replace (size_t pos,        size_t len,        const my_string& str);
  my_string& replace (const_iterator i1, const_iterator i2, const my_string& str);

  my_string& replace (size_t pos,        size_t len,        const my_string& str,
				   size_t subpos, size_t sublen);

  my_string& replace (size_t pos,        size_t len,        const char* s);
  my_string& replace (const_iterator i1, const_iterator i2, const char* s);

  my_string& replace (size_t pos,        size_t len,        const char* s, size_t n);
  my_string& replace (const_iterator i1, const_iterator i2, const char* s, size_t n);

  my_string& replace (size_t pos,        size_t len,        size_t n, char c);
  my_string& replace (const_iterator i1, const_iterator i2, size_t n, char c);

  template <class InputIterator>
  my_string& replace (const_iterator i1, const_iterator i2,
				   InputIterator first, InputIterator last);

  my_string& replace (const_iterator i1, const_iterator i2, std::initializer_list<char> il);


 private:

  void do_resize (size_type n, value_type c) {
	if (n == _size)
	  return;
	if (n > max_size())
	  throw std::length_error("maximum exceeded");
	if (n < _size) {
	  for (size_type i = n; i < _size; i++)
		*(_buffer + i) = c;
	} else if (n < _capacity - 1) {
	  for (size_type i = _size; i <= n; i++)
		*(_buffer + i) = c;
	} else {
	  for (size_type i = 0; i * DEFAULT_CAPACITY <= n; i++)
		_capacity = (i+1) * DEFAULT_CAPACITY;
	  pointer new_buffer = _alloc.allocate(_capacity);
	  if (new_buffer == nullptr)
		throw std::bad_alloc();
	  pointer old_buffer = _buffer;
	  _buffer = new_buffer;
	  memset(_buffer, 0, _capacity);
	  for (size_type i = 0; i < _size; i++)
		*(_buffer + i) = *(old_buffer + i);
	  for (size_type i = _size; i <= n; i++)
		*(_buffer + i) = c;
	}
	_size = n;
  }

  static my_string do_addition(const my_string &s1, const my_string &s2) {
    char temp[s1._size + s2._size + 1];
    memset(temp, 0, sizeof(temp));
    for (int i = 0; i < s1._size; i++)
      temp[i] = s1[i];
    for (int i = s1._size; i < s1._size + s2._size; i++)
      temp[i] = s2[i];
    return my_string(temp);
  }
};

#endif //MY_STRING__MY_STRING_H_
