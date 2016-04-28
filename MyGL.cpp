#include "MyGL.hpp"

// need to be able to change reverse pan

typedef void (Shape::*renderFunc)();

/** Reads in a couple text files, compiles and links stuff and makes a shader program. Calls glUseProgram at the end and then needs to set up the uniforms.*/
ShaderProgram::ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName) {
    program = glCreateProgram();
    if( ! program ) throw std::runtime_error("failed to create program.");
    std::string vertexShaderCode = readFile(vertexShaderFileName);
    std::string fragmentShaderCode = readFile(fragmentShaderFileName);
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    GLchar* vcode = new char[vertexShaderCode.length()+1];
    GLchar* fcode = new char[fragmentShaderCode.length()+1];
    std::strcpy(vcode, vertexShaderCode.c_str());
    std::strcpy(fcode, fragmentShaderCode.c_str());
    glShaderSource( vertexShader, 1, std::move((const GLchar**)&vcode), NULL );
    glShaderSource( fragmentShader, 1, std::move((const GLchar**)&fcode), NULL );
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    checkShaderStepSuccess(vertexShader, GL_COMPILE_STATUS);
    checkShaderStepSuccess(fragmentShader, GL_COMPILE_STATUS);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glLinkProgram(program);
    checkShaderStepSuccess(program, GL_LINK_STATUS);
    glUseProgram(program);

    delete[] vcode;
    delete[] fcode;
    viewOffsetX = glGetUniformLocation(program, "viewOffsetX");
    viewOffsetY = glGetUniformLocation(program, "viewOffsetY");
    unitsPerPixelX = glGetUniformLocation(program, "unitsPerPixelX");
    unitsPerPixelY = glGetUniformLocation(program, "unitsPerPixelY");
    screenWidth = glGetUniformLocation(program, "screenWidth");
    screenHeight = glGetUniformLocation(program, "screenHeight");
}

GLuint ShaderProgram::id() {
    return program;
}

std::string ShaderProgram::readFile(std::string fileName) {

    std::ifstream stream(fileName);
    std::string data;
    std::ostringstream string;
    string << stream.rdbuf();
    data = string.str();
    return data;
    //std::streamsize size = stream.rdbuf()->pubseekoff(0, stream.end);
    //stream.rdbuf()->pubseekoff(0, stream.beg);
    ////char* data = (char*)malloc(size * sizeof(char));
    //std::stringstream data(stream.rdbuf());
    ////char* data = new char[size];
    ////if( ! data ) throw std::runtime_error("Could not allocate memory.");
    ////stream.rdbuf()->sgetn(data, size);
    //std::string string(std::move(data)); //<-- if I do this?
}

void ShaderProgram::checkShaderStepSuccess(GLuint shader, GLuint status) {
    GLint success = -3;
    switch(status) {
        case GL_COMPILE_STATUS:
            glGetShaderiv( shader, status, &success );
            if( success == -3 ) fprintf(stderr, "error: the success check may have a false positive\n");
            if( ! success ) {
                printShaderLog((char*)"error: gl shader program failed to compile.", shader);
                fprintf(stderr, "Exiting.\n");
                exit(1);
            }
            break;
        case GL_LINK_STATUS:
            glGetProgramiv( shader, status, &success );
            printf("success value %d\n", success);
            if( success == -3 ) fprintf(stderr, "error: the success check may have a false positive\n");
            if( ! success ) {
                printShaderLog((char*)"error: gl shader program failed to link.", shader);
                //fprintf(stderr, "Exiting.\n");
                //exit(1);
            }
            break;
        default:
            fprintf(stderr, "function called with unhandled case.\n");
            break;
    }
}

