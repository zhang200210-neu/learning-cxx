#include "../exercise.h"
//递归很可怕，内存消耗大，时间复杂度高，所以我们可以用循环来实现斐波那契数列
constexpr unsigned long long fibonacci(int i) {
    if(i<=1)return i;
    unsigned long long a=0,b=1,c=0;
    for(int j=2;j<=i;j++){
        c=a+b;
        a=b;
        b=c;
    }
    return b;
}

int main(int argc, char **argv) {
    constexpr auto FIB20 = fibonacci(20);
    ASSERT(FIB20 == 6765, "fibonacci(20) should be 6765");
    std::cout << "fibonacci(20) = " << FIB20 << std::endl;

    // TODO: 观察错误信息，修改一处，使代码编译运行
    constexpr auto ANS_N = 100;
    auto ANS = fibonacci(ANS_N);
    std::cout << "fibonacci(" << ANS_N << ") = " << ANS << std::endl;

    return 0;
}


