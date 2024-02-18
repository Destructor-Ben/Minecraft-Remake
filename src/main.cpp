#include <GL.h>
#include <GLFW.h>

int main() {
    glfwInit();
    auto window = glfwCreateWindow(100, 100, "Test", nullptr, nullptr);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    return 0;
}
