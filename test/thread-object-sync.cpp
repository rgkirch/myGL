#include <iostream>
#include <limits>
#include <string>
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

struct Child {
    Child(Parent *parent);
    ~Child();
    void loop();
    Parent *parent;
};

int main(int argc, char* argv[]) {
    int num = 4;
    if(argc > 1) {
        num = std::stoi(argv[1], nullptr);
    }
    std::unique_ptr<Parent> p(new Parent());
    for(int i = 0; i < num; ++i) {
        p->spawn();
    }
    p->loop();
    return 0;
}

Child::Child(Parent *parent) {
    this->parent = parent;
}

Child::~Child() {
    std::unique_lock<std::mutex> lk(parent->joinableMutex);
    parent->joinable.push(std::this_thread::get_id());
    parent->joinableCV.notify_all();
}

void Child::loop() {
    printf("child sleep\t%lu \n", std::hash<std::thread::id>()(std::this_thread::get_id())%100);
    std::this_thread::sleep_for(std::chrono::seconds(rand()/RAND_MAX));
    printf("child wake\t%lu \n", std::hash<std::thread::id>()(std::this_thread::get_id())%100);
}

void Parent::spawn() {
    std::unique_ptr<std::thread> tptr(new std::thread(
        [&] {
            std::unique_ptr<Child> p(new Child(this));
            p->loop();
        }
    ));
    children.insert(std::make_pair(tptr->get_id(), std::move(tptr)));
    numChildren++;
}

void Parent::loop() {
    while(numChildren > 0) {
        std::unique_lock<std::mutex> lk(joinableMutex);
        joinableCV.wait(lk, [&] {return !this->joinable.empty();});
        while(!joinable.empty()) {
            printf("join child\t%lu\n", std::hash<std::thread::id>()(children[joinable.front()]->get_id())%100);
            children[joinable.front()]->join();
            children.erase(joinable.front());
            joinable.pop();
            --numChildren;
        }
    }
}   
