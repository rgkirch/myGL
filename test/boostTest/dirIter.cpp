#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    int count = 0;
    boost::filesystem::recursive_directory_iterator dirIter(argv[1]);
    while(dirIter != boost::filesystem::recursive_directory_iterator())
    {
        try
        {
            dirIter++;
        } catch (boost::filesystem::filesystem_error& e) {
        }
        count++;
    }
    std::cout << count << std::endl;
    return 0;
}
