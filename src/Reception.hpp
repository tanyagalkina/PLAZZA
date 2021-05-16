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

struct KMetaData
{
    int busy_cooks;
    int orders_max;
    int _ownId;
    int currOrders;
    mqd_t orderQueue;
    mqd_t deliveryQueue;

};

class Reception {
public:
    Reception(float, int, int);

    std::vector<Order> parse(const std::string &str);

    void run(); /* MAIN LOOP */
    void getInput(int &order_nb);

    std::mutex MDMutex;
    std::mutex OrderMutex;
    std::mutex MessageMutex;

private:
    float _multiply;
    int _cooks;
    int _refill;
    std::vector<KMetaData> _kitchen_mds;
    std::vector<PizzaQueue> _pizza_to_do;
    std::vector<Order> _orders;
    int _nbKitchens;
    int uniqueKitchenId;
    std::unique_ptr<Messenger> messenger;
private:
    void addKitchen();
    void runWindow();
    int getAvailableKitchen();
    void removeFromMetaData(int);
    void updateOrders(std::string);
    void parse_this_buffer(std::string buffer, int meta_own_id);

};

//<ORDER> orderNumber, pizza_done int, pizza total int,


#endif //RECEPTION_HPP_
