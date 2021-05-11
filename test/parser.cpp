#include <criterion/criterion.h>
#include "../src/Parser.hpp"

Test(string, parse_string_with_letters)
{
    int i = Parser::parseStringToInt("123asdf");

    cr_assert(i == 123);
}

Test(string, parse_string_with_symbols)
{
    int i = Parser::parseStringToInt("§$%123asdf");

    cr_assert(i == 123);
}

Test(string, parse_string_with_symbols_without_numbers)
{
    bool ok = false;
    try {
        int i = Parser::parseStringToInt("§$%asdf");
    } catch (ParseError e) {
        ok = true;
    }

    cr_assert(ok);
}

Test(string, parse_string_empty)
{
    bool ok = false;
    try {
        int i = Parser::parseStringToInt("");
    } catch (ParseError e) {
        ok = true;
    }

    cr_assert(ok);
}

Test(string, parse_string_one_delimiter)
{
    std::vector<std::string> res;
    res.push_back("hallo");
    res.push_back("123");

    auto ret = Parser::parseString("hallo 123", " ");
    bool ok = true;

    for (int i = 0; i < ret.size(); i++) {
        if (ret[i] != res[i])
            ok = false;
    }
    cr_assert(ok);
}

Test(string, parse_string_with_no_given_delimiter)
{
    std::vector<std::string> res;
    res.push_back("hallo");
    res.push_back("123");

    auto ret = Parser::parseString("hallo 123");
    bool ok = true;

    for (int i = 0; i < ret.size(); i++) {
        if (ret[i] != res[i])
            ok = false;
    }
    cr_assert(ok);
}

Test(string, parse_string_with_multiple_delimiters)
{
    std::vector<std::string> res;
    res.push_back("hallo");
    res.push_back("123");
    res.push_back("234");

    auto ret = Parser::parseString("hallo 123:234", " :");
    bool ok = true;

    for (int i = 0; i < ret.size(); i++) {
        if (ret[i] != res[i])
            ok = false;
    }
    cr_assert(ok);
}

Test(string, parse_string_with_empty_delimiter)
{
    std::vector<std::string> res;
    res.push_back("hallo");
    res.push_back("123");
    res.push_back("234");

    bool ok = false;

    try {
        auto ret = Parser::parseString("hallo 123:234", "");
    } catch (ParseError e) {
        ok = true;
    }

    cr_assert(ok);
}

Test(string, parse_string_with_nonpresent_delimiter)
{
    std::vector<std::string> res;
    res.push_back("hallo 123:234");

    bool ok = true;

    auto ret = Parser::parseString("hallo 123:234", "-");

    for (int i = 0; i < ret.size(); i++) {
        if (ret[i] != res[i])
            ok = false;
    }
    cr_assert(ok);
}








