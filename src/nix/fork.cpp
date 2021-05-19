#include "nix/fork.hpp"

#include <cerrno>
#include <cstring>
#include <sys/wait.h>
#include <system_error>
#include <unistd.h>

namespace nix {

int fork::wait() {
    if (_status != -1)
        return _status;

    ::waitpid(_pid, &_status, 0);
    return _status;
}

pid_t fork::c() {
    pid_t ret = ::fork();
    if (ret == -1)
        throw std::system_error(errno, std::generic_category(), "fork");
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

// pid_t wait() {
//     pid_t ret = ::wait(NULL);

//     if (ret == -1)
//         throw std::system_error(errno, std::generic_category(), "wait");
//     return ret;
// }

// pid_t wait(int& status) {
//     pid_t ret = ::wait(&status);

//     if (ret == -1)
//         throw std::system_error(errno, std::generic_category(), "wait");
//     return ret;
// }

}