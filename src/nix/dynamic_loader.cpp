#include "nix/dynamic_loader.hpp"

#include <cerrno>
#include <system_error>
#include <string>

namespace nix {

static inline
void* dlopen(std::string const& filename, int flags) {
    void* ret = ::dlopen(filename.c_str(), flags);

    if (ret == nullptr)
        throw std::runtime_error(std::string("dynamic_loader::open: ") + dlerror());

    return ret;
}

static inline
void* dlsym(void* handle, std::string const& symbol) {
    void* ret = ::dlsym(handle, symbol.c_str());

    if (ret == nullptr)
        throw std::runtime_error(std::string("dynamic_loader::symbol: ") + dlerror());

    return ret;
}

static inline
bool dlclose(void* handle) {
    return (::dlclose(handle) == 0);
}

dynamic_loader::dynamic_loader(std::string const& filename)
    : _dll(nix::dlopen(filename, rtld::lazy)) { }

dynamic_loader::dynamic_loader(std::string const& filename, int flags)
    : _dll(nix::dlopen(filename, flags)) { }

dynamic_loader::~dynamic_loader() {
    nix::dlclose(_dll);
}

void* dynamic_loader::lookup(std::string const& symbol) {
    return nix::dlsym(_dll, symbol);
}

void* dynamic_loader::operator()(std::string const& symbol) {
    return nix::dlsym(_dll, symbol);
}

}