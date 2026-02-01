#include "../exercise.h"
#include <iostream>

class DynFibonacci {
    size_t *cache;
    int capacity;
    int cached;

public:
    DynFibonacci(int cap): cache(new size_t[cap]), capacity(cap), cached(0) {
        // 初始化缓存
        if (cap > 0) {
            cache[0] = 0;
            cached = 1;
        }
        if (cap > 1) {
            cache[1] = 1;
            cached = 2;
        }
    }

    // 删除复制构造函数，禁止复制
    DynFibonacci(DynFibonacci const &) = delete;
    
    // 也可以删除移动构造函数（可选）
    DynFibonacci(DynFibonacci &&) = delete;
    
    // 删除赋值运算符
    DynFibonacci& operator=(DynFibonacci const &) = delete;

    ~DynFibonacci() {
        delete[] cache;
    }

    // 非常量版本：可以修改对象，可以扩展缓存
    size_t get(int i) {
        // 处理边界情况
        if (i < 0) return 0;
        
        // 如果索引超出当前缓存容量，扩展缓存
        if (i >= capacity) {
            size_t *new_cache = new size_t[i + 1];
            for (int j = 0; j < cached; ++j) {
                new_cache[j] = cache[j];
            }
            delete[] cache;
            cache = new_cache;
            capacity = i + 1;
        }
        
        // 计算缺失的值
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

    // 常量版本：只能读取已有的缓存值
    size_t get(int i) const {
        // 检查索引是否在缓存范围内
        if (i >= 0 && i < cached) {
            return cache[i];
        }
        // 如果超出范围，触发断言
        ASSERT(false, "i out of range");
        return 0; // 为了编译通过
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    
    // 创建常量引用，使用常量版本的get方法
    DynFibonacci const &fib_ref = fib;
    ASSERT(fib_ref.get(10) == fib.get(10), "Object cloned");
    
    return 0;
}
