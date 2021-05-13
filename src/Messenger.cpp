//
// Created by tag43 on 5/11/21.
//

#include "Messenger.hpp"
#include <cstring>
#include <fcntl.h>
#include <mqueue.h>
#include <iostream>

Messenger::Messenger()
{
    this->nb_queues = 0;
}

Messenger::~Messenger()
{
    for (int i = 0; i < this->orders.size(); i++) {
        mq_close(this->orders[i].second);
    }
    for (int j = 0; j < this->deliveries.size(); j++) {
        mq_close(this->deliveries[j].second);
    }
}

void Messenger::create_new_pair(int kitchen_id)
{
    mqd_t mqfd;
    struct mq_attr attr;
    std::string name("/stub" + std::to_string(this->nb_queues * 100) + "order");

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 20;
    mq_unlink(name.c_str());
    mqfd = mq_open(name.c_str(), O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
    if (mqfd == -1) {
        std::cerr << "Cannot open message queue.";
        //throw OpenQueueException("Cannot open message queue.");
    }
    std::pair<int, mqd_t> new_order {kitchen_id, mqfd};
    this->orders.push_back(new_order);
    name.clear();
    name = "/stub" + std::to_string(this->nb_queues * 100) + "delivery";
    //std::cout << "the name of delivery added is:" << name << std::endl;
    mq_unlink(name.c_str());
    mqfd = mq_open(name.c_str(), O_RDWR | O_CREAT | O_NONBLOCK, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
    if (mqfd == -1) {
        std::cerr << "Cannot open message queue.\n";
        //throw OpenQueueException("Cannot open message queue.");
    }
    std::pair<int, mqd_t> new_delivery {kitchen_id, mqfd};
    this->deliveries.push_back(new_delivery);
    this->nb_queues++;
}

void Messenger::send_order_to_the_kitchen(int kitchen_id, const std::string message)
{
    int queue_ind = this->get_queue_index(kitchen_id, 1);
    int ret = 0;

    //std::cout << "the mqfd for sending the order message is: " << this->orders[queue_ind].second << std::endl;
    ret = mq_send(this->orders[queue_ind].second, message.c_str(), message.length() + 1, 0);
    if (ret == -1) {
        std::cerr << "Cannot send the message to the kitchen.";
        //throw MessageQueueException("Sending message failed in message queue.", "Messenger");
    }
}

int Messenger::rcv_kitchen_reply(int kitchen_id, std::string &buffer)
{
    int queue_index = this->get_queue_index(kitchen_id, 2);
    int ret = 0;
    char buff[20];

    //std::cout << "to receive reply from the kitchen we use: " << this->deliveries[queue_index].second << std::endl;

    ret = mq_receive(this->deliveries[queue_index].second, buff, 20, 0);
    if (ret == -1 && errno != EAGAIN) {
        std::cerr << "Cannot receive message.\n";
        //throw MessageQueueException("Cannot receive message");
    }
    if (ret > 0)
        buffer.assign(buff);
    return (ret);
}

void Messenger::send_reply_to_reception(mqd_t mqfd, const std::string message)
{
    int ret = 0;
    //std::cout << "to send the reply to the reception we use: " << mqfd << std::endl;

    ret = mq_send(mqfd, message.c_str(), message.length() + 1, 0);
    if (ret == -1) {
        std::cerr << "Cannot send the message from the kitchen." << std::endl;
        //throw MessageQueueException("Sending message failed in message queue.", "Messenger");
    }
}

void Messenger::get_order_from_reception(mqd_t mqfd, std::string &buffer)
{
    int ret = 0;
    char buff[20];

    //std::cout << "to get the order form reception we use is: " << mqfd << std::endl;
    ret = mq_receive(mqfd, buff, 20, 0);
    if (ret == -1) {
        std::cerr << "Cannot receive message." << std::endl;
        //throw MessageQueueException("Receiving message failed in message queue", "Messenger");
    }
    if (ret > 0)
        buffer.assign(buff);
}

int Messenger::get_queue_index(int kitchen_id, int mode)
{
    if (mode == 1) {
        for (int i = 0; i < this->orders.size(); i++) {
            if (this->orders[i].first == kitchen_id)
                return (i);
        }
    } else if (mode == 2) {
        for (int j = 0; j < this->deliveries.size(); j++) {
            if (this->deliveries[j].first == kitchen_id)
                return (j);
        }
    }
    std::cerr << "The requested queue does not exist." << std::endl;
    //throw InvalidQueueException("A non-existing queue was requested", "Messenger");
}