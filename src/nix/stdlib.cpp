#include "nix/stdlib.hpp"
#include <stdlib.h>

#include <cerrno>
#include <system_error>

namespace nix {

int mkstemp(std::string& temp) {
    int ret = ::mkstemp(temp.data());
    if (ret == -1)
        throw std::system_error(errno, std::generic_category(), "mkstemp");

    return ret;
}

}