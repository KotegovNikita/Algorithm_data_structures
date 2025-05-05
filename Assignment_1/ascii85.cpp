#include "ascii85.hpp"
#include <array>
#include <cstdint>
#include <cctype>

namespace {

constexpr int OK  = 0;
constexpr int ERR = 1;

inline void put_word(std::ostream& out, uint32_t w)
{
    if (w == 0) { out.put('z'); return; }
    std::array<char,5> buf;
    for (int i = 4; i >= 0; --i) { buf[i] = static_cast<char>(w % 85 + 33); w /= 85; }
    out.write(buf.data(), 5);
}

inline int value85(int c) { return (c >= 33 && c <= 117) ? c - 33 : -1; }

} 

int encode(std::istream& in, std::ostream& out)
{
    std::array<unsigned char,4> buf{};
    std::size_t have = 0;

    while (true) {
        int byte = in.get();
        if (byte != EOF) buf[have++] = static_cast<unsigned char>(byte);

        if (have == 4 || (byte == EOF && have)) {
            uint32_t w = (buf[0] << 24) | (buf[1] << 16) |
                         (buf[2] <<  8) |  buf[3];
            if (have < 4) {                        
                std::array<char,5> tmp;
                for (int i = 4; i >= 0; --i) { tmp[i] = static_cast<char>(w % 85 + 33); w /= 85; }
                out.write(tmp.data(), static_cast<std::streamsize>(have + 1));
            } else {
                put_word(out, w);
            }
            have = 0;
            buf.fill(0);
        }
        if (byte == EOF) break;
    }
    return OK;
}

int decode(std::istream& in, std::ostream& out)
{
    std::array<int,5> digits{};
    int n = 0;

    while (true) {
        int c = in.get();
        if (c == EOF) break;

        if (c == 'z') {
            if (n) return ERR;                  
            unsigned char zero[4] = {0,0,0,0};
            out.write(reinterpret_cast<char*>(zero), 4);
            continue;
        }
        if (std::isspace(c)) continue;

        int v = value85(c);
        if (v < 0) return ERR;
        digits[n++] = v;

        if (n == 5) {
            uint32_t w = 0;
            for (int i = 0; i < 5; ++i) w = w * 85 + digits[i];
            std::array<unsigned char,4> bytes{
                static_cast<unsigned char>((w >> 24) & 0xFF),
                static_cast<unsigned char>((w >> 16) & 0xFF),
                static_cast<unsigned char>((w >>  8) & 0xFF),
                static_cast<unsigned char>( w        & 0xFF)
            };
            out.write(reinterpret_cast<char*>(bytes.data()), 4);
            n = 0;
        }
    }

    if (n) {                            
        if (n == 1) return ERR;         
        for (int i = n; i < 5; ++i) digits[i] = 84;  
        uint32_t w = 0;
        for (int i = 0; i < 5; ++i) w = w * 85 + digits[i];
        for (int i = 0; i < n - 1; ++i)
            out.put(static_cast<char>((w >> (24 - 8 * i)) & 0xFF));
    }
    return OK;
}
