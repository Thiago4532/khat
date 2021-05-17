#ifndef _FORK_HPP
#define _FORK_HPP

#include <cstdio>
#include <string>
#include <utility>

namespace nix {

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