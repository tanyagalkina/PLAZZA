
#include "Reception.hpp"
#include "Error.hpp"

/*
std::ostream &operator<<(std::ostream &out, order_t &order)
{
    out << "Size: " << order._size << " Type: " << order._type << std::endl;
    return out;
}*/

int cooking_time(int pizza_type)
{
    if (pizza_type == 1)
        return 1;
    else if (pizza_type == 8)
        return 4;
    else
        return 2;
}


void Reception::getInput(int &order_nb)
{
    order_nb++;
    //_orders.clear();
    std::string input;
    std::getline(std::cin, input);
    /*if (input == "status") {
        if (_kitchen_mds.size() != 0) {
            std::cout << _kitchen_mds.size() << std::endl;
            for (auto kitchen: _kitchen_mds) {
                std::cout << "Kitchen nbr: " << kitchen._ownId << " and it has: " << kitchen.currOrders
                          << " pizza cooking\n";
                this->messenger->send_order_to_the_kitchen(kitchen._ownId, "status");
            }
            return;
        }
        std::cout << "Currently no kitchen is available, please order something" << std::endl;
        return;
    }*/
    Order order;
    order = parse_order(input);
    order.input = input;
    order.nb_of_pizzas = 0;
    PizzaQueue pizza_to_send;
    for (auto order_inside : order._parts) {
        int i = order_inside._amount;
        order.nb_of_pizzas += i;
        while (i != 0) {
            pizza_to_send._cooking_time = cooking_time((int) order_inside._pizza._type);
            pizza_to_send._order_nb = order_nb;
            pizza_to_send._pizza_to_cook = std::to_string(pizza_to_send._order_nb) + " " + std::to_string(pizza_to_send._cooking_time) + " " + std::to_string((int) order_inside._pizza._type);
            _pizza_to_do.push_back(pizza_to_send);
            i--;
        }
        std::cout << "Type : " << (int) order_inside._pizza._type << " Size : " <<
                    (int) order_inside._pizza._size << " amount: " << order_inside._amount << std::endl;
    }
    order.order_nb = order_nb;
    order.pizza_finished = 0;
    _orders.push_back(order);

    for (auto pizzas : _pizza_to_do) {
        std::cout << pizzas._pizza_to_cook << std::endl;
    }
}

Reception::Reception(float mulitpy, int cooks, int refill) : _multiply(mulitpy), _cooks(cooks), _refill(refill)
{
    std::unique_ptr<Messenger> messenger(new Messenger());
    this->messenger = std::move(messenger);
    this->_nbKitchens = 0;
    this->uniqueKitchenId = 1;
}

void Reception::runWindow() {
    int i = 0;
    int kitchenId = 0;

    while (1) {
        try {
            getInput(i);
            for (auto order: _orders)
                std::cout << "nb of pizza in this order : " << order.order_nb << std::endl;
        } catch (const ParseError &e) {
            std::cerr << "Bad input: " << e.what() << std::endl;
            continue;

        }

        while (_pizza_to_do.size() != 0) {
            std::string currOrder = this->_pizza_to_do.front()._pizza_to_cook;
            _pizza_to_do.erase(_pizza_to_do.begin());

            kitchenId = getAvailableKitchen();
           // std::cout << "kitchenID: " << kitchenId << std::endl;
            std::cout << "the order lautet:" << currOrder << std::endl;

                if (kitchenId == 0) {
                    //std::cout << "there"
                    MDMutex.lock();
                    MessageMutex.lock();
                    addKitchen();
                    MDMutex.unlock();
                    MessageMutex.unlock();
                  //  std::cout << "the uniqueId is " << uniqueKitchenId << std::endl;
                    this->messenger->send_order_to_the_kitchen(uniqueKitchenId, currOrder);
                    MDMutex.lock();
                    updateKitchenBusy(uniqueKitchenId);
                    MDMutex.unlock();
                } else
                    this->messenger->send_order_to_the_kitchen(kitchenId, currOrder);
                MDMutex.lock();
                updateKitchenBusy(kitchenId);
                MDMutex.unlock();
                currOrder = "";
            }
        }
}

void Reception::updateKitchenBusy(int ownId)
{
    for (int i = 0; i < _kitchen_mds.size(); ++i)
    {
        if (ownId == _kitchen_mds[i]._ownId)
            _kitchen_mds[i].currOrders++;
    }
}

