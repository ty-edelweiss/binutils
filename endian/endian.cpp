#include "endian.hpp"

#include <cstring>
#include <iostream>
#include <memory>

struct Options {
    /* non-functional options */
    bool help = false;
    bool version = false;
};

void printUsage() {
    std::cout << "Usage: endian [options]" << std::endl;
    std::cout << "  " << std::endl;
    std::cout << "  Print endianness on the current system " << std::endl;
    std::cout << "  " << std::endl;
    std::cout << "Allowed options:" << std::endl;
    std::cout << "  -h --help        print help messages" << std::endl;
    std::cout << "  -v --version     print program version" << std::endl;
}

void printVersion() {
    std::cout << ENDIAN_VERSION << std::endl;
}

std::unique_ptr<Options> parseOptions(int argc, char const* argv[]) {
    std::unique_ptr<Options> opts = std::make_unique<Options>();
    for (int i = 0; i < argc; i++) {
        if (!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "--help")) {
            opts->help = true;
        } else if (!std::strcmp(argv[i], "-v") ||
                   !std::strcmp(argv[i], "--version")) {
            opts->version = true;
        } else {
            // skip
        }
    }
    return opts;
}

int main(int argc, char const* argv[]) {
    std::unique_ptr<Options> opts = parseOptions(argc, argv);
    if (opts->help) {
        printUsage();
        return -1;
    } else if (opts->version) {
        printVersion();
        return -1;
    }

    int n = 1;

    unsigned char* p = reinterpret_cast<unsigned char*>(&n);

    if (*p) {
        std::cout << "Little-endian" << std::endl;
    } else {
        std::cout << "Big-endian" << std::endl;
    }

    return 0;
}
