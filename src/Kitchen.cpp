#include "Kitchen.hpp"
#include "Error.hpp"
#include "ThreadPool.hpp"
#include <fcntl.h>
#include <memory>
#include <string>
#include <thread>

Kitchen::Kitchen(int cooks, int ownId)
{
    std::string buffer;
    this->_ownId = ownId;

    this->initMessageQueue();

    this->_pool = std::make_unique<ThreadPool>(cooks, *this);
    this->_pool->joinAll();
}

void Kitchen::initMessageQueue()
{
    struct mq_attr attr;
    std::string name("/stub" + std::to_string((this->_ownId - 1) * 100) + "order");

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 20;
    this->mqfdOrders = mq_open(name.c_str(), O_RDWR | O_CREAT | O_NONBLOCK, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
    if (this->mqfdOrders == -1) {
        std::cerr << "Cannot open message queue\n";
    }
    name.clear();
    name = "/stub" + std::to_string((this->_ownId - 1) * 100) + "delivery";
    //std::cout << "the name of delivery queue is" << name << std::endl;
    this->mqfdDeliveries = mq_open(name.c_str(), O_WRONLY | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
    if (this->mqfdDeliveries == -1) {
        std::cerr << "Cannot open message queue\n";
    }
}


