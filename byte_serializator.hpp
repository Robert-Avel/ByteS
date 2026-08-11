#pragma once
#include <cstddef>
#include <string>


class ByteS {
    unsigned char* _data;
    size_t _size;
    size_t allocated;

    bool expand(size_t __s);
    bool collapse(size_t __s);

    public:
    ByteS();
    ~ByteS();

    ByteS& operator<<(const ByteS& other);

    ByteS& append(char* _src, size_t __s);
    ByteS& append(std::string& __src);

    bool pop(char* __dest, size_t __s);
    bool pop(std::string& __src);


    unsigned char* data();
    size_t size();
};
