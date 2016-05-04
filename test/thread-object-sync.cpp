#include <iostream>
#include <unordered_map>
#include <condition_variable>
#include <memory>
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <queue>

class Child;

struct Parent {
    void spawn();
    void loop();
    int numChildren;
    //std::list<std::unique_ptr<std::thread>> children;
    std::unordered_map<std::thread::id, std::unique_ptr<std::thread>> children;
    std::mutex joinableMutex;
    std::condition_variable joinableCV;
    std::queue<std::thread::id> joinable;
};

class Child {
public:
    Child(Parent *parent);
    ~Child();
    void loop();
    Parent *parent;
};

int main() {
    printf("main started\n");
    int num = 1;
    printf("construct parent\n");
    std::unique_ptr<Parent> p(new Parent());
    for(int i = 0; i < num; ++i) {
        printf("spawn child\n");
        p->spawn();
    }
    printf("call parent loop from main\n");
    p->loop();
    printf("main finished\n");
    return 0;
}

Child::Child(Parent *parent) {
    printf("child constructor\n");
    this->parent = parent;
}

Child::~Child() {
    printf("child destructor\n");
    std::unique_lock<std::mutex> lk(parent->joinableMutex);
    parent->joinable.push(std::this_thread::get_id());
    parent->joinableCV.notify_one();
}

void Child::loop() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    printf("child work\n");
}

void Parent::spawn() {
    std::thread::id id;
    std::unique_ptr<std::thread> tptr(new std::thread(
        [&] {
            id = std::this_thread::get_id();
            std::unique_ptr<Child> p(new Child(this));
            p->loop();
        }
    ));
    children.insert(std::make_pair(std::move(id), std::move(tptr)));
    numChildren++;
}

void Parent::loop() {
    printf("start parent loop\n");
    printf("lock \n");
    std::unique_lock<std::mutex> lk(joinableMutex);
    while(numChildren > 0) {
        joinableCV.wait(lk, [&] {return !this->joinable.empty();});
        while(!joinable.empty()) {
            printf("join child\n");
            children[joinable.front()]->join();
            printf("erase child\n");
            children.erase(joinable.front());
            printf("pop front of joinable\n");
            joinable.pop();
            --numChildren;
        }
    }
}   
