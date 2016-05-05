// Richard Kirchofer

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <math.h>
#include <mutex>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <png.h>

//#include <fstream>
//#include <sstream>
//#include <iostream>

//#include "shape.hpp"
//#include "context.hpp"
//#include "input.hpp"

#ifndef MYGL_HPP
#define MYGL_HPP

extern std::mutex contextMutex;

class Window;
class MyGL;

namespace glfwInputCallback {
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void window_resize_callback(GLFWwindow *window, int width, int height);
    void window_move_callback(GLFWwindow *window, int x, int y);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void drop_callback(GLFWwindow *window, int count, const char** paths);
}

/**
 * The PNG class is for reading and writing '.png' files. There are two static functions called readPNG() and writePNG() that can be used without instantiating an instance of the class.
 * This class was made to solve the problem that
 *      writePNG(readPNG(fileName));
 * writePNG in the above statement doesn't know the width and height of the image. Now the width and height are stored in the object itself.
 */
class PNG {
public:
    PNG();
    PNG(char imageName[]);
    static unsigned char *readPNG(char *imageName);
    static void writePNG(char imageName[], unsigned char *data, int width, int height);
    void read();
    void write();
    int width();
    int height();
    unsigned char* pixels();
    png_image image;
    char* imageName;
    unsigned char *data;
};

class ShaderProgram {
public:
    ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
    static std::string readFile(std::string fileName);
    void checkShaderStepSuccess(GLuint shader, GLuint status);
    void printShaderLog(char *errorMessage, GLuint shader);
    GLuint id();
    GLuint program;
    GLint viewOffsetX;
    GLint viewOffsetY;
    GLint unitsPerPixelX;
    GLint unitsPerPixelY;
    GLint screenWidth;
    GLint screenHeight;
};

class ShaderTextured : ShaderProgram {
public:
};

/**
  The member variables of CursorMovement, MouseButton, and Key match what is provided in the respective GLFW callback functions.
 */
class Input {
};

class CursorMovement : public Input {
public:
    CursorMovement(double xpos, double ypos) :x{xpos}, y{ypos} {}
    double x;
    double y;
};

class MouseButton : public Input {
public:
    MouseButton(int button, int action, int mods) : button{button}, action{action}, mods{mods} {}
    int button;
    int action;
    int mods;
};

class Key : public Input {
public:
    Key(int key, int scancode, int action, int mods) : key{key}, scancode{scancode}, action{action}, mods{mods} {}
    int key;
    int scancode;
    int action;
    int mods;
};

/** A window has a view object.
 *  The View is responsible for maintaining the variables that the vertexShader uses to translate the points from real space to apparent space. Real space refers to the reference by which all of the shape objects in the Composer context are saved. It stretches from -inf to +inf. Apparent space is the (-1, 1) clamped view recognised by opengl.*/
class View {
public:
    View(Window *window, int width, int height);
    void translate(float x, float y);
    void scale(float num);
    double pixelToRealX(double px);
    double pixelToRealY(double py);
    Window *parentWindow;
    // change width and height to start and end coord, width and height doesn't give position
    int width;
    int height;
    double viewOffsetX = 0.0;
    double viewOffsetY = 0.0;
    double unitsPerPixelX = 1.0;
    double unitsPerPixelY = 1.0;
};


/**
 * The Shape class allows a very nice vector<Shape*> in Context. The common stuff for rendering goes here.
 */
class Shape {
public:
    typedef void (Shape::*renderFunc)(void);
    Shape(float x, float y);
    int dataLength();
    void finish(); /** prep the data, gen the buffers, and make 'renderPtr' point to 'finalRender' called before pushing the current shape onto context's vector<Shape*> */
    void render(); /** Context's render renders all shapes in vector<Shape*> and Shape::render calls whatever renderPtr points to */
private:
    void frameRender(); /** gens buffers and cleans them up afterwards, leaves nothing bound, used for shapes that are in progress */
    void finalRender(); /** bind, draw, and unbind; assumes that the correct buffers are already generated */
    renderFunc renderPtr; /** points to either frameRender or finalRender */
    GLuint vbo;
    GLuint vao;
    ShaderProgram *shader;
    std::vector<float> data;
    float startX, startY, endX, endY;
    int lengthOfData = 12;
};

class Context {
public:
    Context();
    void render(ShaderProgram *shader, View *view); /** Calls the render function of all of the Shape objects.*/
    Shape *currentShape;
    std::vector<Shape*> shapes;
};

/** There may be multiple windows in total. They are all kept track of in the MyGL object.
 *  The Window class has a function that matches each of the different available glfw callbacks. These functions are registered with glfw in the Window constructor. The callback register function accepts a pointer to a GLFWwindow so I think that these callbacks are window specific. If they weren't window specific then all of the callback functions could be up a level in mygl.
 *  The window knows the width and heigth of the screen area in pixels so it is responsible for the pixelToReal(X/Y) functions. It is also responsible for the getCursorPos() function.
 *  The window should be responsible for hiding and disabling the mouse cursor. This needs to happen during a pan of the view for example.*/
class Window {
public:
    typedef void (Window::*threadFunc)(void);
    Window(MyGL *parent, int width, int height);
    ~Window();
    bool handles(GLFWwindow *window);
    void loop();
    GLFWwindow *window; /** The window class needs to know which GLFWwindow it is taking care of. 1 Window for 1 GLFWwindow*/
    int width;
    int height;
    std::thread *t;
    View *currentView;
    std::vector<View*> views;
    MyGL *parentMyGL;
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
};

/** This is the topmost class for the program. Creating a new instance of this class means creating a new instance of the program.
 *  The main program should be as simple as running
 *  MyGL* mygl = new MyGL();
 *  The main.cpp main can then interact with the program in different ways through mygl.
 *  mygl->getCurrentContext();
 *  mygl->switchCurrentContext(Context);
 *  */
class MyGL {
public:
    MyGL();
    ~MyGL();
    void snakeGame();
    void mainLoop(); /** Calls on all the windows to update themselvs.*/
    void genLotsWindows();
    GLFWwindow* makeWindowForContext();
    GLFWwindow* windowForContext;
    Context *currentContext;
    ShaderProgram *currentShaderProgram;
    std::list<Window*> windows;
    //std::list<std::unique_ptr<Window>> windows;
    std::vector<std::unique_ptr<Context>> contexts;
    std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;
    std::string vertexShaderFileName {"vertexShader.glsl"};
    std::string fragmentShaderFileName {"fragmentShader.glsl"};
};

#endif
