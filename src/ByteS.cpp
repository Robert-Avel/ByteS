#include "ByteS.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>




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

    std::cout << "Expand: " << size << "\n";
    std::cout << "Size: " << inner_size << " | Allocated: " << allocated << "\n\n";

    return true;
}


bool ByteS::collapse(size_t size) {
    if (_data == nullptr || size > allocated) {
        return false;
    }

    allocated -= size;
    if(allocated == 0) {
        delete [] _data;
        inner_size = 0;
        _data = nullptr;
        return true;
    }

    unsigned char* n_data = new unsigned char[allocated]();
    memcpy(n_data, _data, allocated);

    delete [] _data;
    _data = n_data;
    if (inner_size > allocated) {
        inner_size = allocated;
    }

    std::cout << "Colapse: " << size << "\n";
    std::cout << "Size: " << inner_size << " | Allocated: " << allocated << "\n\n";

    return true;
}


bool ByteS::collapse() {
    if(inner_size == 0) {
        return false;
    }
    if(allocated / inner_size > 5) {
        return collapse(inner_size * 4);
    }
    return false;
}



bool ByteS::expandBloat(size_t __s) {
    return expand(__s * 5);
}


size_t ByteS::calcNeedSize(size_t __s) {
    if (inner_size + __s > allocated) {
        return inner_size + __s - allocated;
    } else return 0;
}



ByteS::ByteS(): _data(nullptr), inner_size(0), allocated(0) {}
ByteS::~ByteS() {
    if (_data != nullptr) {
        delete [] _data;
    }
}


ByteS& ByteS::operator<<(const ByteS& other) {
    this->append((char*) other._data, other.inner_size);
    return *this;
}



ByteS& ByteS::append(char* _src, size_t __s) {
    auto n_size = calcNeedSize(__s);
    if (n_size != 0) {
        expandBloat(n_size);
    }

    unsigned char* data_ptr = _data + inner_size;
    memcpy(data_ptr, _src, __s);
    inner_size += __s;

    std::cout << "Appended: " << __s << "\n";
    std::cout << "Size: " << inner_size << " | Allocated: " << allocated << "\n\n";

    return *this;
}


ByteS& ByteS::append(std::string& __src) {
    auto n_size = calcNeedSize(sizeof(size_t) + __src.size());
    if (n_size != 0) {
        expandBloat(n_size);
    }

    size_t other_size = __src.size();

    this->append( (char*) &other_size, sizeof(size_t));
    this->append(__src.data(), other_size);

    return *this;
}


bool ByteS::pop(std::string& __dest) {
    if(inner_size < sizeof(size_t)) {return false;}

    size_t other_size;
    this->pop( (char*) &other_size, sizeof(size_t));

    if(inner_size < other_size) {
        this->append((char*) &other_size, sizeof(size_t));
        return false;
    }

    __dest.resize(other_size);
    this->pop(__dest.data(), other_size);

    collapse();

    return true;
}


bool ByteS::pop(char* __dest, size_t __s) {
    if(inner_size < __s) {
        return false;
    }

    unsigned char* data_ptr = _data;
    memcpy(__dest, data_ptr, __s);
    inner_size -= __s;
    memmove(_data, _data + __s, inner_size);

    std::cout << "Poped: " << __s << "\n";
    std::cout << "Size: " << inner_size << " | Allocated: " << allocated << "\n\n";

    collapse();

    return true;
}

unsigned char* ByteS::data() {return _data;}
size_t ByteS::size() {return inner_size;}
