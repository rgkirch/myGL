#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <list>

std::mutex cout;

class Object {
public:
    Object() {
        std::lock_guard<std::mutex> guard(cout);
        std::cout << "constructor " << std::this_thread::get_id() << std::endl;
    }
    ~Object() {
        std::lock_guard<std::mutex> guard(cout);
        std::cout << "destructor " << std::this_thread::get_id() << std::endl;
    }
    void loop() {
        for(int i = 0; i < 4; ++i) {
            {
                std::lock_guard<std::mutex> guard(cout);
                std::cout << "loop" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

int main() {
    std::list<std::thread*> objectThreads;
    std::unique_lock<std::mutex> guard(cout, std::defer_lock);
    objectThreads.push_back(new std::thread([]{std::unique_ptr<Object> p(new Object()); p->loop();}));
    objectThreads.push_back(new std::thread([]{std::unique_ptr<Object> p(new Object()); p->loop();}));
    for(std::list<std::thread*>::iterator it = objectThreads.begin(), end = objectThreads.end(); it != end;) {
        guard.lock();
        std::cout << "joining " << (*it)->get_id() << std::endl;
        guard.unlock();
        (*it)->join();
        it = objectThreads.erase(it);
    }
    return 0;
}
