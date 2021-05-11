//
// Created by tag43 on 5/11/21.
//

#ifndef PLAZZA_MESSENGER_HPP
#define PLAZZA_MESSENGER_HPP

#include <vector>
#include <string>
#include <cstdio>
#include <mqueue.h>
#include <time.h>

class Messenger {
public:
    Messenger();
    ~Messenger();
    void create_new_pair(int kitchen_id);
    void send_order_to_the_kitchen(int kitchen_id, const std::string message);
    int rcv_kitchen_reply(int kitchen_id, std::string &buffer);
    static void send_reply_to_reception(mqd_t mqfd, const std::string message);
    static void get_oder_form_reception(mqd_t mqfd, std::string &buffer);
private:
    int nb_queues;
    std::vector<std::pair<int, mqd_t>> orders;
    std::vector<std::pair<int, mqd_t>> deliveries;
    int get_queue_index(int kitchen_id, int mode);
};

#endif //PLAZZA_MESSENGER_HPP
