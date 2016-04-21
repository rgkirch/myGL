                                // draw.cpp
#include "myGL.hpp"

FILE* stdLog;
Context* currentContext;
struct shaderUniforms shaderUniforms;

const char* vertexShaderFileName = "vertexShader.glsl";
const char* fragmentShaderFileName = "fragmentShader.glsl";
int screenWidth = 700;
int screenHeight = 700;
float xmax, ymax, xmin, ymin;
double mouseHiddenAtX;
double mouseHiddenAtY;

GLFWwindow* window;
char windowTitle[] = "plot";
//vector<float*> lines;
std::vector<int> pointLengths;
std::vector<GLuint> vbos;
std::vector<GLuint> vaos;
std::vector<GLuint> primitiveType;
std::vector<float> glRect;
keyboardLayout keyboardL;

//camera - coord of top left of screen
double tempViewOffsetX = 0.0;
double tempViewOffsetY = 0.0;
double viewOffsetRealX = 0.0;
double viewOffsetRealY = 0.0;
double unitsPerPixelX = 1.0;
double unitsPerPixelY = 1.0;
double pixelToRealX(double x);
double pixelToRealY(double y);

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

char* intToString(int num) {
    if(num <= 0) return NULL;
    int cpy = num;
    int length = 0;
    while(cpy > 0) {
        ++length;
        cpy /= 10;
    }
    char* string = (char*)malloc(length * sizeof(char) + 1);
    string[length] = '\0';
    for(int i = 1; num > 0; ++i) {
        string[length - i] = num % 10 + '0';
        num /= 10;
    }
    return string;
}

void testCursorPolling() {
    static double lastTime = 0.0;
    double thisTime;
    double x, y;
    if((thisTime = glfwGetTime()) - lastTime > 1) {
        glfwGetCursorPos(window, &x, &y);
        printf("%f, %f\n", x, screenHeight - y);
        lastTime = thisTime;
    }
}

/*
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
*/

void draw() {
    currentContext = new Composer();

    GLuint shaderProgram = createShader((char*)vertexShaderFileName, (char*)fragmentShaderFileName);
    glUseProgram(shaderProgram);
    shaderUniforms.viewOffsetX = glGetUniformLocation(shaderProgram, "viewOffsetX");
    shaderUniforms.viewOffsetY = glGetUniformLocation(shaderProgram, "viewOffsetY");
    shaderUniforms.unitsPerPixelX = glGetUniformLocation(shaderProgram, "unitsPerPixelX");
    shaderUniforms.unitsPerPixelY = glGetUniformLocation(shaderProgram, "unitsPerPixelY");
    shaderUniforms.screenWidth = glGetUniformLocation(shaderProgram, "screenWidth");
    shaderUniforms.screenHeight = glGetUniformLocation(shaderProgram, "screenHeight");

    glClearColor( 0.3f, 0.0f, 0.3f, 1.0f );
    glPointSize(10);
    glLineWidth(10);

	while( !glfwWindowShouldClose( window ) ) {
		//glfwPollEvents();
        glfwWaitEvents();
		glClear( GL_COLOR_BUFFER_BIT );
        glUniform1f(shaderUniforms.viewOffsetX, viewOffsetRealX + tempViewOffsetX);
        glUniform1f(shaderUniforms.viewOffsetY, viewOffsetRealY + tempViewOffsetY);
        glUniform1f(shaderUniforms.unitsPerPixelX, unitsPerPixelX);
        glUniform1f(shaderUniforms.unitsPerPixelY, unitsPerPixelY);
        glUniform1f(shaderUniforms.screenWidth, screenWidth);
        glUniform1f(shaderUniforms.screenHeight, screenHeight);
        
        if(currentContext) currentContext->render();

		glfwSwapBuffers( window );
        //testCursorPolling();
	}

	glfwDestroyWindow( window );
	glfwTerminate();
    fclose(stdLog);
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

void init(int argc, char* argv[]) {
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            if(strcmp(argv[i], (char*)"-k")) {
                if(strcmp(argv[i+1], (char*)"dvorak")) {
                    keyboardL = keyboardLayout::DVORAK;
                    ++i;
                } else if(strcmp(argv[i+1], (char*)"us")) {
                    keyboardL = keyboardLayout::US;
                    ++i;
                } else {
                    printf("Not a valid keyboard option.\n");
                    ++i;
                }
            } else if(strcmp(argv[i], (char*)"-h")) {
                printf("-k keyboard layout {us, dvorak}");
            } else {
                printf("Not a valid option. Run with -h for a list of flags.\n");
            }
        }
    }

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
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

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
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window,  window_resize_callback);
    glfwSetWindowPosCallback(window, window_move_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetDropCallback(window, drop_callback);

    //printf( "VENDOR = %s\n", glGetString( GL_VENDOR ) ) ;
    //printf( "RENDERER = %s\n", glGetString( GL_RENDERER ) ) ;
    //printf( "VERSION = %s\n", glGetString( GL_VERSION ) ) ;
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
    stdLog = fopen("log.txt", "w");
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    CursorMovement input {xpos, ypos};
    if(currentContext) currentContext->cursorMovement(input);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    //printf("mouse button callback\n\tbutton %d\n\taction %d\n\tmods %d\n", button, action, mods);
    MouseButton input {button, action, mods};
    if(currentContext) currentContext->mouseButton(input);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if(yoffset < 0) {
        zoomOut();
    } else if(yoffset > 0) {
        zoomIn();
    }
    fprintf(stdLog, "scale %.2f %.2f\n", unitsPerPixelX, unitsPerPixelY);
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
    screenWidth = width;
    screenHeight = height;
	glViewport( 0, 0, screenWidth, screenHeight );
    fprintf(stdLog, "window resized to %d %d\n", screenWidth, screenHeight);
}

