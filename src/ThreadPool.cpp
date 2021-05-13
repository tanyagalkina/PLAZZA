#include "ThreadPool.hpp"
#include <ios>
#include <thread>

ThreadPool::ThreadPool(std::size_t threads)
{
    if (threads > std::thread::hardware_concurrency())
        throw TooManyThread("You tried to create too many threads");

    for (std::size_t i = 0; i < threads; i++) {
        _cooks.push_back(std::shared_ptr<std::thread>(new std::thread(&ThreadPool::exec, this))); }
}

void ThreadPool::addOrder(const std::function<void ()> &order)
{
    _mutex.lock();
    _orders.push_back(order);
    _mutex.unlock();
}

void ThreadPool::joinAll()
{
    _mutex.lock();
    _isFinished = true;
    _mutex.unlock();

    for (auto &cook : _cooks) {
        std::cout << cook->get_id() << ": " << std::boolalpha << cook->joinable() << std::endl;
        if (cook->joinable())
            cook->join();
    }
}

void ThreadPool::exec()
{
    bool isRunning = true;

    while (isRunning) {
        _mutex.lock();

        if (!_orders.empty()) {
            auto order = _orders.front();
            _orders.pop_front();

            std::cout << std::this_thread::get_id() << "took the order!\n";
            _mutex.unlock();
            order();
        } else {
            std::this_thread::yield();
            //isRunning = !_isFinished;
            _mutex.unlock();
        }
    }
}
