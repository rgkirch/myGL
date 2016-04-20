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
        shapes[i]->render();
    }
    if(currentShape) currentShape->render();
}

Shape::Shape() {
    finished = false;
}

void primitiveShape::finish() {
    finished = true;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataLength() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void primitiveShape::render() {
    if(!finished) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, dataLength() * sizeof(float), &data[0], GL_STREAM_DRAW);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)sizeof(float));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_LINE_STRIP, 0, dataLength() / 2);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_LINE_STRIP, 0, dataLength() / 2);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

Line::Line(float x, float y) {
    data.push_back(x);
    data.push_back(y);
    finished = false;
    primitiveType = GL_LINE_STRIP;
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
    data[0] = x;
    data[1] = y;
    finished = false;
}

void Rectangle::cursorMovement(CursorMovement cm) {
    data[2] = pixelToRealX((float)cm.x);
    data[3] = pixelToRealY((float)cm.y);
}

void Rectangle::render() {
    glRectf(data[0], data[1], data[2], data[3]);
}

int Rectangle::dataLength() {
    return lengthOfData;
}

void Rectangle::finish() {}
