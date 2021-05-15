#include "ThreadPool.hpp"
#include "Error.hpp"
#include "Parser.hpp"
#include "Kitchen.hpp"
#include <bits/types/__FILE.h>
#include <ctime>
#include <ios>
#include <mqueue.h>
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

/* note:
 *  added 'errno != EAGAIN' to not print error message when queue is empty
 *  added O_NONBLOCK to Kitchen::initMessageQueue()
 *  now since the message queue of the kitchen is not blocking anymore the
 *  counter is working like it should and for now its printing the message
 *  after 5 seconds of doing nothing
 */

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
        if (duration.count() > 5) {
            //@todo send message to the reception that this kitchen should close now
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

    try {
        parse_pizza(buffer, orderID, timeToCook);
    } catch (const ParseError &e) {
        std::cerr << __FILE__ << ": " << e.what() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(timeToCook));
}

void ThreadPool::exec()
{
    std::string buffer;
    struct mq_attr attr;

    while (true) {
        this->_mutex.lock();
        if (checkKitchenTime() != 0) {
            this->_mutex.unlock();
            break;
        }

        this->_mutex.unlock();

        this->_mutexOrder.lock();
        if (getOrder(buffer, attr) != 0)
            continue;
        this->_mutexOrder.unlock();

        processPizza(buffer);

        this->_mutexDelivery.lock();
        Messenger::send_reply_to_reception(6, "Done!!!\n");
        _workingCooks--;
        this->_mutexDelivery.unlock();
    }
}
