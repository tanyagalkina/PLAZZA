#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include "ThreadPool.hpp"
class Kitchen {
public:
    std::unique_ptr<ThreadPool> _pool;
    int _workingThreads;
public:
    Kitchen(int cooks);
};

#endif //KITCHEN_HPP_
