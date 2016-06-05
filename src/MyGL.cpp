#include "MyGL.hpp"

std::mutex contextMutex;
std::mutex glfwMutex;
bool glfwInited;
bool glewInited;

typedef void (Shape::*renderFunc)();

void monitor_callback(GLFWmonitor *monitor, int x) {
    glfwTerminate();
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
}

WindowHints::WindowHints() {
	glfw_context_version_major = 3;
	glfw_context_version_minor = 3;
	glfw_opengl_profile = GLFW_OPENGL_CORE_PROFILE;
	glfw_resizable = 0;
    glfw_opengl_forward_compat = 1;
    glfw_focused = 1;
    glfw_decorated = 1;
    glfw_visible = 1;
    clearColor.x = 0.0;
    clearColor.y = 0.0;
    clearColor.z = 0.0;
    location.x = 0.0f;
    location.y = 0.0f;
    width = 200;
    height = 200;
}

WindowHints::WindowHints(const WindowHints& wh) {
	glfw_context_version_major = wh.glfw_context_version_major;
	glfw_context_version_minor = wh.glfw_context_version_minor;
	glfw_opengl_profile = wh.glfw_opengl_profile;
	glfw_resizable = wh.glfw_resizable;
    glfw_opengl_forward_compat = wh.glfw_opengl_forward_compat;
    glfw_focused = wh.glfw_focused;
    glfw_decorated = wh.glfw_decorated;
    glfw_visible = wh.glfw_visible;
    clearColor.x = wh.clearColor.x;
    clearColor.y = wh.clearColor.y;
    clearColor.z = wh.clearColor.z;
    location.x = wh.location.x;
    location.y = wh.location.y;
    width = wh.width;
    height = wh.height;
}

/** Reads in a couple text files, compiles and links stuff and makes a shader program. Calls glUseProgram at the end and then needs to set up the uniforms.*/
ShaderProgram::ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName) {
    program = glCreateProgram();
    if( ! program ) throw std::runtime_error("failed to create program.");
    std::string vertexShaderCode = readFile(vertexShaderFileName);
    std::string fragmentShaderCode = readFile(fragmentShaderFileName);
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    GLchar *vcode = new char[vertexShaderCode.length()+1];
    GLchar *fcode = new char[fragmentShaderCode.length()+1];
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
    //std::ifstream in(fileName);
    //std::string data;
    //data = static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str();
    //std::cout << data << std::endl;

    std::ifstream stream(fileName);
    std::string data;
    std::ostringstream string;
    string << stream.rdbuf();
    data = string.str();
    return data;
    //std::streamsize size = stream.rdbuf()->pubseekoff(0, stream.end);
    //stream.rdbuf()->pubseekoff(0, stream.beg);
    ////char *data = (char*)malloc(size * sizeof(char));
    //std::stringstream data(stream.rdbuf());
    ////char *data = new char[size];
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

void ShaderProgram::printShaderLog(char *errorMessage, GLuint shader) {
    fprintf(stderr, "%s\n", errorMessage);
    GLint length = 0;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );
    printf("log length %d\n", length);
    GLchar *logText = (GLchar*)malloc(sizeof(GLchar) * (length + 1));
    logText[length] = '\0';
    glGetShaderInfoLog(shader, length, &length, logText);
    printf("printing log\n");
    fprintf(stderr, "%s", logText);
    //cout << logText << endl;
    free(logText);
}

