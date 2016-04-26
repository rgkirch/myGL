#include "myGL.hpp"


void testCode() {
    char name[] = "100.png";
    PNG* image = new PNG(name);
    int levelOfDetail = 0; // base level

    Rectangle* rect = new Rectangle(-300.0, -300.0, 300.0, 300.0);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, levelOfDetail, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels());
    //glGenerateMipmap(GL_TEXTURE_2D);
    rect->render();
}

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

/*
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
*/

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

}


/*
void saveBufferAsBytes() {
    printf("screenshot\n");
    //fprintf(stdLog, "screenshot saved\n");
    FILE* file = fopen("screenshot", "wb");
    GLvoid* data = (GLvoid*)malloc(screenWidth * screenHeight * 4);
    glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
    fwrite(data, screenWidth * screenHeight * 4, 1, file);
    fclose(file);
}
*/

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

/*
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
*/

    //When libpng encounters an error, it expects to longjmp back to your routine. There- fore, you will need to call setjmp and pass your png_jmpbuf(png_ptr). If you read the file from different routines, you will need to update the jmpbuf field every time you enter a new routine that will call a png_*() function.
