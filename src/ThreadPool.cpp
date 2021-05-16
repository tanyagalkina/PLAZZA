#include "ThreadPool.hpp"
#include "Error.hpp"
#include "Parser.hpp"
#include "Kitchen.hpp"
#include "Products.hpp"
#include <bits/types/__FILE.h>
#include <ctime>
#include <ios>
#include <mqueue.h>
#include <thread>

void ThreadPool::checkTime()
{
    while (true) {
        if (_workingCooks == 0 && !_timeIsCounting) {
            start = std::chrono::high_resolution_clock::now();
            _timeIsCounting = true;
        }

        if (_workingCooks != 0) {
            _timeIsCounting = false;
        }

        if (_timeIsCounting) {
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            if (duration.count() > 5) {
                std::cout << "5 seconds are over now" << std::endl;
                break;
            }
        }
    }
    _mutex.lock();
    _allQuit = true;
    _mutex.unlock();
}

ThreadPool::ThreadPool(std::size_t threads, Kitchen &kitchen)
{
    this->_kitchen = &kitchen;
    if (threads > std::thread::hardware_concurrency())
        throw TooManyThread("You tried to create too many threads");

    for (std::size_t i = 0; i < threads; i++) {
        _cooks.push_back(std::shared_ptr<std::thread>(new std::thread(&ThreadPool::exec, this))); }

    _workingCooks = 0;
    std::thread checkTimeThread(&ThreadPool::checkTime, this);
    checkTimeThread.join();
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

int ThreadPool::getOrder(std::string &buffer, struct mq_attr &attr)
{
    Messenger::get_order_from_reception(_kitchen->mqfdOrders, buffer);
    if (buffer.empty()) {
        return 1;
    }

    mq_getattr(_kitchen->mqfdOrders, &attr);
    if (attr.mq_curmsgs == 0) {
        std::this_thread::yield();
    }
    if (buffer == "status") {
        printf("Kitchen status (%d): \n", _kitchen->_ownId);
        _kitchen->storage.dump();
        return 1;
    }
    _workingCooks++;
    return 0;
}

int ThreadPool::processPizza(std::string &buffer)
{
    int orderID;
    int timeToCook;
    int pizzaType;

    try {
        parse_pizza(buffer, orderID, timeToCook, pizzaType);
    } catch (const ParseError &e) {
        std::cerr << __FILE__ << ": " << e.what() << std::endl;
    }

    const auto &needed = getProductsFromPizzaType(pizzaType);

    while (!_kitchen->storage.hasEnoughFor(needed));

    _kitchen->storage -= needed;

    std::this_thread::sleep_for(std::chrono::seconds(timeToCook * _kitchen->_multiply));
    return orderID;
}

void ThreadPool::exec()
{
    std::string buffer;
    struct mq_attr attr;

    while (true) {
        this->_mutex.lock();
        if (_allQuit) {
            this->_mutex.unlock();
            break;
        }
        this->_mutex.unlock();

        this->_mutexOrder.lock();
        if (getOrder(buffer, attr) != 0) {
            this->_mutexOrder.unlock();
            continue;
        }
        this->_mutexOrder.unlock();

        int orderNumber = processPizza(buffer);

        this->_mutexDelivery.lock();
        _workingCooks--;
        Messenger::send_reply_to_reception(_kitchen->mqfdDeliveries, std::to_string(orderNumber));
        this->_mutexDelivery.unlock();
    }
}
