#include "./include/Buffer.h"

using namespace simplemuduo;

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::append(const char* p, const size_t& len)
{
    for (size_t i = 0; i < len; ++i)
    {
        m_buffer.push_back(p[i]);
    }
}

void Buffer::setBuf(const char* p)
{
    m_buffer.clear();
    for (;;)
    {
        m_buffer.push_back(*p);
        if (*p != '0')
            p++;
        else
            break;
    }
}

int Buffer::getSize()
{
    return static_cast<int>(m_buffer.size());
}

const char* Buffer::getBuf()
{
    return &*m_buffer.begin();
}

void Buffer::clear()
{
    m_buffer.clear();
    m_buffer.swap(m_buffer);
}