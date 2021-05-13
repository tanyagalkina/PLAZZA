#ifndef RECEPTION_HPP_
#define RECEPTION_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

#include "Parser.hpp"
#include "Reception.hpp"
#include "Kitchen.hpp"
#include "Messenger.hpp"
#include "Pizza.hpp"

/*
enum PizzaType
{
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};

enum PizzaSize
{
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};

typedef struct {
    PizzaType _type;
    PizzaSize _size;
} order_t;
*/
    class Reception {
    public:
        Reception(float, int, int);

        std::vector<Order> parse(const std::string &str);

        void run(); /* MAIN LOOP */
        void getInput();

//private:
//    PizzaType strToPizzaType(std::string str);
//    PizzaSize strToPizzaSize(std::string str);

    private:
        float _multiply;
        int _cooks;
        int _refill;
        std::vector<Order> _order;
        int _nbKitchens;
        std::unique_ptr<Messenger> messenger;
    private:
        void addKitchen();
        void runWindow();

    };


#endif //RECEPTION_HPP_
