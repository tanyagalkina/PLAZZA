
#include "Reception.hpp"

std::ostream &operator<<(std::ostream &out, order_t &order)
{
    out << "Size: " << order._size << " Type: " << order._type << std::endl;
    return out;
}

void Reception::getInput()
{
    _order.clear();
    std::string input;
    std::getline(std::cin, input);

    auto lines = Parser::parseString(input, ";");

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
    }
}

Reception::Reception(float mulitpy, int cooks, int refill)
    : _multiply(mulitpy), _cooks(cooks), _refill(refill)
{
    std::unique_ptr<Messenger> messenger(new Messenger());
    this->messenger = std::move(messenger);
    this->_nbKitchens = 0;
}

void Reception::run()
{
    std::string input;
    std::string buffer;
    std::vector<order_t> orders;
    this->addKitchen();
    while (true) {
        try {
            getInput();
        } catch (const ParseError &e) {
            std::cerr << e.what() << std::endl;
        }
        this->messenger->send_order_to_the_kitchen(1, "myinput");
        sleep(3);
        this->messenger->rcv_kitchen_reply(1, buffer);
        std::cout << "this is what the kitchen said: " << buffer << std::endl;
    }

    std::cout << "running" << std::endl;
}

void Reception::addKitchen()
{
    pid_t pid;

    this->_nbKitchens++;
    this->messenger->create_new_pair(this->_nbKitchens);
    pid = fork();
    if (pid == 0) {
        Kitchen kitchen(this->_cooks, this->_nbKitchens);
    }
}


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
}
