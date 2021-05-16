#include "Kitchen.hpp"
#include "Error.hpp"
#include "ThreadPool.hpp"
#include <fcntl.h>
#include <memory>
#include <string>
#include <thread>


void Kitchen::supplyKitchen()
{
    while (true)
    {
        storage.increment();
        std::this_thread::sleep_for(std::chrono::milliseconds(_refill));
    }
}

std::thread Kitchen::signSupplyContract()
{
    std::thread supply(&Kitchen::supplyKitchen, this);
    return supply;
}


Kitchen::Kitchen(int cooks, int ownId, int refill, int multiply)
{
    std::string buffer;
    this->_ownId = ownId;
    this->_refill = refill;
    this->_multiply = multiply;

    this->initMessageQueue();
    supply = this->signSupplyContract();

    this->_pool = std::make_unique<ThreadPool>(cooks, *this);
    this->_pool->joinAll();
    supply.detach();
    Messenger::send_reply_to_reception(mqfdDeliveries, "0");
    std::cout << "We are closing" << std::endl;
    exit (0);

}

void Kitchen::initMessageQueue()
{
    struct mq_attr attr;
    std::string name("/stub" + std::to_string((this->_ownId) * 100) + "order");
    //std::cout << "the queue created by the kitchen is:" << name << std::endl;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 20;
    this->mqfdOrders = mq_open(name.c_str(), O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
    if (this->mqfdOrders == -1) {
        std::cerr << "Cannot open message queue\n";
    }
    name.clear();
    name = "/stub" + std::to_string((this->_ownId) * 100) + "delivery";
    //std::cout << "the name of delivery queue is" << name << std::endl;
    this->mqfdDeliveries = mq_open(name.c_str(), O_WRONLY | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
    if (this->mqfdDeliveries == -1) {
        std::cerr << "Cannot open message queue\n";
    }
}
