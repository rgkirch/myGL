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

class Shape {
public:
    Shape();
    // finish will create the final buffers
    virtual void cursorMovement(CursorMovement)=0;
    virtual int dataLength()=0;
    virtual void prepareTheData()=0;
    void finish();
    void frameRender();
    void finalRender();
    void genAndBindBufferAndVao();
    void unbindBufferAndVao();
    void bindBufferAndVao();
    void render();
    typedef void (Shape::*renderFunc)(void);
    renderFunc renderPtr;
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
    void mouseButton(MouseButton);
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

inline double pixelToRealX(double px) {
    return (px - (screenWidth / 2.0)) * unitsPerPixelX - viewOffsetRealX;
}

inline double pixelToRealY(double py) {
    return (py - (screenHeight / 2.0)) * unitsPerPixelY - viewOffsetRealY;
}

#endif
