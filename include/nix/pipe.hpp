#ifndef _PIPE_HPP
#define _PIPE_HPP

#include <cstdio>

namespace nix {

class pipe {
public:
    pipe();

    std::FILE* operator()(int fd);
    int operator[](int fd);
private:
    int _fd[2];
    std::FILE* _f[2];
};

void dup(int oldfd, int newfd);

};

#endif