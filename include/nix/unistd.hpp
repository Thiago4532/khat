#ifndef _UNISTD_HPP
#define _UNISTD_HPP

#include "stdio.hpp"
#include <string>
#include <utility>

namespace nix {

typedef std::pair<int, int> pipefd;

class fdstream {
public:
    fdstream(int fd_read, int fd_write);

    int fd_read() const;
    int fd_write() const;

    std::FILE* fp_read();
    std::FILE* fp_write();

private:
    int _fd_read, _fd_write;
    std::FILE *_read, *_write;
};

pipefd pipe();
fdstream fpipe();
void dup2(int oldfd, int newfd);

pid_t fork();
void execv(std::string const& path, std::initializer_list<std::string> args);
pid_t wait();
pid_t wait(int& status);

// Header-only implementations

template <typename __Function>
inline pid_t fork(__Function child) {
    pid_t cpid = fork();
    if (cpid == 0) {
        child();
        exit(EXIT_SUCCESS);
    }

    return cpid;
}

}

#endif