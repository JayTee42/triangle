#include <stdio.h>
#include <stdlib.h>

#include "gl_calls.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s (%d)\n", description, error);
    exit(EXIT_FAILURE);
}

void framebuffer_size_callback(GLFWwindow* window, int fb_width, int fb_height)
{
    // TODO
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    // TODO
}

int main(void)
{
    printf("Hello triangle!\n");

    // Init GLFW
    printf("Initializing GLFW ...\n");

    glfwSetErrorCallback(error_callback);
    check_error(glfwInit(), "Failed to initialize GLFW :(\n");

    // We want at least OpenGL 4.1.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create the window itself.
    printf("Creating window ...\n");

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello triangle", NULL, NULL);
    check_error(window != NULL, "Failed to create window");

    // TODO

    // Destroy the window.
    glfwDestroyWindow(window);

    // Terminate GLFW in the end.
    glfwTerminate();

    return 0;
}
