#include "nix/stdio.hpp"
#include <stdio.h>

#include <cerrno>
#include <system_error>

namespace nix {

std::FILE* fdopen(int fd, std::string const& mode) {
    FILE* ret = ::fdopen(fd, mode.c_str());

    if (ret == nullptr)
        throw std::system_error(errno, std::generic_category(), "fdopen");

    return ret;
}

bool fclose(std::FILE* stream) {
    return (::fclose(stream) == 0);
}

}