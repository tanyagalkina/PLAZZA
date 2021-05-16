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
    _allQuit = true;
}

ThreadPool::ThreadPool(std::size_t threads, Kitchen &kitchen)
{
    this->_kitchen = &kitchen;
    if (threads > std::thread::hardware_concurrency())
        throw TooManyThread("You tried to create too many threads");

    for (std::size_t i = 0; i < threads; i++) {
        _cooks.push_back(std::shared_ptr<std::thread>(new std::thread(&ThreadPool::exec, this))); }

    std::thread checkTimeThread(&ThreadPool::checkTime, this);
    _workingCooks = 0;
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

int ThreadPool::checkKitchenTime()
{
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
        if (duration.count() > 30) {
            std::cout << "5 seconds are over now" << std::endl;
            return 1;
        }
    }
    return 0;
}

int ThreadPool::getOrder(std::string &buffer, struct mq_attr &attr)
{
    Messenger::get_order_from_reception(_kitchen->mqfdOrders, buffer);
    if (buffer.empty()) {
        this->_mutexOrder.unlock();
        return 1;
    }

    mq_getattr(_kitchen->mqfdOrders, &attr);
    if (attr.mq_curmsgs == 0) {
        std::this_thread::yield();
    }
    _workingCooks++;
    return 0;
}

void ThreadPool::processPizza(std::string &buffer)
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

    //while (!_kitchen->storage.hasEnoughFor(needed));

    std::this_thread::sleep_for(std::chrono::seconds(timeToCook));
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

        //this->_mutex.lock();
        //if (checkKitchenTime() != 0) {
            //this->_mutex.unlock();
            //break;
        //}

        this->_mutex.unlock();

        this->_mutexOrder.lock();
        if (getOrder(buffer, attr) != 0)
            continue;
        this->_mutexOrder.unlock();

        processPizza(buffer);

        this->_mutexDelivery.lock();
        std::cout << std::this_thread::get_id() << std::endl;
        Messenger::send_reply_to_reception(6, "3\n");
        _workingCooks--;
        this->_mutexDelivery.unlock();
    }
}
