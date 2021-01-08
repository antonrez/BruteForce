#pragma once
#include <future>

class MySimpleTimer {
public:
    MySimpleTimer();
    ~MySimpleTimer();
private:
    std::chrono::time_point<std::chrono::steady_clock>start, end;
};
