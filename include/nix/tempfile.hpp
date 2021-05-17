#ifndef _TEMPFILE_HPP
#define _TEMPFILE_HPP

#include <string>

namespace nix {

class tempfile : public std::string {
public:
    tempfile(std::string const& t);
    tempfile(std::string&& t);
    ~tempfile();

    int fd() const;

private:
    int _fd;
};

}

#endif