    #include <vector>
    #include <iostream>
    #include <memory>
    #include <thread>
    #include <list>
    #include <algorithm>
    
    using namespace std;
    
    void one(int& i) {
        i = 1;
        cout << "one\n";
    }
    
    int main() {
        list<unique_ptr<thread>> children;
        vector<int> vec;
        for(int i = 0; i < 3; ++i) {
            vec.push_back(0);
            children.push_back(make_unique<thread>(bind(one, vec[vec.size()-1])));
            //children.push_back(make_unique<thread>([&]{one(vec[vec.size()-1]);}));
        }
        while(!children.empty()) {
            children.front()->join();
            children.pop_front();
        }
        cout << accumulate(vec.begin(), vec.end(), 0) << endl;
    }
