#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include "ThreadPool.hpp"
#include <mqueue.h>
#include <unistd.h>
#include "Messenger.hpp"
#include "Pizza.hpp"

class Products {
public:
    /* this constructor is only used for creation with getProductsFromPizzaType() */
    Products(int d, int t, int g, int h, int m, int s, int e, int go)
        : doe(d), tomato(t), gruyere(g), ham(h), mushrooms(m), steak(s), eggplant(e), goat_cheese(go)
    {
    }

    /* by default init with all ingrediences set to 5 */
    Products()
        : doe(5), tomato(5), gruyere(5), ham(5), mushrooms(5), steak(5), eggplant(5), goat_cheese(5)
    {
    }

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

    Products &operator-=(const Products &other)
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

    bool hasEnoughFor(const Products &other)
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
}

class Kitchen {
public:
        std::unique_ptr<ThreadPool> _pool;
        int _workingThreads;

        void initMessageQueue();

public:
        Kitchen(int cooks, int ownId);
        std::thread  signSupplyContract();
        void supplyKitchen();

public:
        int _ownId;
        int _nbCooks;
        mqd_t mqfdOrders;
        mqd_t mqfdDeliveries;
        Products storage;
        std::thread supply;
        std::vector<std::shared_ptr<std::thread>> _cooks;

        int checkTime = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        std::chrono::duration<float> duration;

        void createCook();
        void DoWork(int);
        void work();
};


#endif //KITCHEN_HPP_
