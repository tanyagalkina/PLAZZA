#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include "ThreadPool.hpp"
#include <mqueue.h>
#include <unistd.h>
#include "Messenger.hpp"
#include "Pizza.hpp"
#include "Products.hpp"

class Kitchen {
public:
        std::unique_ptr<ThreadPool> _pool;
        int _workingThreads;

        void initMessageQueue();

public:
        Kitchen(int cooks, int ownId, int refill, int multiply);
        std::thread  signSupplyContract();
        void supplyKitchen();

public:
        int _ownId;
        int _nbCooks;
        int _refill;
        int _multiply;
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
