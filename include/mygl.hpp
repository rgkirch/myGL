// Richard Kirchofer

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
#include <iomanip>
#include <iostream>
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
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <Magick++.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

//#define STB_DEFINE
//#include "stb/stb.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb/stb_image.h"

#include "shader.hpp"
//#include <fstream>
//#include <sstream>
//#include <iostream>

#ifndef MYGL_HPP
#define MYGL_HPP

extern std::mutex contextMutex;

class Window;
class MyGL;

void printMonitorInfo();

namespace glfwInputCallback
{
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void window_resize_callback(GLFWwindow *window, int width, int height);
    void window_move_callback(GLFWwindow *window, int x, int y);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void drop_callback(GLFWwindow *window, int count, const char** paths);
}

struct WindowHints
{
    WindowHints();
    WindowHints(const WindowHints& wh);
	unsigned int glfw_context_version_major;
	unsigned int glfw_context_version_minor;
	unsigned int glfw_opengl_profile;
	unsigned int glfw_resizable;
    unsigned int glfw_opengl_forward_compat;
    unsigned int glfw_focused;
    unsigned int glfw_decorated;
    unsigned int glfw_visible;
    glm::vec3 clearColor;
    glm::vec2 location;
    int width;
    int height;
};

/**
  The member variables of CursorMovement, MouseButton, and Key match what is provided in the respective GLFW callback functions.
 */
class Input
{
};

class CursorMovement : public Input
{
public:
    CursorMovement(double xpos, double ypos) :x{xpos}, y{ypos} {}
    double x;
    double y;
};

class MouseButton : public Input
{
public:
    MouseButton(int button, int action, int mods) : button{button}, action{action}, mods{mods} {}
    int button;
    int action;
    int mods;
};

class Key : public Input
{
public:
    Key(int key, int scancode, int action, int mods) : key{key}, scancode{scancode}, action{action}, mods{mods} {}
    int key;
    int scancode;
    int action;
    int mods;
};

/** A window has a view object.
 *  The View is responsible for maintaining the variables that the vertexShader uses to translate the points from real space to apparent space. Real space refers to the reference by which all of the shape objects in the Composer context are saved. It stretches from -inf to +inf. Apparent space is the (-1, 1) clamped view recognised by opengl.*/
class View
{
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
class Shape
{
public:
    typedef void (Shape::*renderFunc)(void);
    Shape(float x, float y);
    Shape(float x, float y, float ex, float ey);
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

class Context
{
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
class Window
{
public:
    typedef void (Window::*threadFunc)(void);
    Window(MyGL *parent, const WindowHints& wh);
    ~Window();
    void loop(); /** updates the window*/
    void hide();
    void show();
    void close();
    void moveAbsolute(int x, int y);
    void moveRelative(int x, int y);
    GLFWwindow *window; /** The window class needs to know which GLFWwindow it is taking care of. 1 Window for 1 GLFWwindow*/
    int width;
    int height;
    float clearColorRed;
    float clearColorGreen;
    float clearColorBlue;
    std::unique_ptr<std::thread> t;
    View *currentView;
    std::vector<View*> views;
    MyGL *parentMyGL;
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
};

struct ImageIterator
{
    ImageIterator(std::string);
    Magick::Image operator()();
    boost::filesystem::recursive_directory_iterator dirIter;
    std::string directory;
};

struct Square
{
    Square();
    ~Square();
    void operator()();
    GLuint vao;
    GLuint vbo;
    //float bufferData[] = {-1.0, 1.0, -1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    float bufferData[12] = {-1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0};
    float uvData[12] = {0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    //float bufferData[] = {0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0};
};

/** This is the topmost class for the program. Creating a new instance of this class means creating a new instance of the program.
 *  The main program should be as simple as running
 *  MyGL* mygl = new MyGL();
 *  The main.cpp main can then interact with the program in different ways through mygl.
 *  mygl->getCurrentContext();
 *  mygl->switchCurrentContext(Context);
 *  */
class MyGL
{
public:
    MyGL();
    MyGL(std::string);
    ~MyGL();
    Magick::Image grabNextImage(boost::filesystem::recursive_directory_iterator& dirIter);
    void collage(std::string);
    void cubeCollage(std::string);
    void playVideo(std::string filename);
    void end();
    void genLotsWindows();
    GLFWwindow* makeWindowForContext();

    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void registerUserCursorPositionCallbackFunction(std::function<void(const double, const double)>);

    GLFWwindow* windowForContext;
    std::function<void(const Key&)> inputFunction;
    Context *currentContext;
    ShaderProgram *currentShaderProgram;
    std::list<Window*> windows;
    //std::list<std::unique_ptr<Window>> windows;
    std::vector<std::unique_ptr<Context>> contexts;
    std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;
    std::string vertexShaderFileName {"vertexShader.glsl"};
    std::string fragmentShaderFileName {"fragmentShader.glsl"};
private:
    std::vector<std::function<void(const double, const double)>> userCursorPositionCallbackFunctions;
};

namespace SnakeGame
{
    void snakeGame(MyGL *application);
    int newFoodLocation(int snakeSize, int gridSize, std::unordered_map<int, std::unique_ptr<Window>>& grid);
    void stepNorth();
    void stepEast();
    void stepSouth();
    void stepWest();
}

struct FPScounter
{
    FPScounter();
    double getfps();
    std::chrono::steady_clock::time_point tp;
};

class Joystick
{
public:
    std::tuple<double, double> getLeftThumbStickValues(int joystick);
    std::tuple<double, double> getRightThumbStickValues(int joystick);
private:
    const float* axes;
    const unsigned char* buttons;
};

class Image
{
public:
    Image();
    Image(Image&& image);
    ~Image();
    void read(const std::string& filename);
    void write(std::string filename);
    unsigned char* data;
    int width;
    int height;
    int numberOfChannels;
    int depth;
};

void boostFun(std::string);

#endif
