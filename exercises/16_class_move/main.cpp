

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

#include "../exercise.h"
#include <iostream>
#include <utility>  // 用于 std::swap

class DynFibonacci {
    size_t *cache = nullptr;
    int capacity = 0;
    int cached = 0;

public:
    // 构造函数
    DynFibonacci(int cap): cache(new size_t[cap]), capacity(cap), cached(0) {
        if (cap > 0) cache[0] = 0, cached = 1;
        if (cap > 1) cache[1] = 1, cached = 2;
    }

    // 移动构造函数
    DynFibonacci(DynFibonacci &&other) noexcept 
        : cache(other.cache), capacity(other.capacity), cached(other.cached) {
        other.cache = nullptr;
        other.capacity = 0;
        other.cached = 0;
    }

    // 移动赋值运算符
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        if (this != &other) {
            // 使用swap技巧，异常安全且简洁
            DynFibonacci temp(std::move(other));
            swap(*this, temp);
        }
        return *this;
    }

    // 交换函数（辅助移动赋值）
    friend void swap(DynFibonacci &a, DynFibonacci &b) noexcept {
        using std::swap;
        swap(a.cache, b.cache);
        swap(a.capacity, b.capacity);
        swap(a.cached, b.cached);
    }

    // 析构函数
    ~DynFibonacci() {
        delete[] cache;
    }

    // 下标运算符（非常量版本）
    size_t operator[](int i) {
        if (i < 0) return 0;
        
        // 扩展缓存如果需要
        if (i >= capacity) {
            size_t *new_cache = new size_t[i + 1];
            for (int j = 0; j < cached; ++j) new_cache[j] = cache[j];
            delete[] cache;
            cache = new_cache;
            capacity = i + 1;
        }
        
        // 计算缺失的值
        for (int j = cached; j <= i; ++j) {
            cache[j] = (j < 2) ? j : cache[j-1] + cache[j-2];
        }
        
        if (i >= cached) cached = i + 1;
        return cache[i];
    }

    // 下标运算符（常量版本）
    size_t operator[](int i) const {
        ASSERT(i < cached, "i out of range");
        return cache[i];
    }

    // 检查对象是否"活着"（即资源是否有效）
    bool is_alive() const {
        return cache != nullptr;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);  // 自赋值
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
