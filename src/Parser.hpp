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
void parse_pizza(std::string text, int &order, int &timer);

#endif //PARSER_HPP_