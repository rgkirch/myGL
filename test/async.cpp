#include <iostream>
#include <future>
#include <chrono>
#include <vector>

#define size 10

// a non-optimized way of checking for prime numbers:
bool is_prime (int x) {
  //std::cout << std::this_thread::get_id() << " Calculating " << x << std::endl;
  for (int i=2; i<x; ++i) if (x%i==0) return false;
  return true;
}

int main () {
    std::chrono::steady_clock::time_point start, end;
  
    if(0) {
        std::vector<std::future<bool>> vec;
        std::cout << "automatic" << std::endl;
        start = std::chrono::steady_clock::now();
        for(int i = 0; i < size; ++i) {
            vec.push_back(std::async(std::launch::async | std::launch::deferred, is_prime, 313222313));
        }
        for(auto& x : vec) {
            //std::cout << std::this_thread::get_id() << " " << (x.get() ? "prime" : "not") << std::endl;
            x.get();
        }
        end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    } 
  
    {
        std::vector<std::future<bool>> vec;
        std::cout << "async" << std::endl;
        start = std::chrono::steady_clock::now();
        for(int i = 0; i < size; ++i) {
            vec.push_back(std::async(std::launch::async, is_prime, 313222313));
        }
        for(auto& x : vec) {
            //std::cout << std::this_thread::get_id() << " " << (x.get() ? "prime" : "not") << std::endl;
            x.get();
        }
        end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    }
  
    {
        std::vector<std::unique_ptr<std::thread>> vec;
        std::cout << "threading" << std::endl;
        start = std::chrono::steady_clock::now();
        for(int i = 0; i < size; ++i) {
            vec.push_back(std::make_unique<std::thread>(std::bind(is_prime, 313222313)));
        }
        for(auto& x : vec) {
            x->join();
        }
        end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    }
  
    return 0;
}
