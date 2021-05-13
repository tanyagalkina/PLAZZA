#include "Kitchen.hpp"
#include "Error.hpp"
#include <memory>


void do_join()
{
    std::cout << "Ich bin slave!!\n";
    sleep(3);
    Messenger::send_reply_to_reception(6, "worker DONE!");

}

void do_join1()
{
    std::cout << "Ich bin slave too!!\n";
    sleep(3);
    Messenger::send_reply_to_reception(6, "woker 1I am DON!");

}

Kitchen::Kitchen(int cooks, int ownId)
{
    std::string buffer;
    this->_ownId = ownId;

    /*try {
        this->_pool = std::make_unique<ThreadPool>(cooks);
    } catch (const TooManyThread &e) {
        std::cerr << "ThreadPool could not get created. " << e.what() << std::endl;
    }*/
    struct mq_attr atributes;
    this->initMessageQueue();
    while (1)
    {
        /*mq_getattr(this->mqfdOrders, &atributes);
        if (atributes.mq_curmsgs == 0)
            std::cout << "There are no orders currently\n";
        //int rc = msgctl()*/
        Messenger::get_order_from_reception(this->mqfdOrders, buffer);

        std::cout << "the reception has ordered:" << buffer << std::endl;
        //sleep(3);
        std::thread worker(do_join);
        std::thread worker1(do_join1);
        worker.join();
        worker1.join();
        sleep(2);
        //Messenger::send_reply_to_reception(this->mqfdDeliveries, buffer + "DONE!");


    }
}

void Kitchen::initMessageQueue()
{
    struct mq_attr attr;
    std::string name("/stub" + std::to_string((this->_ownId - 1) * 100) + "order");

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 20;
    this->mqfdOrders = mq_open(name.c_str(), O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
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


