#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdint>

std::vector<unsigned char> base64_decode(const std::string &input) {
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    
    std::vector<unsigned char> output;
    
    if (input.size() % 4 != 0) {
        std::cerr << "Неверная длина base64 строки." << std::endl;
        return output;
    }
    
    for (size_t i = 0; i < input.size();) {
        uint32_t a = input[i] == '=' ? 0 : base64_chars.find(input[i]);
        ++i;
        uint32_t b = input[i] == '=' ? 0 : base64_chars.find(input[i]);
        ++i;
        uint32_t c = input[i] == '=' ? 0 : base64_chars.find(input[i]);
        ++i;
        uint32_t d = input[i] == '=' ? 0 : base64_chars.find(input[i]);
        ++i;
        
        uint32_t triple = (a << 18) | (b << 12) | (c << 6) | d;
        
        output.push_back((triple >> 16) & 0xFF);
        if (input[i - 2] != '=') {
            output.push_back((triple >> 8) & 0xFF);
        }
        if (input[i - 1] != '=') {
            output.push_back(triple & 0xFF);
        }
    }
    return output;
}

int main() {
	const std::size_t bufferSize = 4096;
	std::vector<char> inputData;
	char buffer[bufferSize];

	while (std::cin) {
		std::cin.read(buffer, bufferSize);
		std::streamsize bytesRead = std::cin.gcount();
		if (bytesRead > 0) {
			inputData.insert(inputData.end(), buffer, buffer + bytesRead);
		}
	}

	std::string encoded;
	for(char c : inputData) {
		if (!std::isspace(static_cast<unsigned char>(c))) {
			encoded.push_back(c);
		}
	}

	std::vector<unsigned char> decoded = base64_decode(encoded);
	std::cout.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
	return 0;
}