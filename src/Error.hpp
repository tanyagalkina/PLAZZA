#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <exception>
#include <iostream>

class Error : public std::exception
{
    public:
        Error(std::string const &message) : _message(message) {}

        const char *what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message;
};

class WrongArgument : public Error
{
public:
    WrongArgument(const std::string &message)
        : Error(message)
    {}
};

class ParseError : public Error
{
public:
    ParseError(const std::string &message)
        : Error(message)
    {}
};

class BadArgument : public Error
{
public:
    BadArgument(const std::string &message)
            : Error(message)
    {}
};

class TooManyThread : public Error
{
public:
    TooManyThread(const std::string &message)
            : Error(message)
    {}
};

#endif //ERROR_HPP_
