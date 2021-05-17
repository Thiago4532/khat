#ifndef __STDIO_HPP
#define __STDIO_HPP

#include <string>

namespace nix {

std::FILE* fdopen(int fd, std::string const& mode);
bool fclose(std::FILE* stream);

}

#endif