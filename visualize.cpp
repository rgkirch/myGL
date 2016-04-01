                                // visualize.cpp
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include <fstream>
//#include <sstream>
//#include <iostream>

#include "visualize.hpp"

using namespace std;

extern unsigned int screenWidth;
extern unsigned int screenHeight;
const char* vertexShaderFileName = "vertexShader.glsl";
const char* fragmentShaderFileName = "fragmentShader.glsl";
const GLuint WIDTH = 1366, HEIGHT = 768;

void findMinMax(float &min, float &max, int length, float &nums) {
    for( int i=0; i<length; ++i ) {
        if( *max < nums[i] ) {
            *max = nums[i];
        } else if( *min > nums[i] ) {
            *min = nums[i];
        }
    }
}

void plot(int numberOfPoints, float* xs, float* ys) {
    // look at all of the numbers, save value of largest and smallest on x and y
    // call a drawPoint() function in visualize.cpp at correct location in range of [-1, 1]
    // visualize knows window resolution, visualize will draw dot
    float xmax, ymax, xmin, ymin;
    xmax = ymax = xmin = ymin = 0;
    findMinMax(xmin, xmax, numberOfPoints, xs);
    GLFWwindow* window;
    init(window, "umm", HEIGHT, HEIGHT);
    GLuint shaderProgram = createShader((char*)vertexShaderFileName, (char*)fragmentShaderFileName);
    glUseProgram(shaderProgram);

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    int numberOfBytes = 2*sizeof(float)*numberOfPoints;
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes * 2, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfBytes, position.x);
    glBufferSubData(GL_ARRAY_BUFFER, numberOfBytes, numberOfBytes, position.y);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 1, GL_INT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, 0, (GLvoid*)numberOfBytes);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


	while( !glfwWindowShouldClose( window ) ) {
		glfwPollEvents();
		glClear( GL_COLOR_BUFFER_BIT );
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfBytes, position.x);
        glBufferSubData(GL_ARRAY_BUFFER, numberOfBytes, numberOfBytes, position.y);
        glDrawArrays(GL_POINTS, 0, numberOfParticles);
        step();

		glfwSwapBuffers( window );
	}

	glfwDestroyWindow( window );
	glfwTerminate();
	return 0;
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

void init(GLFWwindow* &window, char* name, GLuint width, GLuint height) {
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
	window = glfwCreateWindow( width, height, name, NULL, NULL );
	if ( !window ) {
		glfwTerminate();
        fprintf(stderr, "GLFW failed to create the window.\n");
        fprintf(stderr, "Exiting.\n");
        exit(1);
	}
	glfwMakeContextCurrent( window );
    glfwSetWindowSizeCallback(window, windowSize);

	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK ) {
        fprintf(stderr, "GLEW failed to init.\n");
        fprintf(stderr, "Exiting.\n");
        exit(1);
	}

	glViewport( 0, 0, width, height );

    //printf( "VENDOR = %s\n", glGetString( GL_VENDOR ) ) ;
    //printf( "RENDERER = %s\n", glGetString( GL_RENDERER ) ) ;
    //printf( "VERSION = %s\n", glGetString( GL_VERSION ) ) ;
}

void windowSize(GLFWwindow* window, int width, int height) {
    printf("w %d h %d\n", width, height);
    screenWidth = width;
    screenHeight = height;
    //glfwSetWindowSize(window, width, height);
}
