#include "Parser.hpp"
#include "Error.hpp"


static std::vector<std::string> split(const std::string &s, const char *delim)
{
    std::stringstream stringStream(s);
    std::string line;
    std::vector<std::string> words;

    while(std::getline(stringStream, line)) {
        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(delim, prev)) != std::string::npos) {
            if (pos > prev)
                words.push_back(line.substr(prev, pos-prev));
            prev = pos+1;
        }
        if (prev < line.length())
            words.push_back(line.substr(prev, std::string::npos));
    }
    return words;
}

static std::vector<std::string> parseFromFd(int fd, size_t bytes, const char *delim)
{
    if (strcmp(delim, "") == 0)
        throw ParseError("Delimiter is empty");

    char *buffer = (char *)malloc(sizeof(char) * bytes);
    if (!buffer)
        throw ParseError("malloc failed");

    int r = read(fd, buffer, bytes);
    if (r == -1)
        throw ParseError("read failed");

    auto ret = split(buffer, delim);

    free(buffer);
    return ret;
}

static std::vector<std::string> parseString(std::string s, const char *delim)
{
    if (s.empty())
        throw ParseError("string is empty");
    if (strcmp(delim, "") == 0)
        throw ParseError("Delimiter is empty");
    return split(s, delim);
}
