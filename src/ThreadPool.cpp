#include "ThreadPool.hpp"

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
        cook->join();
    }
}

void ThreadPool::exec()
{
    bool isRunning = true;

    while (isRunning) {
        /* thread stays here if the _mutex is locked so it waits for the others
         * and dont modify anything that could be modified by another thread at
         * the same time */
        _mutex.lock();

        if (!_orders.empty()) {
            auto order = _orders.front();
            _orders.pop_front();

            /* always unlock after calling lock -> otherwise there will be a
             * deadlock and the program doesnt know what to do */
            _mutex.unlock();
            /* call the lambda function that was passed in as a parameter to
             * addOrder() */
            order();
        } else {
            /* this is a variable that can be modified by multiple thread at
             * the same time so it has to be INSIDE the lock */
            isRunning = !_isFinished;
            _mutex.unlock();
        }
    }
}
