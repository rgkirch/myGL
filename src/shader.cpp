#include "shader.hpp"

/** Reads in a couple text files, compiles and links stuff and makes a shader program. Calls glUseProgram at the end and then needs to set up the uniforms.*/
ShaderProgram::ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName)
{
    program = glCreateProgram();
    if( ! program )
    {
        throw std::runtime_error("failed to create program.");
    }
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

GLuint ShaderProgram::id()
{
    return program;
}

std::string ShaderProgram::readFile(std::string fileName)
{
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

void ShaderProgram::checkShaderStepSuccess(GLuint shader, GLuint status)
{
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

void ShaderProgram::printShaderLog(char *errorMessage, GLuint shader)
{
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

