#include "../exercise.h"
#include <iostream>

class DynFibonacci {
    size_t *cache;
    int capacity;
    int cached;

public:
    DynFibonacci(int cap): cache(new size_t[cap]), capacity(cap), cached(0) {
        // 初始化前两个斐波那契数
        if (capacity > 0) {
            cache[0] = 0;
            cached = 1;
        }
        if (capacity > 1) {
            cache[1] = 1;
            cached = 2;
        }
    }

    ~DynFibonacci() {
        delete[] cache;
    }

    size_t get(int i) {
        // 处理负索引
        if (i < 0) return 0;
        
        // 如果i超出当前缓存范围，扩展缓存
        if (i >= capacity) {
            size_t *new_cache = new size_t[i + 1];
            for (int j = 0; j < cached; ++j) {
                new_cache[j] = cache[j];
            }
            delete[] cache;
            cache = new_cache;
            capacity = i + 1;
        }
        
        // 计算缺失的斐波那契数
        for (int j = cached; j <= i; ++j) {
            if (j == 0) cache[0] = 0;
            else if (j == 1) cache[1] = 1;
            else cache[j] = cache[j - 1] + cache[j - 2];
        }
        
        // 更新缓存计数
        if (i >= cached) {
            cached = i + 1;
        }
        
        return cache[i];
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
