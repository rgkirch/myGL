#include <cstdlib>
#include <iostream>

int main() {
    for(int i = 0; i < 20; ++i) {
        std::cout << rand() % 1000 << std::endl;
    }
    return 0;
}
