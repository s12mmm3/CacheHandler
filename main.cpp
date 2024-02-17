#include <iomanip>
#include <iostream>
#include <chrono>
#include <thread>

#include "cachehandler.h"

int main()
{
    CacheHandler<std::time_t, int, int> cacheHandler([](int a, int b) ->std::time_t {
        std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
        std::time_t c = std::chrono::system_clock::to_time_t(t);
        return c;
    }, 5000);
    auto result = [&]() {
        const auto c = cacheHandler.value(0, 1);
        std::cout << "Cache time: " << std::put_time(std::localtime(&c), "%F %T") << std::endl;
    };
    result();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    result();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    result();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    result();
    return 0;
}
