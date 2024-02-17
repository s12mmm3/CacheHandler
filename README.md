# CacheHandler

CacheHandler是一个C++模板类，用于实现缓存管理的功能。它可以根据给定的函数，参数，超时时间和最大容量，自动缓存函数的返回值，并在需要时更新缓存。

## 使用方法
- 需要包含CacheHandler.h头文件

```cpp
#include "CacheHandler.h"

// Define a function that takes two integers and returns their sum
double add(int a, int b) {
    return a + b;
}

int main() {
    // Create a CacheHandler object with add function, 10 seconds timeout and 5 items max size
    CacheHandler<double, int, int> cache(add, 10000, 5);

    // You can use lambda too
    CacheHandler<double, int, int> cache1([](int a, int b) -> double {
        return a + b;
    }, 10000, 5);

    // Get the value of add(1, 2) and cache it
    int result = cache.value(1, 2);
    std::cout << result << std::endl;

    result = cache.value(1, 2);
    std::cout << result << std::endl;

    return 0;
}
```