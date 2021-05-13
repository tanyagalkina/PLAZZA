#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Pizza.hpp"
#include <vector>
#include "Error.hpp"
#include <string>
#include <functional>

struct OrderPart
{
    Pizza _pizza;
    size_t _amount; //anderen to int
};

struct Order
{
    std::vector<OrderPart> _parts;
};

class ParserState
{
    std::string _text;
    size_t _position = 0;
public:
    explicit ParserState(std::string text);
    char peek() const;
    void pop();
    bool is_empty() const;
    bool peek_class(std::function<bool(char)> predicate) const;
    void skip_space();
    void consume(std::function<bool(char)> predicate);
    void expect(std::function<bool(char)> predicate) const;
    bool optional(std::function<bool(char)> predicate);
};


Order parse_order(std::string text);
size_t parse_integer(std::string text);


/*
#include <vector>
#include <sstream>
#include <memory>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Error.hpp"

class Parser
{
public:
    static std::vector<std::string> parseFromFd(int fd, size_t bytes, const char *delim = " ");
    static std::vector<std::string> parseFromFile(std::string path, const char *delim = " ");
    static std::vector<std::string> parseString(std::string s, const char *delim = " ");
    static int parseStringToInt(std::string s);

private:
    static std::vector<std::string> split(const std::string &s, const char *delim);
};*/

#endif //PARSER_HPP_