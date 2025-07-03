#include <iostream>
#include <random>
#include <chrono>
#include <thread>

int main() {
    const int rate = 16384;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    while (true) {
        float random_float = dis(gen);
        std::cout << random_float << std::endl;
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000/rate));
    }
    return 0;
}
