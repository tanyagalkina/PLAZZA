#include "Parser.hpp"


ParserState::ParserState(std::string text) : _text(text){}

char ParserState::peek() const
{
    if (is_empty())
        throw ParseError("Parser reached end of string");
    return _text[_position];
}

void ParserState::pop()
{
    if (is_empty())
        throw ParseError("Parser reached end of string");
    _position++;
}

bool ParserState::is_empty() const
{
    return _position >= _text.length();
}

bool ParserState::peek_class(std::function<bool(char)> predicate) const
{
    if (is_empty())
        return false;
    return predicate(peek());
}

bool check_space(char c)
{
    return std::isspace(c);
}

bool check_letter(char c)
{
    return std::isalpha(c);
}

bool check_semicolon(char c)
{
    return (c == ';');
}

bool check_x(char c)
{
    return (c == 'x');
}

bool check_digit(char c)
{
    return (c >= '0' && c <= '9');
}

void ParserState::skip_space()
{
    while (peek_class(check_space)) {
        pop();
    }
}

void ParserState::consume(std::function<bool(char)> predicate)
{
    if (peek_class(predicate))
        pop();
    else
        throw ParseError("Unexpected character");
}

void ParserState::expect(std::function<bool(char)> predicate) const
{
    if (!peek_class(predicate))
        throw ParseError ("Unexpected character");
}

bool ParserState::optional(std::function<bool(char)> predicate)
{
    if(peek_class(predicate)) {
        pop();
        return true;
    }
    return false;
}

std::string parse_string(ParserState &state)
{
    state.skip_space();
    std::string result;
    state.expect(check_letter);
    while (state.peek_class(check_letter)) {
        result += state.peek();
        state.pop();
    }
    return result;
}

size_t parse_integer(ParserState &state)
{
    state.skip_space();
    size_t result = 0;
    state.expect(check_digit);
    while (state.peek_class(check_digit)) {
        result *= 10;
        result += state.peek() - '0';
        state.pop();
    }
    return result;
}

PizzaType parse_type(ParserState &state) //can be done better with std::map so it can be more generic
{
    std::string str = parse_string(state);
    if (str == "regina")
        return PizzaType::Regina;
    if (str == "margarita")
        return PizzaType::Margarita;
    if (str == "americana")
        return PizzaType::Americana;
    if (str == "fantasia")
        return PizzaType::Fantasia;
    throw ParseError("wrong Pizza Type");
}

PizzaSize parse_size(ParserState &state)
{
    std::string str = parse_string(state);
    if (str == "S")
        return PizzaSize::S;
    if (str == "M")
        return PizzaSize::M;
    if (str == "L")
        return PizzaSize::L;
    if (str == "XL")
        return PizzaSize::XL;
    if (str == "XXL")
        return PizzaSize::XXL;
    throw ParseError("Wrong Pizza Size");
}

size_t parse_number(ParserState &state)
{
    state.skip_space();
    state.consume(check_x);
    return parse_integer(state);
}

OrderPart parse_order_part(ParserState &state)
{
    auto type = parse_type(state);
    auto size = parse_size(state);
    auto number = parse_number(state);
    Pizza pizza = {type, size};
    OrderPart part = {pizza, number};
    return part;
}

Order parse_order(ParserState &state)
{
    Order result;
    do {
        auto part = parse_order_part(state);
        result._parts.push_back(part);
        state.skip_space();
    } while (state.optional(check_semicolon));
    if (!state.is_empty())
        throw ParseError ("Extra character after order");
    return result;
}

Order parse_order(std::string text)
{
    ParserState state(text);
    return parse_order(state);
}

size_t parse_integer(std::string text)
{
    ParserState state(text);
    return parse_integer(state);
}

/*
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
}*/
