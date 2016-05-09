#include <iostream>
#include <glm/glm.hpp>

int main() {
    glm::vec2 t(1.0, 2.0);
    glm::vec4 f(t, 3.0, 4.0);
    std::cout << f.x << f.y << f.z << f.w << std::endl;

}
