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
    //std::vector<KMetaData> _kitchen_mds;
    //std::vector<Pizza> _pizza_todo;
    std::vector<Order> _orders;
    int _nbKitchens;
    std::unique_ptr<Messenger> messenger;
private:
    void addKitchen();
    void runWindow();

};


#endif //RECEPTION_HPP_
