                                // plot.cpp
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

//#include <fstream>
//#include <sstream>
//#include <iostream>

#include "plot.hpp"

using namespace std;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_resize_callback(GLFWwindow* window, int width, int height);
struct mouseState {
    double pressX;
    double pressY;
    double releaseX;
    double releaseY;
    int button;
    int action;
    int mods;
    vector<float> trail;
};

void init();

GLuint createShader(char* vertexShaderFileName, char* fragmentShaderFileName);
void checkShaderStepSuccess(GLint program, GLuint status);
void printShaderLog(char* errorMessage, GLuint shader);
struct shaderUniforms {
    GLint cameraOffsetXShaderUniformLocation;
    GLint cameraOffsetYShaderUniformLocation;
    GLint cameraScaleXShaderUniformLocation;
    GLint cameraScaleYShaderUniformLocation;
    GLint screenWidth;
    GLint screenHeight;
};

void addLinesFromMouseState();

struct shaderUniforms shaderUniforms;
struct mouseState mouse;


const char* vertexShaderFileName = "vertexShader.glsl";
const char* fragmentShaderFileName = "fragmentShader.glsl";
int screenWidth = 700, screenHeight = 700;
float xmax, ymax, xmin, ymin;
GLFWwindow* window;
char windowTitle[] = "plot";
//vector<float*> lines;
vector<int> pointLengths;
vector<GLuint> vbos;
vector<GLuint> vaos;
vector<GLuint>primitiveType;

//camera - coord of top left of screen
double cameraOffsetX = 0.0;
double cameraOffsetY = 0.0;
double cameraScaleX = 1.0;
double cameraScaleY = 1.0;
double physicalToRealX(double x);
double physicalToRealY(double y);

float scrollSpeedMultiplier = 0.1;

void findMinMax(float &min, float &max, int length, float* nums) {
    for( int i=0; i<length; ++i ) {
        if( max < nums[i] ) {
            max = nums[i];
        } else if( min > nums[i] ) {
            min = nums[i];
        }
    }
}

void testCursorPolling() {
    static double lastTime = 0.0;
    double thisTime;
    double x, y;
    if((thisTime = glfwGetTime()) - lastTime > 1) {
        glfwGetCursorPos(window, &x, &y);
        printf("%f, %f\n", x, y);
        lastTime = thisTime;
    }
}

// sizeof(GL_FLOAT)?
void addPoint(float x, float y) {
    float* point = (float*)malloc(sizeof(float) * 2);
    point[0] = x;
    point[1] = y;
    pointLengths.push_back(1);
    primitiveType.push_back(GL_POINTS);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    vbos.push_back(vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2, point, GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float), &nums[0]);
    //glBufferSubData(GL_ARRAY_BUFFER, sizeof(float), sizeof(float), &nums[0]);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}
