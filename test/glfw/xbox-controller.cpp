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
    int windowWidth, windowHeight;
    windowWidth = windowHeight = 100;
    if(!glfwInit())
    {
        throw std::runtime_error("GLFW failed to init.");
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_VISIBLE, true);
	glfwWindowHint( GLFW_RESIZABLE, false );
    glfwWindowHint( GLFW_FOCUSED, true);
    glfwWindowHint( GLFW_DECORATED, true );

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "gamepad", NULL, NULL);
	if(!window)
    {
		glfwTerminate();
        throw std::runtime_error("GLFW failed to create the window in Window constructor.");
	}
    glfwMakeContextCurrent(window);
    //glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW failed to init.\n");
        fprintf(stderr, "Exiting.\n");
        throw std::runtime_error("glew failed to init");
    }
	glViewport(0, 0, windowWidth, windowHeight);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glClearColor(1,1,1,1);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwMakeContextCurrent(NULL);
    return 0;
}