void ShaderProgram::printShaderLog(char* errorMessage, GLuint shader) {
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

PNG::PNG() : imageName(NULL), data(NULL) {
    memset(&image, 0x00, sizeof(image));
}

PNG::PNG(char imageName[]) : data(NULL) {
    this->imageName = imageName;
    memset(&image, 0x00, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.opaque = NULL;
    png_image_begin_read_from_file(&image, imageName);
    image.format = PNG_FORMAT_RGBA;
    image.colormap_entries = 0;
    data = (unsigned char*)malloc(image.width * image.height * 4 * sizeof(unsigned char));
    png_image_finish_read(&image, NULL, data, 0, NULL);
}

int PNG::width() {
    return image.width;
}

int PNG::height() {
    return image.height;
}

unsigned char* PNG::pixels() {
    return data;
}

unsigned char* PNG::readPNG(char* imageName) {
    unsigned char* data = NULL;
    png_image image;
    memset(&image, 0x00, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.opaque = NULL;
    png_image_begin_read_from_file(&image, imageName);
    image.format = PNG_FORMAT_RGBA;
    image.colormap_entries = 0;
    data = (unsigned char*)malloc(image.width * image.height * 4 * sizeof(unsigned char));
    png_image_finish_read(&image, NULL, data, 0, NULL);
    png_image_free(&image);
    return data;
}

void PNG::writePNG(char imageName[], unsigned char* data, int width, int height) {
    //GLvoid* data = (GLvoid*)malloc(screenWidth * screenHeight * 4);
    //glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
    png_image image;
    memset(&image, 0x00, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = height;
    image.format = PNG_FORMAT_RGBA;
    image.flags = 0;
    image.opaque = NULL;
    image.colormap_entries = 0;
    png_image_write_to_file(&image, imageName, 0, data, 0, NULL);
    png_image_free(&image);
}

/** A Window*/
View::View(Window* window, int width, int height) {
    parentWindow = window;
    this->width = width;
    this->height = height;
}

void View::scale(float num) {}

void View::translate(float x, float y) {
    viewOffsetX = x * unitsPerPixelX;
    viewOffsetY = y * unitsPerPixelY;
}

/*
            // l key
            case 33:
                currentShape = new Line(pixelToRealX((float)x), pixelToRealY(screenHeight - (float)y));
                break;
            // r key
            case 32:
                currentShape = new Rectangle(pixelToRealX((float)x), pixelToRealY(screenHeight - (float)y));
                break;
                */

/*
void Composer::key(Key key) {
    if(key.action == GLFW_PRESS && key.mods == 0) {
        switch(key.scancode) {
            // space
            case 65:
                view->pan();
                break;
            default:
                lastScancode = key.scancode;
                break;
        }
    } else if(key.action == GLFW_RELEASE && key.mods == 0) {
        switch(key.scancode) {
            // space
            case 65:
                view->endPan();
                break;
            default:
                lastScancode = key.scancode;
                break;
        }
    }
}
*/

Context::Context() {}

void Context::render() {
    for(int i = 0; i < shapes.size(); ++i) {
        shapes[i]->render();
    }
    if(currentShape) {
        //(currentShape->*Shape::renderPtr)();
        currentShape->render();
    }
}

void Shape::render() {
    (this->*renderPtr)();
}

int Shape::dataLength() {
    return lengthOfData;
}

void Shape::frameRender() {
    data.reserve(lengthOfData);
    data[0] = startX;
    data[1] = startY;
    data[2] = endX;
    data[3] = startY;
    data[4] = startX;
    data[5] = endY;
    data[6] = endX;
    data[7] = endY;
    data[8] = startX;
    data[9] = endY;
    data[10] = endX;
    data[11] = startY;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataLength() * sizeof(float), &data[0], GL_STREAM_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    //glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, dataLength() / 2);

    glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// buffer the data one last time
void Shape::finish() {
    data.reserve(lengthOfData);
    data[0] = startX;
    data[1] = startY;
    data[2] = endX;
    data[3] = startY;
    data[4] = startX;
    data[5] = endY;
    data[6] = endX;
    data[7] = endY;
    data[8] = startX;
    data[9] = endY;
    data[10] = endX;
    data[11] = startY;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataLength() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    renderPtr = &Shape::finalRender;
}

void Shape::finalRender() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, dataLength() / 2);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Shape::Shape(float x, float y) {
    renderPtr = &Shape::frameRender;
    startX = x;
    startY = y;
    endX = x;
    endY = y;
}

Window::Window(MyGL* parent, int width, int height) {
    this->parentMyGL = parent;
    this->width = width;
    this->height = height;
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); /** for mac compatability*/
    window = glfwCreateWindow( width, height, "myGL", NULL, NULL);
	if ( !window ) {
		glfwTerminate();
        throw std::runtime_error("GLFW failed to create the window.");
	}
	glfwMakeContextCurrent( window );

    currentView = new View(this, width, height);
	glViewport( 0, 0, width, height );

    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPosCallback(window, parent->cursor_position_callback);
    glfwSetMouseButtonCallback(window, parent->mouse_button_callback);
    glfwSetScrollCallback(window, parent->scroll_callback);
    glfwSetWindowSizeCallback(window,  parent->window_resize_callback);
    glfwSetWindowPosCallback(window, parent->window_move_callback);
    //glfwSetCharCallback(window, parent->character_callback);
    glfwSetKeyCallback(window, parent->key_callback);
    //glfwSetDropCallback(window, parent->drop_callback);

}

Window::~Window() {
    glfwDestroyWindow( window );
}

void MyGL::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    CursorMovement input {xpos, ypos};
}
void MyGL::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    //printf("mouse button callback\n\tbutton %d\n\taction %d\n\tmods %d\n", button, action, mods);
    MouseButton input {button, action, mods};
}
void MyGL::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if(yoffset < 0) {
    } else if(yoffset > 0) {
    }
}

