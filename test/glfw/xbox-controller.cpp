#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <iomanip>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[])
{
    if(!glfwInit())
    {
        throw std::runtime_error("GLFW failed to init.");
    }
    std::cout << glfwGetVersionString() << std::endl;
    while(1)
    {
        std::cout << std::string(30, '\n') << std::endl;
        for(int j = GLFW_JOYSTICK_1; j < GLFW_JOYSTICK_LAST; ++j)
        {
            if(glfwJoystickPresent(j))
            {
                std::cout << "joystick " << j << std::endl;
                int count = 0;
                const float* axes = glfwGetJoystickAxes(j, &count);
                //std::cout << std::setw(6) << std::setprecision(2) << count << " axes" << std::endl;
                std::cout << std::setw(6) << std::setprecision(2) << axes[0] << " left thumbstick X" << std::endl;
                std::cout << std::setw(6) << std::setprecision(2) << axes[1] << " left thumbstick Y" << std::endl;
                std::cout << std::setw(6) << std::setprecision(2) << axes[2] << " left trigger" << std::endl;
                std::cout << std::setw(6) << std::setprecision(2) << axes[3] << " right thumbstick X" << std::endl;
                std::cout << std::setw(6) << std::setprecision(2) << axes[4] << " right thumbstick Y" << std::endl;
                std::cout << std::setw(6) << std::setprecision(2) << axes[5] << " right trigger" << std::endl;
                std::cout << std::setw(6) << std::setprecision(2) << axes[6] << " directional pad X" << std::endl;
                const unsigned char* buttons = glfwGetJoystickButtons(j, &count);
                //std::cout << std::setw(6) << std::setprecision(2) << count << " buttons" << std::endl;
                std::cout << (GLFW_PRESS == buttons[0]) << " A" << std::endl;
                std::cout << (GLFW_PRESS == buttons[1]) << " B" << std::endl;
                std::cout << (GLFW_PRESS == buttons[2]) << " X" << std::endl;
                std::cout << (GLFW_PRESS == buttons[3]) << " Y" << std::endl;
                std::cout << (GLFW_PRESS == buttons[4]) << " left bumper" << std::endl;
                std::cout << (GLFW_PRESS == buttons[5]) << " right bumper" << std::endl;
                std::cout << (GLFW_PRESS == buttons[6]) << " back button" << std::endl;
                std::cout << (GLFW_PRESS == buttons[7]) << " start button" << std::endl;
                std::cout << (GLFW_PRESS == buttons[8]) << " big X" << std::endl;
                std::cout << (GLFW_PRESS == buttons[9]) << " left stick" << std::endl;
                std::cout << (GLFW_PRESS == buttons[10]) << " right stick" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    glfwMakeContextCurrent(NULL);
    return 0;
}
