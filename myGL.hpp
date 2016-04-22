// Richard Kirchofer

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>

#include <boost/filesystem.hpp>
#include <png.h>

//#include <fstream>
//#include <sstream>
//#include <iostream>

#include "draw.hpp"


//#include "shape.hpp"
//#include "context.hpp"
//#include "input.hpp"

#ifndef MYGL_HPP
#define MYGL_HPP

// data declarations
struct arguments {
    enum {EMPTY, MOUSE_BUTTON, CURSOR_POSITION, KEY_CALLBACK} argumentType;
    union {
        struct {
            int mButton;
            int mAction;
            int mMods;
        };
        struct {
            double x;
            double y;
        };
        struct {
            int kKey;
            int kScancode;
            int kAction;
            int kMods;
        };
    };
};
struct shaderUniforms {
    GLint viewOffsetX;
    GLint viewOffsetY;
    GLint unitsPerPixelX;
    GLint unitsPerPixelY;
    GLint screenWidth;
    GLint screenHeight;
};

// enumerations
enum inputEvent {null, mouseLeftPress, mouseLeftHold, mouseLeftRelease, mouseRightPress, mouseRightHold, mouseRightRelease, mouseMiddlePress, mouseMiddleHold, mouseMiddleRelease, cursorMovement, cursorEnter, cursorLeave, windowMovement, windowResize, keySpacePress, keySpaceHold, keySpaceRelease, keySlashPress, keySlashHold, keySlashRelease, scrollLeft, scrollRight, mouseLeftDrag};
enum keyboardLayout {US, DVORAK};
enum functionHandleEnums {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, scrollUp, scrollDown};
enum contextEnums {DRAW};

// function declarations
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_resize_callback(GLFWwindow* window, int width, int height);
void window_move_callback(GLFWwindow* window, int x, int y);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drop_callback(GLFWwindow* window, int count, const char** paths);

void saveBufferAsBytes();
void init(int argc, char** argv);

GLuint createShader(char* vertexShaderFileName, char* fragmentShaderFileName);
void checkShaderStepSuccess(GLint program, GLuint status);
void printShaderLog(char* errorMessage, GLuint shader);

void manualMapKeys();
void zoomIn();
void zoomOut();

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
    static unsigned char* readPNG(char* imageName);
    static void writePNG(char imageName[], unsigned char* data, int width, int height);
    void read();
    void write();
    int width();
    int height();
    unsigned char* pixels();
    png_image image;
    char* imageName;
    unsigned char* data;
};

class ShaderProgram {
};
/**
 * The member variables of CursorMovement, MouseButton, and Key match what is provided in the respective GLFW callback functions.
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

/**
 * The context has a view. The view has member variables to allow for mapping the 'real' world coordinate space to screen coordinates. Screen coordinates are such that only points in the range of [-1, 1] are visible. Not sure about the inclusion actually.
 */
class View {
public:
    View();
    void cursorMovement(CursorMovement);
    void pan();
    void endPan();
    bool panning;
    bool reversePan;
    double mouseHiddenAtX;
    double mouseHiddenAtY;
};

/**
 * The Shape class allows a very nice vector<Shape*> in Context. The common stuff for rendering goes here.
 */
class Shape {
public:
    Shape();
    // finish will create the final buffers
    virtual void cursorMovement(CursorMovement)=0;
    virtual int dataLength()=0;
    virtual void prepareTheData()=0; /** e.g. allows a rectangle to fill 'vector<float> data' from startXYendXY before the buffers are made */
    void finish(); /** prep the data, gen the buffers, and make 'renderPtr' point to 'finalRender' called before pushing the current shape onto context's vector<Shape*> */
    void frameRender(); /** gens buffers and cleans them up afterwards, leaves nothing bound, used for shapes that are in progress */
    void finalRender(); /** bind, draw, and unbind; assumes that the correct buffers are already generated */
    void genAndBindBufferAndVao(); /** gens, binds, and leaves them bound **/
    void unbindBufferAndVao(); /** dissables vertex attrib arrays and unbinds vbo and vao */
    void bindBufferAndVao(); /** binds vao and vbo and enables attrib arrays */
    void render(); /** Context's render renders all shapes in vector<Shape*> and Shape::render calls whatever renderPtr points to */
    typedef void (Shape::*renderFunc)(void);
    renderFunc renderPtr; /** points to either frameRender or finalRender */
    bool finished;
    GLuint vbo;
    GLuint vao;
    GLenum primitiveType;
    GLenum bufferUsage;
    std::vector<float> data;
};

class Line : public Shape {
public:
    Line(float x, float y);
    virtual void prepareTheData() override;
    ~Line();
    void cursorMovement(CursorMovement) override;
    int dataLength() override;
};

class Rectangle : public Shape {
public:
    Rectangle(float x, float y);
    Rectangle(float ax, float ay, float bx, float by);
    void cursorMovement(CursorMovement) override;
    void prepareTheData() override;
    int dataLength() override;
    float startX, startY, endX, endY;
    int lengthOfData = 12;
};

class Context {
public:
    virtual void cursorMovement(CursorMovement)=0;
    virtual void key(Key)=0;
    // public, visible to user
    void mouseButton(MouseButton); /** updates 'mousePressed' before calling mb() */
    // protected, hidden to user
    virtual void mb(MouseButton)=0;
    // not sure if all contexts will have a render function, use dynamic cast in draw.cpp
    virtual void render()=0;
    Context* currentContext;
    // lastKeyPressed is used to pick the new that is to be created
    int lastScancode;
    bool mousePressed;
};

class Composer : public Context {
public:
    Composer();
    void cursorMovement(CursorMovement) override;
    void mb(MouseButton) override;
    void key(Key) override;
    void render() override;
    View* view;
    Shape* currentShape;
    std::vector<Shape*> shapes;
};

extern FILE* stdLog;
extern Context* currentContext;
extern struct shaderUniforms shaderUniforms;
extern keyboardLayout keyboardL;
extern GLFWwindow* window;
extern int screenWidth, screenHeight;
extern double tempViewOffsetX;
extern double tempViewOffsetY;
extern double viewOffsetRealX;
extern double viewOffsetRealY;
extern double unitsPerPixelX;
extern double unitsPerPixelY;

// maybe should be in view?
inline double pixelToRealX(double px) {
    return (px - (screenWidth / 2.0)) * unitsPerPixelX - viewOffsetRealX;
}

inline double pixelToRealY(double py) {
    return (py - (screenHeight / 2.0)) * unitsPerPixelY - viewOffsetRealY;
}

#endif
