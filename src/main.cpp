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

void usage()
{
    std::cout <<    "USAGE\n" << 
                    "\t[float] multiplier\n" << 
                    "\t[int] cooks per kitchen\n" << 
                    "\t[int] refill time" << std::endl;
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
        //TODO start
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
