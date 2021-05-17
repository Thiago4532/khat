#include "nix/tempfile.hpp"

#include "nix/stdlib.hpp"
#include <unistd.h>

namespace nix {

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