void window_move_callback(GLFWwindow* window, int x, int y) {
}


// action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT)
// key GLFW_UNKNOWN
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    printf("%d %d %d\n", key, scancode, mods);
    Key input {key, scancode, action, mods};
    if(currentContext) currentContext->key(input);
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    fprintf(stdLog, "drop callback\n");
    for (int i = 0; i < count; ++i) {
        printf("%s\n", paths[i]);
    }
}

void saveBufferAsBytes() {
    printf("screenshot\n");
    //fprintf(stdLog, "screenshot saved\n");
    FILE* file = fopen("screenshot", "wb");
    GLvoid* data = (GLvoid*)malloc(screenWidth * screenHeight * 4);
    glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
    fwrite(data, screenWidth * screenHeight * 4, 1, file);
    fclose(file);
}

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    //printf("character callback\n");
    /*
    switch(codepoint) {
        case '/':
            //saveBufferAsBytes();
            writePNG((char*)"screenshot.png");
            break;
        case 'c':
            static int i = 33;
            if(i < 127) {
                char name[8] = {'\0'};
                strcpy(name, intToString(i));
                strcat(name, ".png");
                printf("saving %s\n", name);
                writePNG(name);
                printf("next %c\n", i + 1);
                ++i;
            }
            break;
        default:
            break;
    }
    */
}

void zoomOut() {
    fprintf(stdLog, "scroll up\n");
    unitsPerPixelX *= 1.0 + scrollSpeedMultiplier;
    unitsPerPixelY *= 1.0 + scrollSpeedMultiplier;
}

void zoomIn() {
    fprintf(stdLog, "scroll down\n");
    unitsPerPixelX *= 1.0 - scrollSpeedMultiplier;
    unitsPerPixelY *= 1.0 - scrollSpeedMultiplier;
}

void writePNG(char* imageName) {
    GLvoid* data = (GLvoid*)malloc(screenWidth * screenHeight * 4);
    glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
    png_image image;
    memset(&image, 0x00, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = screenWidth;
    image.height = screenHeight;
    image.format = PNG_FORMAT_RGBA;
    image.flags = 0;
    image.opaque = NULL;
    image.colormap_entries = 0;
    png_image_write_to_file(&image, imageName, 0, data, 0, NULL);
    png_image_free(&image);
}

void manualMapKeys() {
    for(int i = 33; i <= 126; ++i) {
    }
}

int readPNG() {
/*
    int imageSizeBytes = 0;
    char imageName[] = "screenShot.png";
    png_image image;
    memset(&image, 0x00, imageSizeBytes);
    image.version = PNG_IMAGE_VERSION;
    image.width = screenWidth;
    image.height = screenHeight;
    if (png_image_begin_read_from_file(&image, imageName) != 0) {
        png_bytep buffer;
        image.format = PNG_FORMAT_RGBA;
        buffer = (unsigned char*)malloc(PNG_IMAGE_SIZE(image));
        if (buffer != NULL && png_image_finish_read(&image, NULL, buffer, 0, NULL) != 0) {
        }
    }
*/
    return 0;
}
    //When libpng encounters an error, it expects to longjmp back to your routine. There- fore, you will need to call setjmp and pass your png_jmpbuf(png_ptr). If you read the file from different routines, you will need to update the jmpbuf field every time you enter a new routine that will call a png_*() function.
