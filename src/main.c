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

    // Create the user data struct.
    user_data_t user_data =
    {
        .window_width = 800,
        .window_height = 600
    };

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

    GLFWwindow* window = glfwCreateWindow(user_data.window_width, user_data.window_height, "Hello triangle", NULL, NULL);
    check_error(window != NULL, "Failed to create window");

    // Store (a pointer to) the user data inside the window.
    glfwSetWindowUserPointer(window, &user_data);

    // Make the OpenGL context of the window the current one.
    glfwMakeContextCurrent(window);

    // Let GLAD load all the OpenGL functions.
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Try to swap every frame.
    glfwSwapInterval(1);

    // Init everything related to OpenGL.
    init_gl(window);

    // Main draw loop
    while (!glfwWindowShouldClose(window))
    {
        // Draw a frame.
        draw_gl(window);

        // Swap the buffers to avoid tearing.
        glfwSwapBuffers(window);

        // React to the window manager's events (e.g. minimize, close, ...).
        glfwPollEvents();
    }

    // Deinit the OpenGL stuff.
    teardown_gl(window);

    // Destroy the window.
    glfwDestroyWindow(window);

    // Terminate GLFW in the end.
    glfwTerminate();

    return 0;
}
