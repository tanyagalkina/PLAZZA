#include "Parser.hpp"
#include "Error.hpp"


std::vector<std::string> Parser::split(const std::string &s, const char *delim)
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

std::vector<std::string> Parser::parseFromFd(int fd, size_t bytes, const char *delim)
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

std::vector<std::string> Parser::parseFromFile(std::string path, const char *delim)
{
    if (strcmp(delim, "") == 0)
        throw ParseError("Delimiter is empty");
    if (path.empty())
        throw ParseError("string is empty");
    std::string line;
    std::ifstream file (path);
    std::vector<std::string> text;
    if (file.is_open())
        while (getline(file, line))
            if (!line.empty())
                text.push_back(line);
    file.close();
    return (text);
}

std::vector<std::string> Parser::parseString(std::string s, const char *delim)
{
    if (s.empty())
        throw ParseError("string is empty");
    if (strcmp(delim, "") == 0)
        throw ParseError("Delimiter is empty");
    return split(s, delim);
}

int Parser::parseStringToInt(std::string s)
{
    if (s.empty())
        throw ParseError("string is empty");
    s.erase(remove_if(s.begin(), s.end(), [](char c) { return !isdigit(c); } ), s.end());
    if (s.empty())
        throw ParseError("string has no digit");
    int r = std::stoi(s);
    return r;
}
