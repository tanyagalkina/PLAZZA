#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include "Reception.hpp"
#include <iostream>

class Ingredients {
public:
    int doe;
    int tomato;
    int gruyere;
    int ham;
    int mushrooms;
    int steak;
    int eggplant;
    int goat_cheese;

    /* Easy subtraction inside the kitchen if an order with pizza comes in so
     * you can just use kitchen.ingredients -= pizza.ingredients */
    Ingredients &operator-=(const Ingredients &other)
    {
        this->doe -= other.doe;
        this->tomato -= other.tomato;
        this->gruyere -= other.gruyere;
        this->ham -= other.ham;
        this->mushrooms -= other.mushrooms;
        this->steak -= other.steak;
        this->eggplant -= other.eggplant;
        this->goat_cheese -= other.goat_cheese;

        return *this;
    }

    /* just increments all ingredients by 1. Useful for the 'add one of each
     * every N seconds thing' */
    void increment()
    {
        this->doe += 1;
        this->tomato += 1;
        this->gruyere += 1;
        this->ham += 1;
        this->mushrooms += 1;
        this->steak += 1;
        this->eggplant += 1;
        this->goat_cheese += 1;
    }
};

typedef struct pizza
{
    PizzaType type;
    PizzaSize size;
    std::string name;
    int time;
    Ingredients ing;

} pizza_t;
/*
 *  1. order r-k
 *  2. can we take it k-r
 *  3. print status r-k
 *  4. im closing k-r
 * */

enum MsgType
{
    ORDER,
    APPROVAL,
    PRINT_STATUS,
    CLOSING
};

class Message
{
public:
    Message(MsgType, pizza_t);  /* ORDER, PRINT_STATUS */
    Message(MsgType, bool);     /* APPROVAL */
    Message(MsgType);           /* CLOSING */
public:
    MsgType _type;
    pizza_t _pizza;
    bool _hasApproval;
};

#endif //MESSAGE_HPP_
