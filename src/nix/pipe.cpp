#include "nix/pipe.hpp"

#include <unistd.h>
#include <cerrno>
#include <system_error>

namespace nix {

static inline
std::FILE* fdopen(int fd, std::string const& mode) {
    FILE* ret = ::fdopen(fd, mode.c_str());

    if (ret == nullptr)
        throw std::system_error(errno, std::generic_category(), "fdopen");

    return ret;
}

pipe::pipe() {
    if (::pipe(_fd) == -1)
        throw std::system_error(errno, std::generic_category(), "pipe");

    _f[0] = fdopen(_fd[0], "r");
    _f[1] = fdopen(_fd[1], "w");
}

std::FILE* pipe::operator()(int fd) { return _f[fd]; }
int pipe::operator[](int fd) { return _fd[fd]; }

void dup(int oldfd, int newfd) {
    if (::dup2(oldfd, newfd) == -1)
        throw std::system_error(errno, std::generic_category(), "dup2");
}

}