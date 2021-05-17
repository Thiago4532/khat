#ifndef _FORK_HPP
#define _FORK_HPP

#include <cstdio>
#include <string>
#include <utility>
#include <sys/wait.h>

namespace nix {

class fork {
public:
    template <typename __Function>
    fork(__Function child) : _status(-1), _pid(c()) {
        if (_pid == 0) {
            child();
            exit(EXIT_SUCCESS);
        }
    }

    int pid() const { return _pid; }
    int wait() { 
        if (_status != -1)
            return _status;
        
        ::waitpid(_pid, &_status, 0);
        return _status;
    }

    static pid_t c();

private:
    pid_t _pid;
    int _status;
};

void execv(std::string const& path, std::initializer_list<std::string> args);
// pid_t wait();
// pid_t wait(int& status);

}

#endif