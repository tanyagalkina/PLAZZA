#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>
#include <ctype.h>

#include "Error.hpp"
#include "Reception.hpp"
#include "ThreadPool.hpp"
#include "Message.hpp"



void usage()
{
    std::cout   << "USAGE\n"
                << "\t[float] multiplier\n"
                << "\t[int] cooks per kitchen\n"
                << "\t[int] refill time" << std::endl;
    exit(0);
}

void print(const std::string &msg)
{
    std::cout << msg << std::endl;
}

int errorhandling(std::string s)
{
    int dot_counter = 0;
    for (char c : s) {
        if (!isdigit(c) && c != '.')
            return 1;
        if (c == '.')
            dot_counter++;
        if (dot_counter > 1)
            return 1;
    }
    return 0;
}

int main(int ac, char **av)
{
    std::cout << "Main thread: " << std::this_thread::get_id() << std::endl;
    if (ac == 2 && strcmp(av[1], "-h") == 0)
        usage();
    if (ac != 4 || errorhandling(av[1]) != 0) {
        std::cerr << "Bad argument. Rerun with -h" << std::endl;
        return 84;
    }

    float multiplicator = 0.0;
    int cooks = 0;
    int refill = 0;
    try {
        multiplicator = std::atof(av[1]);
        cooks = parse_integer(av[2]);
        refill = parse_integer(av[3]);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    if (multiplicator <= 0 || cooks <= 0 || refill <= 0) {
        std::cerr << "Arguments must be positiv" << std::endl;
        return 84;
    }

    Reception reception(multiplicator, cooks, refill);
    reception.run();

    return 0;
}