// add point will maintain the max and min and make buffers for the data
void addPoint(int length, float* nums) {
    pointLengths.push_back(length / 2);
    primitiveType.push_back(GL_POINTS);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    vbos.push_back(vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * length, nums, GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float), &nums[0]);
    //glBufferSubData(GL_ARRAY_BUFFER, sizeof(float), sizeof(float), &nums[0]);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void draw() {
    GLuint shaderProgram = createShader((char*)vertexShaderFileName, (char*)fragmentShaderFileName);
    glUseProgram(shaderProgram);
    shaderUniforms.cameraOffsetXShaderUniformLocation = glGetUniformLocation(shaderProgram, "cameraOffsetX");
    shaderUniforms.cameraOffsetYShaderUniformLocation = glGetUniformLocation(shaderProgram, "cameraOffsetY");
    shaderUniforms.cameraScaleXShaderUniformLocation = glGetUniformLocation(shaderProgram, "cameraScaleX");
    shaderUniforms.cameraScaleYShaderUniformLocation = glGetUniformLocation(shaderProgram, "cameraScaleY");
    shaderUniforms.screenWidth= glGetUniformLocation(shaderProgram, "screenWidth");
    shaderUniforms.screenHeight= glGetUniformLocation(shaderProgram, "screenHeight");

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glPointSize(10);

	while( !glfwWindowShouldClose( window ) ) {
		glfwPollEvents();
		glClear( GL_COLOR_BUFFER_BIT );
        glUniform1f(shaderUniforms.cameraOffsetXShaderUniformLocation, cameraOffsetX);
        glUniform1f(shaderUniforms.cameraOffsetYShaderUniformLocation, cameraOffsetY);
        glUniform1f(shaderUniforms.cameraScaleXShaderUniformLocation, cameraScaleX);
        glUniform1f(shaderUniforms.cameraScaleYShaderUniformLocation, cameraScaleY);
        glUniform1f(shaderUniforms.screenWidth, screenWidth);
        glUniform1f(shaderUniforms.screenHeight, screenHeight);
        for(int i = 0; i < pointLengths.size(); ++i) {
            glBindVertexArray(vaos[i]);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
            // TODO - maybe add -1
            glDrawArrays(primitiveType[i], 0, pointLengths[i]);
        }

        // draw the in progress line
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mouse.trail.size(), &mouse.trail[0], GL_STREAM_DRAW);
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_LINE_STRIP, 0, mouse.trail.size() / 2);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        // draw the in progress line

		glfwSwapBuffers( window );
        //testCursorPolling();
	}

	glfwDestroyWindow( window );
	glfwTerminate();
}

// allocates memory for (shader) file contents
GLchar* readFile(char* fileName) {
    FILE* file = fopen(fileName, "rb");
    if( ! file ) fprintf(stderr, "error: Could not open file %s\n", fileName);
    long fileLength = 0;
    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    rewind(file);
    GLchar* contents = (GLchar*)malloc(fileLength + 1);
    if( ! contents ) fprintf(stderr, "error: Could not allocate memory.\n");
    contents[fileLength] = '\0';
    fread(contents, 1, fileLength, file);
    fclose(file);
    return contents;
}

GLuint createShader(char* vertexShaderFileName, char* fragmentShaderFileName) {
    if( ! vertexShaderFileName ) fprintf(stderr, "error: NULL passed in for vertex shader file name.\n");
    if( ! fragmentShaderFileName ) fprintf(stderr, "error: NULL passed in for fragment shader file name.\n");
    char* sourceFileName[] = { vertexShaderFileName, fragmentShaderFileName };
    GLint shaderEnums[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    GLuint program = glCreateProgram();
    if( ! program ) fprintf(stderr, "error: failed to create program\n");
    for(int i = 0; i < 2; ++i) {
        GLchar* shaderCode = readFile( sourceFileName[i] );
        GLuint shader = glCreateShader( shaderEnums[i] );
        glShaderSource( shader, 1, (const GLchar**)&shaderCode, NULL );
        glCompileShader(shader);
        checkShaderStepSuccess(shader, GL_COMPILE_STATUS);
        glAttachShader(program, shader);
        free( shaderCode );
        //delete [] shaderCode;
        glDeleteShader(shader);
    }
    glLinkProgram(program);
    checkShaderStepSuccess(program, GL_LINK_STATUS);
    return program;
}

void checkShaderStepSuccess(GLint program, GLuint status) {
    GLint success = -3;
    switch(status) {
        case GL_COMPILE_STATUS:
            glGetShaderiv( program, status, &success );
            if( success == -3 ) fprintf(stderr, "error: the success check may have a false positive\n");
            if( ! success ) {
                printShaderLog((char*)"error: gl shader program failed to compile.", program);
                fprintf(stderr, "Exiting.\n");
                exit(1);
            }
            break;
        case GL_LINK_STATUS:
            glGetProgramiv( program, status, &success );
            printf("success value %d\n", success);
            if( success == -3 ) fprintf(stderr, "error: the success check may have a false positive\n");
            if( ! success ) {
                printShaderLog((char*)"error: gl shader program failed to link.", program);
                //fprintf(stderr, "Exiting.\n");
                //exit(1);
            }
            break;
        default:
            fprintf(stderr, "function called with unhandled case.\n");
            break;
    }
}

void printShaderLog(char* errorMessage, GLuint shader) {
    fprintf(stderr, "%s\n", errorMessage);
    GLint length = 0;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );
    printf("log length %d\n", length);
    GLchar* logText = (GLchar*)malloc(sizeof(GLchar) * (length + 1));
    logText[length] = '\0';
    glGetShaderInfoLog(shader, length, &length, logText);
    printf("printing log\n");
    fprintf(stderr, "%s", logText);
    //cout << logText << endl;
    free(logText);
}

