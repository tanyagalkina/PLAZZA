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

class WrongArugument : public Error
{
public:
    WrongArugument(const std::string &message)
        : Error(message)
    {}
};

#endif //ERROR_HPP_