///returns 0 if all the kitchens are busy_max;
int Reception::getAvailableKitchen()
{
    if (_kitchen_mds.size() == 0) {
        return 0;
        std::cout << "there is no kitchen currently open\n";
    }
    //return _kitchen_mds[0]._ownId;

    //for (auto entry : _kitchen_mds)
       // std::cout << "available kitchens: "<< entry._ownId << std::endl;

    //for (auto entry : _kitchen_mds)
     //   std::cout << "BEFORE: "<< entry.currOrders << std::endl;

    std::sort(_kitchen_mds.begin(), _kitchen_mds.end(), [](const auto &a, const auto &b) { return a.currOrders < b.currOrders; });

    //_kitchen_mds.sort()
    //for (auto entry : _kitchen_mds)
    //    std::cout << "AFTER: "<< entry.currOrders << std::endl;


    //printf("cur = %d -- max = %d\n",_kitchen_mds[0].currOrders, _kitchen_mds[0].orders_max);
    if (_kitchen_mds[0].currOrders == _kitchen_mds[0].orders_max)
        return 0;
    return _kitchen_mds[0]._ownId;
}

void Reception::parse_this_buffer(std::string buffer, int meta_own_id)
{
    int value = parse_integer(buffer);
    if (value == 0) {
        //remove from metadata shit;
        for (int i = 0; i < _kitchen_mds.size(); i++) {
            if (_kitchen_mds[i]._ownId == meta_own_id) {
                std::cout << "Kitchen nb " << _kitchen_mds[i]._ownId << " closed" << std::endl;
                _kitchen_mds.erase(_kitchen_mds.begin() + i);
            }
        }
    }
    else {
        for (int i = 0; i < _kitchen_mds.size(); i++) {
            if (_kitchen_mds[i]._ownId == meta_own_id)
                _kitchen_mds[i].currOrders--;
        }
        for (int i = 0; i < _orders.size(); i++) {
            if (value == _orders[i].order_nb) {
                _orders[i].pizza_finished++;
                if (_orders[i].pizza_finished == _orders[i].nb_of_pizzas) {
                    std::cout << "This order is done: " << _orders[i].input << std::endl;
                    _orders.erase(_orders.begin() + i);
                    // remove this order; << to do
                    //std::remove_if(_orders.begin(), _orders.end(), order);
                }
            }
        }
    }
}
void Reception::run() {
    std::string input;
    std::string buffer;
    std::vector<Order> orders;
    //this->addKitchen();
    std::thread window(&Reception::runWindow, this);
    //std::thread window(do_loop);
    //andree thread finisch();
    int order_nb = 0;
    struct mq_attr attr;
    while (true) {
        if (_kitchen_mds.size() == 0)
            continue;
        for (auto meta: _kitchen_mds) {
            mq_getattr(meta.deliveryQueue, &attr);
            if (attr.mq_curmsgs != 0) {
                this->messenger->rcv_kitchen_reply(meta._ownId, buffer);
                if (buffer != "") {
                    std::cout << " this is what the kitchen said: " << buffer << std::endl;
                    parse_this_buffer(buffer, meta._ownId);
                    //if the message is "orderNumber" -> update_orders

                    //if the nessage is "Goodbye!" -> remove from MetaData
                    //
                    buffer = "";
                }
            }
        }
        //try {
            //getInput(order_nb);
        //} catch (const ParseError &e) {
            //std::cerr << e.what() << std::endl;
            //exit (84);
        //}
        /*this->messenger->send_order_to_the_kitchen(1, "FirstPizza");
        this->messenger->send_order_to_the_kitchen(1, "SecondPizza");
        this->messenger->send_order_to_the_kitchen(1, "ThirdPizza");*/

        //sleep(3);
        //for (alle queues);
        //this->messenger->rcv_kitchen_reply(1, buffer);
        //if (buffer != "") {
        //    std::cout << " this is what the kitchen said: " << buffer << std::endl;
        //    buffer = "";
        }
        //if (buffer == "Goodbye!")

            //removeFromMd()
    //window.join();
    //}

    std::cout << "running" << std::endl;
}

void Reception::addKitchen() {

    this->uniqueKitchenId++;
    pid_t pid;

    this->_nbKitchens++;
    this->messenger->create_new_pair(this->uniqueKitchenId);
    pid = fork();
    if (pid == 0) {
        Kitchen kitchen(this->_cooks, this->uniqueKitchenId, this->_refill, this->_multiply);
    }

    KMetaData metaData;
    metaData._ownId = uniqueKitchenId;
    metaData.busy_cooks = 0;
    metaData.currOrders = 0;
    metaData.orders_max = this->_cooks * 2;

    _kitchen_mds.push_back(metaData);
}
