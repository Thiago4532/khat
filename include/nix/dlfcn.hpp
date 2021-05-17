#ifndef _DLFCN_HPP
#define _DLFCN_HPP
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

void* dlopen(std::string const& filename, int flags);
void* dlsym(void* handle, std::string const& symbol);
bool dlclose(void* handle);

}

#endif