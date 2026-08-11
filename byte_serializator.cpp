#include "byte_serializator.hpp"
#include <cstddef>
#include <cstring>


bool ByteS::expand(size_t size) {
    if(_data == nullptr) {
        this->_data = new unsigned char[size]();
        allocated += size;
        return true;
    }
    size_t old_s = allocated;
    allocated += size;

    unsigned char* old_d = _data;
    this->_data = new unsigned char[allocated];

    memcpy(_data, old_d, old_s);

    delete [] old_d;
    return true;
}


bool ByteS::collapse(size_t __s) {
    if (_data == nullptr || __s > allocated) {
        return false;
    }

    allocated -= __s;
    if(allocated == 0) {
        delete [] _data;
        _size = 0;
        _data = nullptr;
        return true;
    }

    unsigned char* n_data = new unsigned char[allocated]();
    memcpy(n_data, _data, allocated);

    delete [] _data;
    _data = n_data;
    if (_size > allocated) {
        _size = allocated;
    }
    return true;
}



ByteS::ByteS(): _data(nullptr), _size(0), allocated(0) {}
ByteS::~ByteS() {
    if (_data != nullptr) {
        delete [] _data;
    }
}


ByteS& ByteS::operator<<(const ByteS& other) {
    this->append((char*) other._data, other._size);
    return *this;
}



ByteS& ByteS::append(char* _src, size_t __s) {
    if (_size + __s > allocated) {
        expand(_size + __s - allocated);
    }

    unsigned char* data_ptr = _data + _size;
    memcpy(data_ptr, _src, __s);
    _size += __s;

    return *this;
}


ByteS& ByteS::append(std::string& __src) {
    size_t other_size = __src.size();

    this->append( (char*) &other_size, sizeof(size_t));

    expand(other_size);
    memcpy(this->_data + _size, __src.data(), other_size);
    _size += other_size;

    return *this;
}


bool ByteS::pop(std::string& __dest) {
    if(_size < sizeof(__dest)) {return false;}

    size_t other_size;
    this->pop( (char*) &other_size, sizeof(size_t));

    __dest.resize(other_size);
    memcpy(__dest.data(), this->_data, other_size);
    memmove(_data, _data + other_size, _size);
    collapse(other_size);

    return true;
}


bool ByteS::pop(char* __dest, size_t __s) {
    if(_size < __s) {
        return false;
    }

    unsigned char* data_ptr = _data;
    memcpy(__dest, data_ptr, __s);
    _size -= __s;
    memmove(_data, _data + __s, _size);
    collapse(__s);

    return true;
}

unsigned char* ByteS::data() {return _data;}
size_t ByteS::size() {return _size;}
