#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include "ThreadPool.hpp"
#include <mqueue.h>
#include <unistd.h>
#include "Messenger.hpp"

typedef struct products {
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
    int doe;
    int tomato;
    int gruyere;
    int ham;
    int mushrooms;
    int steak;
    int eggplant;
    int goat_cheese;
}prod_t;


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
        prod_t storage;
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
