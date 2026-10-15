#pragma once
#include <cstddef>
#include <string>


class ByteS {
    unsigned char* _data;
    size_t inner_size;
    size_t allocated;


    bool expand(size_t __s);
    bool collapse(size_t __s);
    bool collapse();

    bool expandBloat(size_t __s);
    size_t calcNeedSize(size_t __s);

    public:
    ByteS();
    ~ByteS();

    ByteS& operator<<(const ByteS& other);

    ByteS& append(char* _src, size_t __s);
    ByteS& append(std::string& __src);

    template<typename T>
    ByteS& append(const T& _src) {return this->append( (char*) &_src, sizeof(T));}

    bool pop(char* __dest, size_t __s);
    bool pop(std::string& __src);

    template<typename T>
    bool pop(T& __dest) {return this->pop( (char*) &__dest, sizeof(T));}

    unsigned char* data();
    size_t size();
};
