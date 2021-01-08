#include "SimpleTimer.h"
#include <iostream>

    MySimpleTimer:: MySimpleTimer() {
        start = std::chrono::high_resolution_clock::now();
    }
    MySimpleTimer:: ~MySimpleTimer() {
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        std::cout << "Time procces - " << duration.count() << "sec." << std::endl;
    }

