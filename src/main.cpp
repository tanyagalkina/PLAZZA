/*
Reception -> Class



Kitchen -> Class
 - bool kitchenAvailable(Pizza) is it ready to make this pizza


KitchenState -> Enum
 - OK
 - KO
 - Pending
*/

/*      PutInKitchen 
        for (kitchen in kitchens)
            KitchenState = kitchenAvailable(current Pizza to check) 
                if true:
                    balance the pizzas to all the available kitchens
                if false:
                    create new kitchen

        return pid 
*/

// TODO figure out products size?
// child pool (namedpipe, shared memory ...)
// pthread or std::thread?
//
/*
    parsingArguments
    while (1):
        order = takeOrder() -> list of pizzas

        for (pizza in order)
            putInKitchen

*/

#include <exception>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "Error.hpp"
#include <sstream>
#include <algorithm>

void usage()
{
    std::cout   << "USAGE\n"
                << "\t[float] multiplier\n"
                << "\t[int] cooks per kitchen\n"
                << "\t[int] refill time" << std::endl;
}

void error_handling(float multi, int cooks, int refill)
{
    if (multi <= 0 || cooks <= 0 || refill <= 0)
        throw WrongArgument ("Must be positive");
}

enum PizzaType
{
    Regina = 1 ,
    Margarita = 2 ,
    Americana = 4 ,
    Fantasia = 8
};
enum PizzaSize
{
    S = 1 ,
    M = 2 ,
    L = 4 ,
    XL = 8 ,
    XXL = 16
};


typedef struct orders {
    PizzaType _type;
    PizzaSize _size;
}orders_t;
/*regina XXL x2; fantasia M x3; margarita S x1
regina XXL
regina XXL
fantasia M
*/


std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

PizzaType strtoPizzaType(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str.compare("regina") == 0)
        return Regina;
    if (str.compare("margarita") == 0)
        return Margarita;
    if (str.compare("americana") == 0)
        return Americana;
    if (str.compare("fantasia") == 0)
        return Fantasia;
    throw ParseError ("wrong pizza type");
}
PizzaSize strtoPizzaSize(std::string str)
{
    if (str.compare("S"))
        return S;
    if (str.compare("M"))
        return M;
    if (str.compare("L"))
        return L;
    if (str.compare("XL"))
        return XL;
    if (str.compare("XXL"))
        return XXL;
    throw ParseError("wrong PizzaSize");
}

std::vector<orders_t> Parse(std::string str)
{
    std::vector<orders_t> order;
    std::vector<std::string> lines = split(str, ";");
    for (const std::string &line : lines) {
        std::vector<std::string> parts = split(line, " ");
        if (parts.size() != 3)
            throw ParseError ("bad input");
        if (parts[2][0] != 'x')
            throw ParseError ("bad input");
        int number = std::stoi(parts[2], 1);
        if (number >= 100 || number <= 0)
            throw ParseError ("bad number");
        while (number != 0) {
            order->push_back(strtoPizzaType(parts[0]) , strtoPizzaSize(parts[1]));
            //strtoPizzaSize(parts[1]);
            number--;
        }
    }
    return order;
        //for (int i = 0; parts[3][0]; i++)

}

int main(int ac, char **av)
{
    if (ac == 2 && strcmp(av[1], "-h") == 0) {
        usage();
        return 0;
    }
    if (ac != 4)
        return 84;

    try {
        float multiplicator = std::stof(av[1]);
        int cooks = std::stoi(av[2]);
        int refill = std::stoi(av[3]);
        error_handling(multiplicator, cooks, refill);
        //TODO start
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    std::vector<orders_t> orders;
    orders = Parse("regina XXL x2; fantasia M x3; margarita S x1");

    return 0;
}
