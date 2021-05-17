#include "nix/tempfile.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <cerrno>
#include <system_error>

namespace nix {

static inline
int mkstemp(std::string& temp) {
    int ret = ::mkstemp(temp.data());
    if (ret == -1)
        throw std::system_error(errno, std::generic_category(), "tempfile::mkstemp");

    return ret;
}

tempfile::tempfile(std::string const& t)
    : std::string(t)
    , _fd(mkstemp(*this)) { }

tempfile::tempfile(std::string&& t)
    : std::string(t)
    , _fd(mkstemp(*this)) { }

tempfile::~tempfile() {
    unlink(c_str());
}

int tempfile::fd() const {
    return _fd;
}

}