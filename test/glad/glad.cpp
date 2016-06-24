// glad, include glad *before* glfw
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
    // Init GLFW
    glfwInit();
    // ... <snip> ... setup a window and a context
 
    // Load all OpenGL functions using the glfw loader function
    // If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    // Alternative use the builtin loader, e.g. if no other loader function is available
    /*
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    */
  
    // glad populates global constants after loading to indicate,
    // if a certain extension/version is available.
    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    if(GLAD_GL_VERSION_3_0) {
        /* We support at least OpenGL version 3 */
    }

    // ... <snip> ... more code
}
