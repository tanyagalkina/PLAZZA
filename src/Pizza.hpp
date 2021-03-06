#ifndef PIZZA_HPP_
#define PIZZA_HPP_

#include <string>

enum class PizzaType
{
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};

enum class PizzaSize
{
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};

struct Pizza
{
    PizzaType _type;
    PizzaSize _size;
};

struct PizzaQueue
{
    int _order_nb;
    int _cooking_time;
    std::string _pizza_to_cook;
};

#endif //PIZZA_HPP_
