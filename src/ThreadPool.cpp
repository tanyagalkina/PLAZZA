#include "ThreadPool.hpp"
#include "Error.hpp"
#include "Parser.hpp"
#include "Kitchen.hpp"
#include <bits/types/__FILE.h>
#include <ctime>
#include <ios>
#include <thread>

ThreadPool::ThreadPool(std::size_t threads, Kitchen &kitchen)
{
    this->_kitchen = &kitchen;
    if (threads > std::thread::hardware_concurrency())
        throw TooManyThread("You tried to create too many threads");

    for (std::size_t i = 0; i < threads; i++) {
        _cooks.push_back(std::shared_ptr<std::thread>(new std::thread(&ThreadPool::exec, this))); }

    _workingCooks = 0;
}

int ThreadPool::getBusyCooks()
{
    return _workingCooks;
}

int ThreadPool::getFreeCooks()
{
    return _cooks.size() - _workingCooks;
}

void ThreadPool::joinAll()
{
    _mutex.lock();
    _isFinished = true;
    _mutex.unlock();

    for (auto &cook : _cooks) {
        if (cook->joinable())
            cook->join();
    }
}

void ThreadPool::exec()
{
    std::string buffer;
    struct mq_attr atributes;

    while (true) {
        this->_mutexOrder.lock();

        Messenger::get_order_from_reception(_kitchen->mqfdOrders, buffer);
        // orderID timeToCook

        mq_getattr(_kitchen->mqfdOrders, &atributes);
        if (atributes.mq_curmsgs == 0) {
            std::this_thread::yield();
        }
        _workingCooks++;
        this->_mutexOrder.unlock();

        if (buffer == "STATUS") {
            this->_mutexDelivery.lock();
            _workingCooks--;
            Messenger::send_reply_to_reception(6, "Free cooks right now: " + std::to_string(getFreeCooks()));
            this->_mutexDelivery.unlock();
            continue;
        }
        int orderID;
        int timeToCook;

        printf("buffer: #%s#", buffer.c_str());

        try {
            parse_pizza(buffer, orderID, timeToCook);
        } catch (const ParseError &e) {
            std::cerr << __FILE__ << ": " << e.what() << std::endl;
        }

        printf("Cooking pizza from orderID = %d for %d seconds\n", orderID, timeToCook);
        fflush(stdout);

        std::this_thread::sleep_for(std::chrono::seconds(timeToCook));

        this->_mutexDelivery.lock();
        Messenger::send_reply_to_reception(6, "Done!!!\n");
        _workingCooks--;
        this->_mutexDelivery.unlock();
    }
}
