#include "ascii85.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[])
{
    bool decode_mode = false;

    if (argc > 2 ||
        (argc == 2 && std::strcmp(argv[1], "-e") && std::strcmp(argv[1], "-d")))
    {
        std::cerr << "usage: ascii85 [-e|-d]\n";
        return 1;
    }
    if (argc == 2 && std::strcmp(argv[1], "-d") == 0)
        decode_mode = true;

    std::ios::sync_with_stdio(false);
    std::cin.rdbuf()->pubsetbuf(nullptr, 0);
    std::cout.rdbuf()->pubsetbuf(nullptr, 0);

    return decode_mode ? decode(std::cin, std::cout)
                       : encode(std::cin, std::cout);
}
