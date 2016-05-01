#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <cstdio>
#include <list>

void hi() {
    printf("hello\n");
}

struct Item {
    void operator()(int x) {
       boost::this_thread::sleep_for(boost::chrono::seconds{x});
        printf("hello\n");
    }
};

int main() {
    Item x;
    Item y;
    boost::thread xt{x, 1};
    boost::thread yt{y, 5};
    yt.join();
    printf("yt joined\n");
    xt.join();
    printf("xt joined\n");
    return 0;
}
