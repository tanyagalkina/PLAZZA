#include "Kitchen.hpp"
#include "Error.hpp"
#include <memory>

Kitchen::Kitchen(int cooks)
{
    try {
        this->_pool = std::make_unique<ThreadPool>(cooks);
    } catch (const TooManyThread &e) {
        std::cerr << "ThreadPool could not get created. " << e.what() << std::endl;
    }
}

