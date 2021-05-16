
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
    Order order;
    order = parse_order(input);
    _orders.push_back(order);
    //_pizza_to_do.push_back();
    PizzaQueue pizza_to_send;

    for (auto order_inside : order._parts) {
        int i = order_inside._amount;
        //order_inside._pizza._cooking_time = order_inside._pizza._type / 2;
        while (i != 0) {
            pizza_to_send._cooking_time = cooking_time((int) order_inside._pizza._type);
            pizza_to_send._order_nb = order_nb;
            pizza_to_send._pizza_to_cook = std::to_string(pizza_to_send._order_nb) + " " + std::to_string(pizza_to_send._cooking_time);
            _pizza_to_do.push_back(pizza_to_send);
            i--;
        }
        std::cout << "Type : " << (int) order_inside._pizza._type << " Size : " <<
                    (int) order_inside._pizza._size << " amount: " << order_inside._amount << std::endl;
    }
    for (auto pizzas : _pizza_to_do) {
        std::cout << pizzas._pizza_to_cook << std::endl;
    }

    /*auto lines = Parser::parseString(input, ";");

    for (const auto &line : lines) {
        auto parts = Parser::parseString(line);
        //@todo check for STATUS as a vaild input
        if (parts.size() != 3 || parts[2][0] != 'x')
            throw ParseError("bad input");
        int howMany = Parser::parseStringToInt(parts[2]);
        if (howMany > 99 || howMany < 1)
            throw ParseError("bad number");
        while (howMany--) {
            order_t order = { strToPizzaType(parts[0]), strToPizzaSize(parts[1]) };
            _order.push_back(order);
        }
    }*/
}

Reception::Reception(float mulitpy, int cooks, int refill) : _multiply(mulitpy), _cooks(cooks), _refill(refill)
{
    std::unique_ptr<Messenger> messenger(new Messenger());
    this->messenger = std::move(messenger);
    this->_nbKitchens = 0;
    this->uniqueKitchenId = 1;
}

void Reception::runWindow()
{
    int i = 0;
    int kitchenId = 0;

    while (1) {
        try {
            getInput(i);
        } catch (const ParseError &e) {
            std::cerr << "Bad input: " << e.what() << std::endl;
            continue;
        }
        std::string currOrder = this->_pizza_to_do.front()._pizza_to_cook;
        std::cout << "the order lautet:" << currOrder << std::endl;
        _pizza_to_do.erase(_pizza_to_do.begin());

            kitchenId = getAvailableKitchen();
            if (kitchenId == 0) {
                //std::cout << "there"
                MDMutex.lock();
                MessageMutex.lock();
                addKitchen();
                MDMutex.unlock();
                MessageMutex.unlock();
                std::cout << "the uniqueId is " << uniqueKitchenId << std::endl;
                this->messenger->send_order_to_the_kitchen(uniqueKitchenId, currOrder);
                MDMutex.lock();
                //update kitchenMetaData vector (uniqueKitchenId)
                MDMutex.unlock();
            }
            else
                this->messenger->send_order_to_the_kitchen(kitchenId, currOrder);
            MDMutex.lock();
            //_kitchen_mds. update(KitchenId)
            MDMutex.unlock();

    }
}

///returns 0 if all the kitchens are busy_max;
int Reception::getAvailableKitchen()
{
    for (auto entry : _kitchen_mds)
        std::cout << "available kitchens: "<< entry._ownId << std::endl;

    return 0;
}

void Reception::run() {
    std::string input;
    std::string buffer;
    std::vector<Order> orders;
    this->addKitchen();
    std::thread window(&Reception::runWindow, this);
    //std::thread window(do_loop);
    //andree thread finisch();
    int order_nb = 0;
    struct mq_attr attr;
    while (true) {
        for (auto meta: _kitchen_mds) {
            mq_getattr(meta.deliveryQueue, &attr);
            if (attr.mq_curmsgs != 0) {
                this->messenger->rcv_kitchen_reply(meta._ownId, buffer);
                if (buffer != "") {
                    std::cout << " this is what the kitchen said: " << buffer << std::endl;
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
        Kitchen kitchen(this->_cooks, this->uniqueKitchenId, this->_refill);
    }

    KMetaData metaData;
    metaData._ownId = uniqueKitchenId;
    metaData.busy_cooks = 0;
    metaData.currOrders = 0;
    metaData.orders_max = this->_cooks * 2;

    _kitchen_mds.push_back(metaData);
}

/*
PizzaType Reception::strToPizzaType(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str.compare("regina") == 0)
        return Regina;
    if (str.compare("margarita") == 0)
        return Margarita;
    if (str.compare("americana") == 0)
        return Americana;
    if (str.compare("fantasia") == 0)
        return Fantasia;
    throw ParseError("wrong pizza type");
}

PizzaSize Reception::strToPizzaSize(std::string str)
{
    if (str.compare("S") == 0)
        return S;
    if (str.compare("M") == 0)
        return M;
    if (str.compare("L") == 0)
        return L;
    if (str.compare("XL") == 0)
        return XL;
    if (str.compare("XXL") == 0)
        return XXL;
    throw ParseError("wrong PizzaSize");
}*/
