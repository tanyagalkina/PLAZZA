#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <vector>
#include <sstream>
#include <memory>
#include <unistd.h>
#include <string.h>

#include "Error.hpp"

class Parser
{
public:
    static std::vector<std::string> parseFromFd(int fd, size_t bytes, const char *delim = " ");
    static std::vector<std::string> parseString(std::string s, const char *delim = " ");

private:
    static std::vector<std::string> split(const std::string &s, const char *delim);
};


#endif //PARSER_HPP_
