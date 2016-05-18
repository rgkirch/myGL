#include <vector>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <list>
#include <algorithm>

#define size 100000

using namespace std;

int one() {
    return 1;
}

int sum(int& left, future<int>& right) {
    return left + right.get();
}

int main() {
    vector<future<int>> vec;
    int arr[size] = {0};
    for(int i = 0; i < size; ++i) {
        //vec.push_back(0);
        vec.push_back(async(one));
        //children.push_back(make_unique<thread>([&]{one(vec[vec.size()-1]);}));
    }
    cout << std::accumulate(vec.begin(), vec.end(), 0, sum) << endl;
}
