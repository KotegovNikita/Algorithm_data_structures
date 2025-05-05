#include "ascii85.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <random>
#include <vector>
#include <cstring>

/* ----------- вспомогалки ----------- */
static std::string enc(const std::string& bin)
{
    std::istringstream in(bin, std::ios::binary);
    std::ostringstream out;
    EXPECT_EQ(encode(in, out), 0);
    return out.str();
}

static std::string dec(const std::string& a85, int expect_rc = 0)
{
    std::istringstream in(a85);
    std::ostringstream out(std::ios::binary);
    EXPECT_EQ(decode(in, out), expect_rc);
    return out.str();
}

/* ----------- базовые случаи ----------- */
TEST(Ascii85, Empty)      { EXPECT_EQ(dec(enc("")), ""); }
TEST(Ascii85, Hello)      { EXPECT_EQ(dec(enc("Hello")), "Hello"); }

TEST(Ascii85, ZeroZ)
{
    EXPECT_EQ(dec("z"), std::string(4, '\0'));
}

TEST(Ascii85, BadInput)
{
    std::istringstream in("!!!!"); std::ostringstream out;
    EXPECT_EQ(decode(in, out), 1);
}

/* ----------- случайные данные ----------- */
TEST(Ascii85, RandomRoundTrip)
{
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> len(0, 256);
    std::uniform_int_distribution<int> byte(0, 255);

    for (int it = 0; it < 100; ++it) {
        std::string src;
        src.resize(len(rng));
        for (char& c : src) c = static_cast<char>(byte(rng));

        auto a85 = enc(src);
        auto back = dec(a85);
        EXPECT_EQ(back, src) << "iteration " << it;
    }
}