void init() {
	/* Initialize the library */
	if ( !glfwInit() ) {
        fprintf(stderr, "GLFW failed to init.\n");
        fprintf(stderr, "Exiting.\n");
        exit(1);
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow( screenWidth, screenHeight, windowTitle, NULL, NULL );
	if ( !window ) {
		glfwTerminate();
        fprintf(stderr, "GLFW failed to create the window.\n");
        fprintf(stderr, "Exiting.\n");
        exit(1);
	}
	glfwMakeContextCurrent( window );
    //glfwSetWindowSizeCallback(window, windowSize);

	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK ) {
        fprintf(stderr, "GLEW failed to init.\n");
        fprintf(stderr, "Exiting.\n");
        exit(1);
	}

	glViewport( 0, 0, screenWidth, screenHeight );

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window,  window_resize_callback);

    //printf( "VENDOR = %s\n", glGetString( GL_VENDOR ) ) ;
    //printf( "RENDERER = %s\n", glGetString( GL_RENDERER ) ) ;
    //printf( "VERSION = %s\n", glGetString( GL_VERSION ) ) ;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if(mouse.action == GLFW_PRESS) {
        mouse.trail.push_back(physicalToRealX(xpos));
        mouse.trail.push_back(physicalToRealY(ypos));
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    //printf("mouse button callback\n\tbutton %d\n\taction %d\n\tmods %d\n", button, action, mods);
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS) {
            glfwGetCursorPos(window, &mouse.pressX, &mouse.pressY);
            mouse.action = GLFW_PRESS;
        }
        if(action == GLFW_RELEASE) {
            glfwGetCursorPos(window, &mouse.releaseX, &mouse.releaseY);
            mouse.action = GLFW_RELEASE;
            if(mouse.pressX == mouse.releaseX && mouse.pressY == mouse.releaseY) {
                addPoint(physicalToRealX(mouse.pressX), physicalToRealY(mouse.pressY));
            } else {
                addLinesFromMouseState();
            }
            mouse.trail.clear();
        }
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    double x, y;
    //printf("scroll x: %f y: %f\n", xoffset, yoffset);
    cameraScaleX *= 1.0 + (yoffset * scrollSpeedMultiplier);
    cameraScaleY *= 1.0 + (yoffset * scrollSpeedMultiplier);
    glfwGetCursorPos(window, &x, &y);
    if(mouse.action == GLFW_PRESS) {
        mouse.trail.push_back(physicalToRealX(x));
        mouse.trail.push_back(physicalToRealY(y));
    }
}

double physicalToRealX(double px) {
        return (px - (screenWidth / 2.0)) / cameraScaleX;
}
double physicalToRealY(double py) {
        return ((screenHeight - py) - (screenHeight / 2.0)) / cameraScaleY;
}

void addLinesFromMouseState() {
    pointLengths.push_back(mouse.trail.size() / 2);
    primitiveType.push_back(GL_LINE_STRIP);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    vbos.push_back(vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // TODO - include mouse.press and mouse.release in the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mouse.trail.size(), &mouse.trail[0], GL_STATIC_DRAW);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //duplicate clear...
    mouse.trail.clear();
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
    printf("%d, %d\n", width, height);
    screenWidth = width;
    screenHeight = height;
	glViewport( 0, 0, screenWidth, screenHeight );
}

