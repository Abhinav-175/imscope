#include <iostream>
#include <chrono>

int main() {
    int count = 0;
    auto start = std::chrono::steady_clock::now();
    float number;

    while (std::cin >> number) {
        count++;
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        if (elapsed_seconds.count() >= 1.0) {
            std::cout << count << " samples/sec" << std::endl;
            count = 0;
            start = std::chrono::steady_clock::now();
        }
    }
    return 0;
}

