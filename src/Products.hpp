#ifndef PRODUCTS_HPP_
#define PRODUCTS_HPP_

#include "Pizza.hpp"
#include "Error.hpp"

class Products {
public:
    Products(int d, int t, int g, int h, int m, int s, int e, int go);
    Products();

    void increment();
    Products &operator-=(const Products &other);
    bool hasEnoughFor(const Products &other);

private:
    int doe;
    int tomato;
    int gruyere;
    int ham;
    int mushrooms;
    int steak;
    int eggplant;
    int goat_cheese;
};

Products getProductsFromPizzaType(int type);

#endif //PRODUCTS_HPP_
