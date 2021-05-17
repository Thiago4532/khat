#ifndef _DYNAMIC_LOADER_HPP
#define _DYNAMIC_LOADER_HPP

#include <dlfcn.h>
#include <string>

namespace nix {

struct rtld {
    static constexpr int lazy = RTLD_LAZY;
    static constexpr int now = RTLD_NOW;
    static constexpr int binding_mask = RTLD_BINDING_MASK;
    static constexpr int noload = RTLD_NOLOAD;
    static constexpr int deepbind = RTLD_DEEPBIND;
    static constexpr int global = RTLD_GLOBAL;
    static constexpr int local = RTLD_LOCAL;
    static constexpr int nodelete = RTLD_NODELETE;
};

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