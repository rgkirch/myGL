#include <cstdio>
#include <vector>

struct args {
    enum {A, B} type;
    union {
        struct {
            int x;
            int y;
        };
        struct {
            double z;
        };
    };
};

int num = 0;
void(*currentFunction)(args);
std::vector<void(*)(args)> functions;

//void(*function)();


void inc(args arg) {
    switch (arg.type) {
        case args::A:
            num += arg.x + arg.y;
            break;
        case args::B:
            num += arg.z;
            break;
    }
}


void dec(args arg) {
    switch (arg.type) {
        case args::A:
            num -= arg.x + arg.y;
            break;
        case args::B:
            num -= arg.z;
            break;
    }
}

int main() {
    functions.push_back(inc);
    functions.push_back(dec);
    currentFunction = functions[0];
    currentFunction(args {args::A, 1, 2});
    functions[1](args {args::B, 0.5});
    printf("%d\n", num);
    return 0;
}