void MyGL::window_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport( 0, 0, width, height );
}

void MyGL::window_move_callback(GLFWwindow* window, int x, int y) {
}


// action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT)
// key GLFW_UNKNOWN
void MyGL::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    printf("%d %d %d\n", key, scancode, mods);
    Key input {key, scancode, action, mods};
}

void MyGL::drop_callback(GLFWwindow* window, int count, const char** paths)
{
    for (int i = 0; i < count; ++i) {
        printf("%s\n", paths[i]);
    }
}

double View::pixelToRealX(double px) {
    return (px - (width / 2.0)) * unitsPerPixelX - viewOffsetX;
}

double View::pixelToRealY(double py) {
    return (py - (height / 2.0)) * unitsPerPixelY - viewOffsetY;
}


MyGL::MyGL() {
	/** Initialize the library */
	if ( !glfwInit() ) {
        throw std::runtime_error("GLFW failed to init.");
	}

	/** Create a windowed mode window and its OpenGL context */
	currentWindow = new Window(this, 700, 700);
    windows.push_back(currentWindow);

	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK ) {
        fprintf(stderr, "GLEW failed to init.\n");
        fprintf(stderr, "Exiting.\n");
        exit(1);
	}
    
    draw();
    //printf( "VENDOR = %s\n", glGetString( GL_VENDOR ) ) ;
    //printf( "RENDERER = %s\n", glGetString( GL_RENDERER ) ) ;
    //printf( "VERSION = %s\n", glGetString( GL_VERSION ) ) ;

    /*
    unsigned char pixels[16*16*4];
    memset(pixels, 0xff, sizeof(pixels));
    GLFWimage image;
    image.width = 16;
    image.height = 16;
    image.pixels = pixels;
    GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
    //GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    if(cursor == NULL) {
        printf("Failed to create GLFW cursor.\n");
    }
    if(glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        printf("Gamepad %s Connected.", glfwGetJoystickName(GLFW_JOYSTICK_1));
    }
    */
}

void MyGL::draw() {
    currentShaderProgram = new ShaderProgram(vertexShaderFileName, fragmentShaderFileName);
    shaderPrograms.push_back(currentShaderProgram);
    //GLuint shaderProgram = createShader((char*)vertexShaderFileName, (char*)fragmentShaderFileName);

    glClearColor( 0.3f, 0.0f, 0.3f, 1.0f );
    glPointSize(10);
    glLineWidth(10);

    // for multiple windows, should close with the last of them? need code change...
	while( !currentWindow->windowShouldClose() ) {
		//glfwPollEvents();
        glfwWaitEvents();
		glClear( GL_COLOR_BUFFER_BIT );
        glUniform1f(currentShaderProgram->viewOffsetX, currentWindow->currentView->viewOffsetX);
        glUniform1f(currentShaderProgram->viewOffsetY, currentWindow->currentView->viewOffsetY);
        glUniform1f(currentShaderProgram->unitsPerPixelX, currentWindow->currentView->unitsPerPixelX);
        glUniform1f(currentShaderProgram->unitsPerPixelY, currentWindow->currentView->unitsPerPixelY);
        glUniform1f(currentShaderProgram->screenWidth,  currentWindow->currentView->width);
        glUniform1f(currentShaderProgram->screenHeight, currentWindow->currentView->height);
        
        if(currentContext) currentContext->render();

        //testCode();

        currentWindow->swapBuffers();
        //testCursorPolling();
	}

	glfwTerminate();
}
