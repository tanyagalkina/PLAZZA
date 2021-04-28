
#include "Reception.hpp"

std::ostream &operator<<(std::ostream &out, order_t &order)
{
    out << "Size: " << order._size << " Type: " << order._type << std::endl;
    return out;
}

std::vector<order_t> Reception::parse(const std::string &str)
{
    int howMany;
    std::vector<order_t> order;
    std::vector<std::string> lines = split(str, ';');

    for (const std::string &line : lines) {
        std::vector<std::string> parts = split(line, ' ');

        if (parts.size() != 3 || parts[2][0] != 'x')
            throw ParseError ("bad input");

        parts[2].erase(std::remove(parts[2].begin(), parts[2].end(), 'x'), parts[2].end());
        howMany = std::stoi(parts[2]);

        if (howMany >= 100 || howMany <= 0)
            throw ParseError ("bad number");

        while (howMany-- != 0) {
            order_t tmp = { strToPizzaType(parts[0]), strToPizzaSize(parts[1]) };
            order.push_back(tmp);
        }
    }
    return order;
}

Reception::Reception(float mulitpy, int cooks, int refill)
    : _multiply(mulitpy), _cooks(cooks), _refill(refill)
{
}

void Reception::run()
{
    std::cout << "running" << std::endl;
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
