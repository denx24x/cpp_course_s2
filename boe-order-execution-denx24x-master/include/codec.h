#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

inline uint8_t decode_uint8(const unsigned char * start)
{
    return *start;
}

inline uint16_t decode_uint16(const unsigned char * start)
{
    return (static_cast<uint32_t>(decode_uint8(start + 1)) << 8) + decode_uint8(start);
}

inline uint32_t decode_uint32(const unsigned char * start)
{
    return (static_cast<uint32_t>(decode_uint16(start + 2)) << 16) + decode_uint16(start);
}

inline uint64_t decode_uint64(const unsigned char * start)
{
    return (static_cast<uint64_t>(decode_uint32(start + 4)) << 32) + decode_uint32(start);
}

inline int64_t decode_int64(const unsigned char * start)
{
    return static_cast<int64_t>(decode_uint64(start));
}

inline std::string decode_str(const unsigned char * start, const size_t field_size)
{
    auto char_start = reinterpret_cast<const char *>(start);
    auto pos = std::find(char_start, char_start + field_size, '\0');
    return std::string(char_start, pos - char_start);
}

inline unsigned char * encode(unsigned char * start, const uint8_t value)
{
    *start = value;
    return start + 1;
}

inline unsigned char * encode(unsigned char * start, const uint16_t value)
{
    *start++ = static_cast<unsigned char>(value & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 8) & 0xFF);
    return start;
}

inline unsigned char * encode(unsigned char * start, const uint32_t value)
{
    *start++ = static_cast<unsigned char>(value & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 8) & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 16) & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 24) & 0xFF);
    return start;
}

inline unsigned char * encode(unsigned char * start, int64_t value)
{
    for (int i = 0; i < 8; ++i) {
        *start++ = static_cast<unsigned char>(value & 0xFF);
        value >>= 8;
    }
    return start;
}

inline unsigned char * encode(unsigned char * start, const std::string & str, const size_t field_size)
{
    size_t i = 0;
    while (i < str.size() && i < field_size) {
        *start++ = str[i];
        ++i;
    }
    while (i < field_size) {
        *start++ = '\0';
        ++i;
    }
    return start;
}