View::View(Window *window, int width, int height) {
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

void Context::render(ShaderProgram *shader, View *view) {
    glUniform1f(shader->viewOffsetX,    view->viewOffsetX);
    glUniform1f(shader->viewOffsetY,    view->viewOffsetY);
    glUniform1f(shader->unitsPerPixelX, view->unitsPerPixelX);
    glUniform1f(shader->unitsPerPixelY, view->unitsPerPixelY);
    glUniform1f(shader->screenWidth,    view->width);
    glUniform1f(shader->screenHeight,   view->height);

    for(size_t i = 0; i < shapes.size(); ++i) {
        printf("shape vector size %d\n", (int)shapes.size());
        if(shapes[i]) shapes[i]->render();
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

Shape::Shape(float x, float y, float ex, float ey) {
    renderPtr = &Shape::frameRender;
    startX = x;
    startY = y;
    endX = ex;
    endY = ey;
}

Window::Window(MyGL *parent, const WindowHints& wh) {
    this->parentMyGL = parent;
    this->width = wh.width;
    this->height = wh.height;
    if(!glfwInited) {
        if ( !glfwInit() ) {
            throw std::runtime_error("GLFW failed to init.");
        }
        glfwSetErrorCallback(error_callback);
        glfwSetMonitorCallback(monitor_callback);
    }
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, wh.glfw_context_version_major );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, wh.glfw_context_version_minor );
	//glfwWindowHint( GLFW_OPENGL_PROFILE, wh.glfw_opengl_profile );
	glfwWindowHint( GLFW_RESIZABLE, wh.glfw_resizable );
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, wh.glfw_opengl_forward_compat ); /** for mac compatability*/
    glfwWindowHint( GLFW_FOCUSED, wh.glfw_focused );
    glfwWindowHint( GLFW_DECORATED, wh.glfw_decorated );
    glfwWindowHint( GLFW_VISIBLE, wh.glfw_visible );
    clearColorRed = wh.clearColor.x;
    clearColorGreen = wh.clearColor.y;
    clearColorBlue = wh.clearColor.z;
    window = glfwCreateWindow( width, height, "myGL", NULL, NULL);
	if ( !window ) {
		glfwTerminate();
        throw std::runtime_error("GLFW failed to create the window in Window constructor.");
	}
    if(!glewInited) {
        std::unique_lock<std::mutex> contextLock(contextMutex);
        glfwMakeContextCurrent( window );

        glewExperimental = GL_TRUE;
        if( glewInit() != GLEW_OK ) {
            fprintf(stderr, "GLEW failed to init.\n");
            fprintf(stderr, "Exiting.\n");
            throw std::runtime_error("glew failed to init");
        }
        glfwMakeContextCurrent( NULL );
        contextLock.unlock();
    }
    glfwSetWindowPos(window, wh.location.x, wh.location.y);

    std::unique_lock<std::mutex> contextLock(contextMutex);
	glfwMakeContextCurrent( window );

    //currentView = new View(this, width, height);
	glViewport( 0, 0, width, height );
    glfwSwapInterval(1);

	glfwMakeContextCurrent( NULL );
    contextLock.unlock();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(window, parent);

    glfwSetCursorPosCallback(window,   glfwInputCallback::cursor_position_callback);
    glfwSetMouseButtonCallback(window, glfwInputCallback::mouse_button_callback);
    glfwSetScrollCallback(window,      glfwInputCallback::scroll_callback);
    glfwSetWindowSizeCallback(window,  glfwInputCallback::window_resize_callback);
    glfwSetWindowPosCallback(window,   glfwInputCallback::window_move_callback);
    //glfwSetCharCallback(window,      glfwInputCallback::character_callback);
    glfwSetKeyCallback(window,         glfwInputCallback::key_callback);
    //glfwSetDropCallback(window,      glfwInputCallback::drop_callback);

}

void Window::hide() {
    glfwHideWindow(window);
}

void Window::show() {
    glfwShowWindow(window);
}

void Window::moveRelative(int x, int y) {
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);
    glfwSetWindowPos(window, xpos + x, ypos + y);
}

void Window::moveAbsolute(int x, int y) {
    glfwSetWindowPos(window, x, y);
}

void Window::close() {
    glfwSetWindowShouldClose(window, 1);
}

void Window::loop() {
    if(window && !glfwWindowShouldClose(window)) {
        std::lock_guard<std::mutex> lock(contextMutex);
        glfwMakeContextCurrent( window );
        //glClearColor( 0.3f, 0.0f, 0.3f, 1.0f );
        glClearColor( clearColorRed, clearColorGreen, clearColorBlue, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        glfwSwapBuffers( window );
        glfwMakeContextCurrent( NULL );
    } else if(window) glfwDestroyWindow( window );
}

Window::~Window() {
    glfwSetWindowShouldClose(window, 1);
    if(window) glfwDestroyWindow( window );
}

bool Window::handles(GLFWwindow *window) {
    return this->window == window;
}

void glfwInputCallback::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    //printf("window %p\n", window);
    CursorMovement input {xpos, ypos};
    MyGL *mygl = static_cast<MyGL*>(glfwGetWindowUserPointer(window));
}
void glfwInputCallback::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    //printf("mouse button callback\n\tbutton %d\n\taction %d\n\tmods %d\n", button, action, mods);
    MouseButton input {button, action, mods};
}
void glfwInputCallback::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if(yoffset < 0) {
    } else if(yoffset > 0) {
    }
}

