#include <thread>
#include <list>
#include <cassert>
#include <memory>
#include <functional>
#include <iostream>

void function(std::list<int>& list, int argument)
{
    assert(argument > 0);
    while(argument)
    {
        std::cout << "loop" << std::endl;
        list.push_back(argument);
        --argument;
    }
}

void joiner(std::thread* thread)
{
    std::cout << "join start" << std::endl;
    thread->join();
    std::cout << "join end" << std::endl;
}

int main(int argc, char* argv[])
{
    std::list<int> list;
    //std::unique_ptr<std::thread> thread = std::make_unique<std::thread>(std::bind(function, std::ref(list), 7));
    //std::unique_ptr<std::thread> thread(new std::thread(std::bind(function, std::ref(list), 7)));
    std::unique_ptr<std::thread, void (*)(std::thread*)> thread(new std::thread(std::bind(function, std::ref(list), 7)), joiner);
    //std::unique_ptr<std::thread, void (*)(std::thread*)> thread(new std::thread(std::bind(function, std::ref(list), 7)), [](std::thread* thread) -> void {thread->join();});

    //thread.get()->join();
    for(auto& x : list)
    {
        std::cout << x << std::endl;
    }
    return 0;
}
