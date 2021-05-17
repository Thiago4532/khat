#include "nix/unistd.hpp"
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <sys/wait.h>
#include <system_error>

namespace nix {

fdstream::fdstream(int fd_read, int fd_write)
    : _fd_read(fd_read)
    , _fd_write(fd_write)
    , _read(fdopen(fd_read, "r"))
    , _write(fdopen(fd_write, "w")) { }

int fdstream::fd_read() const { return _fd_read; }
int fdstream::fd_write() const { return _fd_write; }

std::FILE* fdstream::fp_read() { return _read; };
std::FILE* fdstream::fp_write() { return _write; };

inline pipefd pipe() {
    int fd[2];
    if (::pipe(fd) == -1)
        throw std::system_error(errno, std::generic_category(), "pipe");

    return { fd[0], fd[1] };
}

fdstream fpipe() {
    int fd[2];
    if (::pipe(fd) == -1)
        throw std::system_error(errno, std::generic_category(), "fpipe");

    return fdstream(fd[0], fd[1]);
}

void dup2(int oldfd, int newfd) {
    if (::dup2(oldfd, newfd) == -1)
        throw std::system_error(errno, std::generic_category(), "dup2");
}

pid_t fork() {
    pid_t ret = ::fork();
    if (ret == -1)
        throw std::system_error(errno, std::generic_category(), "dup2");
    return ret;
}

void execv(std::string const& path, std::initializer_list<std::string> args) {
    char** argv = new char*[args.size() + 1];

    int i = 0;
    for (auto it = args.begin(); it != args.end(); ++it)
        argv[i++] = const_cast<char*>(it->c_str());
    argv[i] = nullptr;

    ::execv(path.c_str(), argv);
}

pid_t wait() {
    pid_t ret = ::wait(NULL);

    if (ret == -1)
        throw std::system_error(errno, std::generic_category(), "wait");
    return ret;
}

pid_t wait(int& status) {
    pid_t ret = ::wait(&status);

    if (ret == -1)
        throw std::system_error(errno, std::generic_category(), "wait");
    return ret;
}

}