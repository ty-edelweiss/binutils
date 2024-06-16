#include "hex.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <ios>
#include <iostream>
#include <memory>
#include <string>

struct Options {
    /* non-functional options */
    bool help = false;
    bool version = false;

    /* functional options */
    bool reverse = false;
    int pos = 0;
    int num = 0;
};

void printUsage() {
    std::cout << "Usage: hex [options]" << std::endl;
    std::cout << "  " << std::endl;
    std::cout << "  Print hex for the input" << std::endl;
    std::cout << "  " << std::endl;
    std::cout << "Allowed options:" << std::endl;
    std::cout << "  -h --help            print help messages" << std::endl;
    std::cout << "  -v --version         print program version" << std::endl;
    std::cout << "  -r --reverse         inverse transform from hex" << std::endl;
    std::cout << "  -p --pos bytes       starting position to read in bytes" << std::endl;
    std::cout << "                       relative to the end of the input if negative" << std::endl;
    std::cout << "  -n --num bytes       bytes to read from starting position" << std::endl;
}

void printVersion() {
    std::cout << HEX_VERSION << std::endl;
}

std::unique_ptr<Options> parseOptions(int argc, char const* argv[]) {
    std::unique_ptr<Options> opts = std::make_unique<Options>();
    for (int i = 0; i < argc; i++) {
        if (!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "--help")) {
            opts->help = true;
        } else if (!std::strcmp(argv[i], "-v") ||
                   !std::strcmp(argv[i], "--version")) {
            opts->version = true;
        } else if (!std::strcmp(argv[i], "-r") ||
                   !std::strcmp(argv[i], "--reverse")) {
            opts->reverse = true;
        } else if (!std::strcmp(argv[i], "-p") ||
                   !std::strcmp(argv[i], "--pos")) {
            if (i >= argc - 1) continue;
            try {
                opts->pos = std::stoi(argv[++i]);
            } catch (const std::invalid_argument& ex) {
                std::cerr << ex.what() << std::endl;
                std::exit(1);
            }
        } else if (!std::strcmp(argv[i], "-n") ||
                   !std::strcmp(argv[i], "--num")) {
            if (i >= argc - 1) continue;
            try {
                opts->num = std::stoi(argv[++i]);
            } catch (const std::invalid_argument& ex) {
                std::cerr << ex.what() << std::endl;
                std::exit(1);
            }
        } else {
            // skip
        }
    }
    return opts;
}

int hexize(std::string& bin, int pos = 0, int n = 0) {
    if (pos < 0) {
        pos = bin.size() + pos;
    }

    if (n <= 0) {
        n = bin.size();
    }

    while (n > 0 && pos < bin.size()) {
        unsigned char tmp = bin[pos];
        unsigned int hex1 = (tmp & 0x00f0) >> 4;
        unsigned int hex2 = (tmp & 0x000f);
        std::cout << std::hex << hex1 << hex2;
        pos++, n--;
    }
    std::cout << std::endl;

    return 0;
}

int unhexize(std::string& hex, int pos = 0, int n = 0) {
    if ((hex.size() % 2) || std::any_of(hex.begin(), hex.end(), [](char& c) {
            return !std::isxdigit(c);
        })) {
        std::cerr << "hex is invalid." << std::endl;
        return -1;
    }

    if (pos < 0) {
        pos = hex.size() + pos;
    }

    if (n <= 0) {
        n = hex.size();
    }

    while (n > 0 && pos < hex.size()) {
        std::string tmp = hex.substr(pos, 2);
        unsigned char bin = std::stoi(tmp, nullptr, 16);
        std::cout << bin;
        pos += 2, n -= 2;
    }
    std::cout << std::endl;

    return 0;
}

int main(int argc, char const *argv[]) {
    std::unique_ptr<Options> opts = parseOptions(argc, argv);
    if (opts->help) {
        printUsage();
        return -1;
    } else if (opts->version) {
        printVersion();
        return -1;
    }

    std::string buf;
    std::cin >> buf;

    if (!opts->reverse) {
        return hexize(buf, opts->pos, opts->num);
    } else {
        return unhexize(buf, opts->pos * 2, opts->num * 2);
    }
}
