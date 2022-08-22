#pragma once

#include <stddef.h>
#include <vector>

namespace simplemuduo
{
    class Buffer
    {
    private:
        std::vector<char> m_buffer;

    public:
        Buffer();
        ~Buffer();
        void append(const char* p, const size_t& len);
        void setBuf(const char* p);
        int getSize();
        const char* getBuf();
        void clear();
    };
}