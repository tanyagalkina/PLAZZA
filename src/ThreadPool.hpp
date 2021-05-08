#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <bits/c++config.h>
#include <memory>
#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <deque>

/* Possible usage of this ThreadPool :) */

/*
    ThreadPool t(2);

    t.addOrder(
        [] () -> void {
        std::cout << "1. started 2s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "1. ended 2s" << std::endl;
    });

    t.addOrder(
        [] () -> void {
        std::cout << "2. started 5s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "2. ended 5s" << std::endl;
    });

    t.addOrder(
        [] () -> void {
        std::cout << "3. started 2s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "3. ended 2s" << std::endl;
    });

    t.joinAll(); */

/* TODO what happens if a kitchen closes does all the threads get destroyed correctly */
class ThreadPool
{
public:
    std::vector<std::shared_ptr<std::thread>> _cooks;
    std::mutex _mutex;
    std::deque<std::function<void ()>> _orders;
    bool _isFinished;

public:
    explicit ThreadPool(std::size_t threads);

    void addOrder(const std::function<void ()> &order);

    /* this gets called after all tasks have been added */
    void joinAll();

private:
    /* actual function that get executed by the thread */
    void exec();
};

#endif //THREADPOOL_HPP_
