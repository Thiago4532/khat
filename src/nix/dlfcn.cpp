#include "nix/dlfcn.hpp"

#include <stdexcept>

namespace nix {

void* dlopen(std::string const& filename, int flags) {
    void* ret = ::dlopen(filename.c_str(), flags);

    if (ret == nullptr)
        throw std::runtime_error(std::string("dlopen: ") + dlerror());

    return ret;
}

void* dlsym(void* handle, std::string const& symbol) {
    void* ret = ::dlsym(handle, symbol.c_str());

    if (ret == nullptr)
        throw std::runtime_error(std::string("dlsym: ") + dlerror());

    return ret;
}

bool dlclose(void* handle) {
    return (::dlclose(handle) == 0);
}

}