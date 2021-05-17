#ifndef _DYNAMIC_LOADER_HPP
#define _DYNAMIC_LOADER_HPP

#include "nix/dlfcn.hpp"
#include <string>

namespace nix {

class dynamic_loader {
public:
    dynamic_loader(std::string const& filename);
    dynamic_loader(std::string const& filename, int flags);
    ~dynamic_loader();

    void* lookup(std::string const& symbol);
    void* operator()(std::string const& symbol);

private:
    void* _dll;
};

}

#endif