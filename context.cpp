#include "myGL.hpp"

View::View() {
    panning = false;
    mouseHiddenAtX = 0.0;
    mouseHiddenAtY = 0.0;
}

void View::pan() {
    panning = true;
    glfwGetCursorPos(window, &mouseHiddenAtX, &mouseHiddenAtY);
    mouseHiddenAtY = screenHeight - mouseHiddenAtY;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void View::endPan() {
    panning = false;
    viewOffsetRealX += tempViewOffsetX;
    viewOffsetRealY += tempViewOffsetY;
    tempViewOffsetX = 0.0;
    tempViewOffsetY = 0.0;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(window, mouseHiddenAtX, screenHeight - mouseHiddenAtY);
}

void View::cursorMovement(CursorMovement cm) {
    if(panning) {
        tempViewOffsetX = (cm.x - mouseHiddenAtX) * unitsPerPixelX;
        tempViewOffsetY = (screenHeight - cm.y - mouseHiddenAtY) * unitsPerPixelY;
    }
}

// records if the mouse is currently pressed
void Context::mouseButton(MouseButton input) {
    if(input.action == GLFW_PRESS) {
        mousePressed = true;
    } else if(input.action == GLFW_RELEASE) {
        mousePressed = false;
    }
    mb(input);
}

Composer::Composer() {
    view = new View();
    currentShape = NULL;
}

void Composer::cursorMovement(CursorMovement input) {
    if(currentShape && mousePressed) {
        currentShape->cursorMovement(input);
    }
    if(view) view->cursorMovement(input);
}

void Composer::mb(MouseButton mb) {
    if(!currentShape && mb.action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        switch(lastScancode) {
            // l key
            case 33:
                currentShape = new Line(pixelToRealX((float)x), pixelToRealY(screenHeight - (float)y));
                break;
            // r key
            case 32:
                currentShape = new Rectangle(pixelToRealX((float)x), pixelToRealY(screenHeight - (float)y));
                break;

        }
    } else if(currentShape && mb.action == GLFW_RELEASE) {
        printf("finished current shape\n");
        currentShape->finish();
        shapes.push_back(currentShape);
        currentShape = NULL;
    }
}

void Composer::key(Key key) {
    if(key.action == GLFW_PRESS && key.mods == 0) {
        lastScancode = key.scancode;
        switch(key.scancode) {
            // space
            case 65:
                view->pan();
                break;
        }
    } else if(key.action == GLFW_RELEASE && key.mods == 0) {
        switch(key.scancode) {
            // space
            case 65:
                view->endPan();
                break;
        }
    }
}

void Composer::render() {
    for(int i = 0; i < shapes.size(); ++i) {
        (shapes[i]->*renderPtr)();
    }
    if(currentShape) (currentShape->*renderPtr)();
}

Shape::Shape() {
    Shape::renderPtr = &Shape::frameRender;
    bufferUsage = GL_STREAM_DRAW;
}

void Shape::frameRender() {
    prepareTheData();
    genAndBindBufferAndVao();
    glDrawArrays(primitiveType, 0, dataLength() / 2);
    unbindBufferAndVao();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Shape::bindBufferAndVao() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Shape::unbindBufferAndVao() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// buffer the data one last time
void Shape::finish() {
    bufferUsage = GL_STATIC_DRAW;
    prepareTheData();
    genAndBindBufferAndVao();
    renderPtr = &Shape::finalRender;
}

void Shape::finalRender() {
    bindBufferAndVao();
    glDrawArrays(primitiveType, 0, dataLength() / 2);
    unbindBufferAndVao();
}

void Shape::genAndBindBufferAndVao() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataLength() * sizeof(float), &data[0], bufferUsage);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

Line::Line(float x, float y) {
    data.push_back(x);
    data.push_back(y);
    finished = false;
    primitiveType = GL_LINE_STRIP;
}

void Line::prepareTheData() {
    //no-op
}

int Line::dataLength() {
    return data.size();
}

Line::~Line() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Line::cursorMovement(CursorMovement cm) {
    data.push_back(pixelToRealX((float)cm.x));
    data.push_back(pixelToRealY(screenHeight - (float)cm.y));
}

Rectangle::Rectangle(float x, float y) {
    startX = x;
    startY = y;
    endX = x;
    endY = y;
    finished = false;
    primitiveType = GL_TRIANGLES;
}

void Rectangle::cursorMovement(CursorMovement cm) {
    endX = pixelToRealX((float)cm.x);
    endY = pixelToRealY(screenHeight - (float)cm.y);
}

void Rectangle::prepareTheData() {
    data.reserve(12);
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
}

int Rectangle::dataLength() {
    return lengthOfData;
}

