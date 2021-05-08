#include <criterion/criterion.h>
#include "../src/ThreadPool.hpp"
#include "../src/Error.hpp"

Test(too_many_threads, creating_too_many_threads)
{
    bool ok = true;

    try {
        ThreadPool t(100);
    } catch (const TooManyThread &e) {
        ok = false;
    }
    cr_assert(!ok);
}

Test(test_time, two_threads_try_2s_4s_2s_in_max_4s)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;

    ThreadPool t(2);

    start = std::chrono::high_resolution_clock::now();

    t.addOrder( [] () -> void { std::this_thread::sleep_for(std::chrono::seconds(2)); });
    t.addOrder( [] () -> void { std::this_thread::sleep_for(std::chrono::seconds(4)); });
    t.addOrder( [] () -> void { std::this_thread::sleep_for(std::chrono::seconds(2)); });
    t.joinAll();

    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout << "Timer: " << duration.count() << "s" << std::endl;
    cr_assert(duration.count() < 4.1 && duration.count() > 3.9);
}
