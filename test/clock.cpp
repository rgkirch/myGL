#include <chrono>
#include <iostream>

int main() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    tp += std::chrono::seconds(1);
    while(std::chrono::system_clock::now() < tp) {
        std::cout << "busy wait" << std::endl;
    }
}
