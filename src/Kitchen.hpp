#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include "ThreadPool.hpp"
#include <mqueue.h>
#include <unistd.h>
#include "Messenger.hpp"

    class Kitchen {
    public:
        std::unique_ptr<ThreadPool> _pool;
        int _workingThreads;

        void initMessageQueue();

    public:
        Kitchen(int cooks, int ownId);

    private:
        int _ownId;
        int _nbCooks;
        mqd_t mqfdOrders;
        mqd_t mqfdDeliveries;

        void DoWork(int);
    };


#endif //KITCHEN_HPP_
