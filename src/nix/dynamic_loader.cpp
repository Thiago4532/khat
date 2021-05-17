#include "nix/dynamic_loader.hpp"

namespace nix {

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