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
#include <ostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Error.hpp"
#include "Reception.hpp"


void usage()
{
    std::cout   << "USAGE\n"
                << "\t[float] multiplier\n"
                << "\t[int] cooks per kitchen\n"
                << "\t[int] refill time" << std::endl;
    exit(0);
}

void errorhandling(int ac, char **av)
{
    if (ac == 2 && strcmp(av[1], "-h") == 0)
        usage();

    if (ac != 4)
        throw BadArgument("rerun with -h");

    //TODO maybe fix this later
    //if (strlen(av[1]) != strlen(std::to_string(std::stof(av[1])).c_str()))
        //throw BadArgument("rerun with -dh");
    if (strlen(av[2]) != strlen(std::to_string(std::stoi(av[2])).c_str()))
        throw BadArgument("rerun with -h");
    if (strlen(av[3]) != strlen(std::to_string(std::stoi(av[3])).c_str()))
        throw BadArgument("rerun with -h");
}

int main(int ac, char **av)
{
    try {
        errorhandling(ac, av);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    float multiplicator = std::stof(av[1]);
    int cooks = std::stoi(av[2]);
    int refill = std::stoi(av[3]);

    if (multiplicator <= 0 || cooks <= 0 || refill <= 0) {
        std::cerr << "Arguments must be positiv" << std::endl;
        return 84;
    }

    Reception reception(multiplicator, cooks, refill);
    reception.run();

    return 0;
}
