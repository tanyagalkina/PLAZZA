#include "Products.hpp"

Products getProductsFromPizzaType(int type)
{
    switch (static_cast<PizzaType>(type)) {
        case PizzaType::Margarita:
            return Products(1, 1, 1, 0, 0, 0, 0, 0);
        case PizzaType::Regina:
            return Products(1, 1, 1, 1, 1, 0, 0, 0);
        case PizzaType::Americana:
            return Products(1, 1, 1, 0, 0, 1, 0, 0);
        case PizzaType::Fantasia:
            return Products(1, 1, 0, 0, 0, 0, 1, 1);
    };
    throw ParseError("getProductsFromPizzaType() should not throw");
}

Products::Products(int d, int t, int g, int h, int m, int s, int e, int go)
    : doe(d), tomato(t), gruyere(g), ham(h), mushrooms(m), steak(s), eggplant(e), goat_cheese(go)
{
}

Products::Products()
    : doe(5), tomato(5), gruyere(5), ham(5), mushrooms(5), steak(5), eggplant(5), goat_cheese(5)
{
}
void Products::increment()
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

Products &Products::operator-=(const Products &other)
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

bool Products::hasEnoughFor(const Products &other)
{
    if (this->doe < other.doe) return false;
    if (this->tomato < other.tomato) return false;
    if (this->gruyere < other.gruyere) return false;
    if (this->ham < other.ham) return false;
    if (this->mushrooms < other.mushrooms) return false;
    if (this->steak < other.steak) return false;
    if (this->eggplant < other.eggplant) return false;
    if (this->goat_cheese < other.goat_cheese) return false;
    return true;
}
