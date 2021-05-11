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

void errorhandling(int ac, char **av)
{
    if (ac == 2 && strcmp(av[1], "-h") == 0)
        usage();

    if (ac != 4)
        throw BadArgument("rerun with -h");

    std::string mult = std::to_string(std::stof(av[1]));
    mult.erase(std::remove(mult.begin(), mult.end(), '0'), mult.end());

    if (strlen(av[1]) != mult.size() && (std::stoi(av[1]) != std::stof(av[1])))
        throw BadArgument("rerun with -h");
    if (strlen(av[2]) != strlen(std::to_string(std::stoi(av[2])).c_str()))
        throw BadArgument("rerun with -h");
    if (strlen(av[3]) != strlen(std::to_string(std::stoi(av[3])).c_str()))
        throw BadArgument("rerun with -h");
}

// @todo: think about a clever logging system

// std::fstream Log()
//{
    //std::fstream file("logfile", std::fstream::out | std::fstream::app);
    //return file;
//}
//static std::fstream mlog("logfile", std::fstream::out | std::fstream::app);

void print(const std::string &msg)
{
    std::cout << msg << std::endl;
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
