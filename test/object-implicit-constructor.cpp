#include <iostream>

class Charlie;

struct Alice {
    Alice(Charlie);
    int y;
};

struct Charlie {
    Charlie(int);
    int x;
};

Charlie::Charlie(int n) {
    x = n;
}

Alice::Alice(Charlie c) {
    y = c.x;
}

int main() {
    struct Alice a(3);
    std::cout << a.y << std::endl;
    return 0;
}