void glfwInputCallback::window_resize_callback(GLFWwindow *window, int width, int height) {
	glViewport( 0, 0, width, height );
}

void glfwInputCallback::window_move_callback(GLFWwindow *window, int x, int y) {
}


// action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT)
// key GLFW_UNKNOWN
void glfwInputCallback::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    printf("%d %d %d\n", key, scancode, mods);
    const Key input {key, scancode, action, mods};
    MyGL* mygl = static_cast<MyGL*>((glfwGetWindowUserPointer)(window));
    if(mygl && mygl->inputFunction) {
        mygl->inputFunction(input);
    }
}

void glfwInputCallback::drop_callback(GLFWwindow *window, int count, const char **paths)
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


MyGL::MyGL(std::string str) {
}

MyGL::MyGL() {

    //shaderPrograms.push_back(std::unique_ptr<ShaderProgram>(new ShaderProgram(vertexShaderFileName, fragmentShaderFileName)));
    //printMonitorInfo();
    //SnakeGame::snakeGame(this);


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
    GLFWcursor *cursor = glfwCreateCursor(&image, 0, 0);
    //GLFWcursor *cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    if(cursor == NULL) {
        printf("Failed to create GLFW cursor.\n");
    }
    if(glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        printf("Gamepad %s Connected.", glfwGetJoystickName(GLFW_JOYSTICK_1));
    }
    */
}

MyGL::~MyGL() {
}

// TODO - how does caller know that it returned the last image, return tuple with bool?
ImageIterator::ImageIterator(std::string dirName) {
    dirIter = boost::filesystem::recursive_directory_iterator(dirName);
}

Magick::Image ImageIterator::operator()() {
    Magick::Image pic;
    while(dirIter != boost::filesystem::recursive_directory_iterator()) {
        while(!boost::filesystem::is_regular_file(dirIter->path())) { // TODO - what happens when it runs out of files
            dirIter++;
        }
        try {
            pic.read(dirIter->path().string());
            std::cout << dirIter->path() << std::endl;
            ++dirIter;
            break;
        } catch(Magick::Exception& e) {
            //std::cout << e.what() << std::endl;
            ++dirIter;
        }
    }
    return pic;
}

