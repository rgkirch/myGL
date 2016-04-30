#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>

class Item {
public:
    explicit Item() {x = new char[10](); std::strcpy(x, "hello");}
    explicit Item(std::string str) {x = new char[100](); std::strcpy(x, str.c_str());}
    ~Item() {std::cout << "destructor" << std::endl;}
    char *x;
};

int main() {
    std::list< std::unique_ptr<Item> > items;
    items.push_back(std::unique_ptr<Item>(new Item()));
    items.push_back(std::unique_ptr<Item>(new Item(std::string("there"))));
    for(const auto& i : items) {
        std::cout << i->x;
    }
    std::cout << std::endl;
    return 0;
}
