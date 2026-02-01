#include "../exercise.h"

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>

struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};

struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    char virtual_name() const override {
        return 'B';
    }
    char direct_name() const {
        return 'B';
    }
};

struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    char virtual_name() const final {
        return 'C';
    }
    char direct_name() const {
        return 'C';
    }
};

struct D : public C {
    // D没有重写virtual_name，因为C中的virtual_name是final的
    // 但是可以重写direct_name
    char direct_name() const {
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";

    A a;
    B b;
    C c;
    D d;

    // 直接调用，没有多态
    ASSERT(a.virtual_name() == 'A', MSG);
    ASSERT(b.virtual_name() == 'B', MSG);
    ASSERT(c.virtual_name() == 'C', MSG);
    ASSERT(d.virtual_name() == 'C', MSG);  // D没有重写virtual_name，使用C的版本
    ASSERT(a.direct_name() == 'A', MSG);
    ASSERT(b.direct_name() == 'B', MSG);
    ASSERT(c.direct_name() == 'C', MSG);
    ASSERT(d.direct_name() == 'D', MSG);

    // 引用调用
    A &rab = b;  // A类型引用指向B对象
    B &rbc = c;  // B类型引用指向C对象
    C &rcd = d;  // C类型引用指向D对象

    // 虚函数：运行时多态，根据实际对象类型调用
    ASSERT(rab.virtual_name() == 'B', MSG);  // 实际是B对象
    ASSERT(rbc.virtual_name() == 'C', MSG);  // 实际是C对象
    ASSERT(rcd.virtual_name() == 'C', MSG);  // 实际是D对象，但virtual_name来自C（final）
    
    // 非虚函数：编译时绑定，根据引用类型调用
    ASSERT(rab.direct_name() == 'A', MSG);  // A类型引用调用A的版本
    ASSERT(rbc.direct_name() == 'B', MSG);  // B类型引用调用B的版本
    ASSERT(rcd.direct_name() == 'C', MSG);  // C类型引用调用C的版本

    // 更多引用测试
    A &rac = c;  // A类型引用指向C对象
    B &rbd = d;  // B类型引用指向D对象

    ASSERT(rac.virtual_name() == 'C', MSG);  // 虚函数，根据实际对象类型C
    ASSERT(rbd.virtual_name() == 'C', MSG);  // 虚函数，实际是D对象，但virtual_name来自C
    
    ASSERT(rac.direct_name() == 'A', MSG);  // 非虚函数，根据引用类型A
    ASSERT(rbd.direct_name() == 'B', MSG);  // 非虚函数，根据引用类型B

    A &rad = d;  // A类型引用指向D对象

    ASSERT(rad.virtual_name() == 'C', MSG);  // 虚函数，实际是D对象，但virtual_name来自C
    ASSERT(rad.direct_name() == 'A', MSG);   // 非虚函数，根据引用类型A

    return 0;
}