// TODO - use instancing
void MyGL::renderSquare() {
    //float bufferData[] = {-1.0, 1.0, -1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    float bufferData[] = {-1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0};
    //float bufferData[] = {0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0};
    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), bufferData, GL_STREAM_DRAW); // GL_STATIC_DRAW, GL_STREAM_DRAW
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    //glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// assume directory is legit
void MyGL::collage(std::string directory) {
    int tileSize = 200;
    int tilesWide = 6;
    int tilesHigh = 4;
    int numTiles = tilesWide * tilesHigh;
    Magick::InitializeMagick(NULL);
    std::unique_ptr<Window> win;
    WindowHints wh;
    wh.clearColor = glm::vec3(1.0, 1.0, 1.0);
    wh.width = tilesWide * tileSize;
    wh.height = tilesHigh * tileSize;
    win = std::make_unique<Window>(this, wh);
    glfwMakeContextCurrent( win->window );
    ShaderProgram shader(std::string("vertexShader.glsl"), std::string("fragmentShader.glsl"));

    GLuint tex[numTiles];
    glGenTextures(numTiles, tex);

    ImageIterator imgIter(directory);

    for(int texNum = 0; texNum < numTiles;) {
        int texWidth;
        int texHeight;
        Magick::Image pic = imgIter();
        //pic.read(dirIter->path().string());
        pic.flip();
        texWidth = pic.columns();
        texHeight = pic.rows();
        char *data = new char[3 * texWidth * texHeight]();
        pic.write(0, 0, texWidth, texHeight, "RGB", Magick::CharPixel, data);

        glActiveTexture(GL_TEXTURE0 + texNum);
        glBindTexture(GL_TEXTURE_2D, tex[texNum]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        std::cout << "added texture" << std::endl;
        ++texNum;
    }

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    int delay = 200;
    tp += std::chrono::milliseconds(delay);
    int interval = 0; /** Always increasing. Increment per new pic read.*/
    //while(std::chrono::system_clock::now() < tp) {
    while(!glfwWindowShouldClose(win->window)) {
        glfwPollEvents();

        if(std::chrono::system_clock::now() > tp) {
            tp += std::chrono::milliseconds(delay);
            int texWidth;
            int texHeight;
            Magick::Image pic = imgIter();
            //pic.read(dirIter->path().string());
            pic.flip();
            texWidth = pic.columns();
            texHeight = pic.rows();
            std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(3 * texWidth * texHeight);
            pic.write(0, 0, texWidth, texHeight, "RGB", Magick::CharPixel, &data[0]);

            glActiveTexture(GL_TEXTURE0 + interval % numTiles);
            glBindTexture(GL_TEXTURE_2D, tex[interval % (tilesWide * tilesHigh)]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "added texture" << std::endl;
            ++interval;
        }

        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        for(int tile = 0; tile < numTiles; ++tile) {
            glm::mat4 translationMatrix = glm::translate(glm::vec3((-1.0 + 2.0 / tilesWide / 2.0) + ((tile % tilesWide) * (2.0 / tilesWide)), (1.0 - 2.0 / tilesHigh / 2.0) - ((tile / tilesHigh) * (2.0 / tilesHigh)), 0.0f));
            glm::mat4 rotationMatrix(1.0f);
            glm::mat4 scaleMatrix = glm::scale(glm::vec3(1.0 / tilesWide, 1.0 / tilesHigh, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.program, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shader.program, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shader.program, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
            glUniform1i(glGetUniformLocation(shader.program, "texture"), tile);
            renderSquare();
        }
        glfwSwapBuffers( win->window );
    }
    glfwMakeContextCurrent( NULL );
    //glDeleteTextures(16, tex);
    //delete[] data;
}

void MyGL::end() {
    glfwTerminate();
}

void MyGL::genLotsWindows() {
    int windowPosition = 0;
    for(int i = 0; i < 20; ++i) {
        //Window *temp = new Window(this, 50, 50);
        //glfwSetWindowPos(temp->window, windowPosition % 1920, (windowPosition / 1920) * 50 % 1080);
        //windows.push_back(std::unique_ptr<Window>(temp));
        //windowPosition += 50;
        //delete temp;
    }
}

GLFWwindow* MyGL::makeWindowForContext() {
    int width = 1;
    int height = 1;

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	//glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); /** for mac compatability*/
    glfwWindowHint( GLFW_FOCUSED, GL_FALSE );
    glfwWindowHint( GLFW_DECORATED, GL_FALSE );
    glfwWindowHint( GLFW_VISIBLE, GL_FALSE );

    GLFWwindow* win = glfwCreateWindow( width, height, "myGL", NULL, NULL);

	if ( !win ) {
		glfwTerminate();
        throw std::runtime_error("GLFW failed to create the window for hidden context.");
	}

    std::lock_guard<std::mutex> lock(contextMutex);
	glfwMakeContextCurrent( win );

    //currentView = new View(this, width, height);
	glViewport( 0, 0, width, height );

    glfwSwapInterval(1);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glClearColor( 0.3f, 0.0f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glfwSwapBuffers( win );

	glfwMakeContextCurrent( NULL );
    return win;
}

void SnakeGame::snakeGame(MyGL *application) {
    std::unordered_map<int, std::unique_ptr<Window>> grid; // row major

    int numberOfMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numberOfMonitors);
    const GLFWvidmode *mode;
    int x = 0;
    int y = 0;
    int screenWidth;
    int screenHeight;
    int tileSize = 100; //px
    mode = glfwGetVideoMode(monitors[numberOfMonitors-1]);
    screenWidth = mode->width;
    screenHeight = mode->height;
    int gridHeight = screenHeight / tileSize;
    int gridWidth = screenWidth / tileSize;
    std::cout << "gridWidth " << gridWidth << " gridHeight " << gridHeight << std::endl;
    int gridSize = gridHeight * gridWidth;

    //std::list<int> movement;
    int movement;
    //movement.push_back(1);
    movement = 1;
    application->inputFunction = [&](const Key& key) {
        if(key.action == GLFW_PRESS) {
            switch(key.scancode) {
                case 111: // up
                    //movement.push_back(-gridWidth);
                    movement = movement == gridWidth ? movement : -gridWidth;
                    break;
                case 114: //right
                    //movement.push_back(1);
                    movement = movement == -1 ? movement : 1;
                    break;
                case 116: //down
                    //movement.push_back(gridWidth);
                    movement = movement == -gridWidth ? movement : gridWidth;
                    break;
                case 113: //left
                    //movement.push_back(-1);
                    movement = movement == 1 ? movement : -1;
                    break;
            }
        }
    };

    struct WindowHints snakeWindowHint;
    snakeWindowHint.glfw_decorated = 0;
    snakeWindowHint.glfw_visible = 1;
    snakeWindowHint.clearColor.x = 1.0f;
    snakeWindowHint.glfw_focused = 0;
    struct WindowHints foodWindowHint(snakeWindowHint);
    foodWindowHint.clearColor.x = 0.0f;
    foodWindowHint.clearColor.y = 1.0f;

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    tp += std::chrono::milliseconds(200);


    int head = 0;
    std::list<int> snake;
    for(; head < 5; ++head) {
        snake.push_back(head);
        snakeWindowHint.location.x = (head % gridWidth) * tileSize;
        snakeWindowHint.location.y = (head / gridWidth) * tileSize;
        snakeWindowHint.width = tileSize;
        snakeWindowHint.height = tileSize;
        grid.insert(std::make_pair(head, std::make_unique<Window>(application, snakeWindowHint)));
        grid.find(head)->second->loop();
    }
    head--;

    int food = SnakeGame::newFoodLocation(snake.size(), gridSize, grid);

    foodWindowHint.location.x = (food % gridWidth) * tileSize;
    foodWindowHint.location.y = (food / gridWidth) * tileSize;
    foodWindowHint.width = tileSize;
    foodWindowHint.height = tileSize;
    grid.insert(std::make_pair(food, std::make_unique<Window>(application, foodWindowHint)));
    grid.find(food)->second->loop();

    int chrono = 200;

    while(1) {
        //glfwPostEmptyEvent();
        glfwPollEvents();
        if(std::chrono::system_clock::now() > tp) {
            //head += movement.front();
            if(movement == 1 && head % gridWidth == gridWidth - 1) {
                std::cout << "Out of bounds. You lose. Score " << snake.size() << std::endl;
                break;
            }
            if(movement == -1 && head % gridWidth == 0) {
                std::cout << "Out of bounds. You lose. Score " << snake.size() << std::endl;
                break;
            }
            head += movement;
            if(head < 0 || head >= (gridWidth * gridHeight)) {
                std::cout << "Out of bounds. You lose. Score " << snake.size() << std::endl;
                break;
            }
            if(head == food) {
                std::cout << "eat" << std::endl;
                if(chrono > 50) {
                    chrono--;
                }
                grid.find(head)->second->clearColorRed = 1.0;
                grid.find(head)->second->clearColorGreen = 0.0;
                grid.find(head)->second->loop();
                snake.push_back(head);
                if(gridSize - snake.size() == 0) {
                    printf("You win!\n");
                    break;
                }
                food = newFoodLocation(snake.size(), gridSize, grid);
                foodWindowHint.location.x = (food % gridWidth) * tileSize;
                foodWindowHint.location.y = (food / gridWidth) * tileSize;
                foodWindowHint.width = tileSize;
                foodWindowHint.height = tileSize;
                grid.insert(std::make_pair(food, std::make_unique<Window>(application, foodWindowHint)));
                grid.find(food)->second->loop();
            } else {
                //grid.find(temp)->second->close();
                //grid.find(temp)->second->loop();
                grid.erase(snake.front());
                snake.erase(snake.begin());
                if(grid.find(head) != grid.end()) {
                    std::cout << "Ran into self. You lose. Score " << snake.size() << std::endl;
                    break;
                }
                snakeWindowHint.location.x = (head % gridWidth) * tileSize;
                snakeWindowHint.location.y = (head / gridWidth) * tileSize;
                snakeWindowHint.width = tileSize;
                snakeWindowHint.height = tileSize;
                grid.insert(std::make_pair(head, std::make_unique<Window>(application, snakeWindowHint)));
                grid.find(head)->second->loop();
                snake.push_back(head);
            }
            tp += std::chrono::milliseconds(chrono);
        }

        /*
        for(const auto x : snake) {
            //std::cout << x;
            grid.find(x)->second->loop();
        }
        */
        //std::cout << std::endl;
        if(grid.find(food) != grid.end()) grid.find(food)->second->loop();
    }
    for(auto item = snake.begin(), end = snake.end(); item != end;) {
        // remove them
        grid.find(*item)->second->close();
        grid.erase(*item);
        item = snake.erase(item);
    }
    printf("finish snake game\n");



    //create one window in the top left
    //the direction will be to the right
    //loop this
    //    sleep
    //    move in direction
    //    check if food window at index
    //        if food window
    //            remove food and place new window at index
    //            place food somewhere else
    //            if nowhere else to place food
    //                win the game
    //    else check if snake window at index
    //        if window at index, lose
    //    else check if no window
    //        place new window


    /*
    while(! glfwWindowShouldClose(windowForContext)) {
        glfwWaitEvents();
        std::lock_guard<std::mutex> lock(contextMutex);
        glfwMakeContextCurrent( windowForContext );
        glClearColor( 0.3f, 0.0f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        glfwSwapBuffers( windowForContext );
        glfwMakeContextCurrent( NULL );
    }
    */
}

int SnakeGame::newFoodLocation(int snakeSize, int gridSize, std::unordered_map<int, std::unique_ptr<Window>>& grid) {
    int r = rand() % (gridSize - snakeSize);
    for(int i = 0; i < gridSize; ++i) {
        if(grid.find(i) == grid.end()) {
            if(r == 0) {
                return i;
            }
            r--;
        }
    }
    throw std::runtime_error("this function should have returned");
}

void SnakeGame::stepNorth() {}
void SnakeGame::stepEast() {}
void SnakeGame::stepSouth() {}
void SnakeGame::stepWest() {}

void printMonitorInfo() {
    int numberOfMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numberOfMonitors);
    const GLFWvidmode *mode;
    int x = 0;
    int y = 0;
    for(int i = 0; i < numberOfMonitors; ++i) {
        glfwGetMonitorPhysicalSize(monitors[i], &x, &y);
        std::cout << "monitor " << i << " is " << x << "mm wide and " << y << "mm tall" << std::endl;
        mode = glfwGetVideoMode(monitors[i]);
        std::cout << mode->width << " pixels wide by " << mode->height << " pixels high " << std::endl;
        glfwGetMonitorPos(monitors[i], &x, &y);
        std::cout << "monitor " << i << " is " << x << " and " << y << std::endl;
    }
}

int ls(boost::filesystem::path p) {
    std::cout << std::this_thread::get_id() << std::endl;
    using namespace boost::filesystem;
    if(exists(p)) {
        if(is_regular_file(p)) {
            //std::cout << p << std::endl;
            return 1;
        } else if(is_directory(p)) {
            std::vector<std::future<int>> vec;
            try {
                directory_iterator d(p);
                while(d != directory_iterator()) {
                    vec.push_back(std::async(ls, d->path()));
                    //children.push_back(std::make_unique<std::thread>([=]{ls(d->path());}));
                    d++;
                }
            } catch (const filesystem_error& ex) {
                std::cout << ex.what() << std::endl;
            }
            return std::accumulate(vec.begin(), vec.end(), 0, [] (int& s, std::future<int>& f) -> int {return s + f.get();});
        }
    }
    return 0;
}

void boostFun(std::string str) {
    using namespace boost;
    //filesystem::path p(std::string("/home/eve/Github/myGL/"));
    filesystem::path p(str);
    //std::copy(boost::filesystem::directory_iterator(d), boost::filesystem::directory_iterator(), std::back_inserter(vec));
    std::cout << ls(p) << std::endl;
}
