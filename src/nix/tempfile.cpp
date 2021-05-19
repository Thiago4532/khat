#include "nix/tempfile.hpp"

#include <cerrno>
#include <stdlib.h>
#include <system_error>
#include <unistd.h>

#include <fmt/core.h>
namespace nix {

static inline int mkstemp(std::string& temp, int suffix = 0) {
    int ret = ::mkstemps(temp.data(), suffix);
    if (ret == -1)
        throw std::system_error(errno, std::generic_category(), "tempfile::mkstemp");

    return ret;
}

tempfile::tempfile(std::string const& t)
    : std::string(t) {

    int _template = t.find("XXXXXX");
    if (_template == std::string::npos)
        throw std::runtime_error("tempfile: missing template!");

    int suffix = t.size() - _template - 6;

    _fd = mkstemp(*this, suffix);
}

tempfile::tempfile(std::string&& t)
    : std::string(t) {

    int _template = t.find("XXXXXX");
    if (_template == std::string::npos)
        throw std::runtime_error("tempfile: missing template!");

    int suffix = t.size() - _template - 6;

    _fd = mkstemp(*this, suffix);
}

tempfile::~tempfile() {
    unlink(c_str());
}

int tempfile::fd() const {
    return _fd;
}

}