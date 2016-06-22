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
        list.push_back(argument);
        --argument;
    }
}

int main(int argc, char* argv[])
{
    std::list<int> list;
    std::unique_ptr<std::thread> thread = std::make_unique<std::thread>(std::bind(function, list, 7));
    //std::unique_ptr<std::thread> thread = std::make_unique<std::thread>([&] {function(list, 7);});
    thread.get()->join();
    for(auto& x : list)
    {
        std::cout << x << std::endl;
    }
    return 0;
}
