#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <bits/c++config.h>
#include <memory>
#include <iostream>
#include <mqueue.h>
#include <vector>
#include <thread>
#include <future>
#include <deque>
#include "Error.hpp"

class Kitchen;

class ThreadPool
{

private:
    std::vector<std::shared_ptr<std::thread>> _cooks;
    std::deque<std::function<void ()>> _orders;
    bool _isFinished;
    Kitchen *_kitchen;
    std::mutex _mutex;
    std::mutex _mutexDelivery;
    std::mutex _mutexOrder;
    int _workingCooks;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;
    bool _timeIsCounting = false;
    bool _allQuit = false;

public:
    explicit ThreadPool(std::size_t threads, Kitchen &kitchen);
    void joinAll();
    int getBusyCooks();
    int getFreeCooks();

private:
    void exec();
    int checkKitchenTime();
    int getOrder(std::string &buffer, struct mq_attr &attr);
    int processPizza(std::string &buffer);
    void checkTime();
};

#endif //THREADPOOL_HPP_
