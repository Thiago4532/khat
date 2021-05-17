#include "nix/fork.hpp"

#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/wait.h>
#include <system_error>

namespace nix {

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