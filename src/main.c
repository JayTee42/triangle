#include <stdio.h>
#include <stdlib.h>

#include "gl_calls.h"

static void draw_frame(GLFWwindow* window)
{
    // Execute the draw call.
    draw_gl(window);

    // Swap the buffers to avoid tearing.
    glfwSwapBuffers(window);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s (%d)\n", description, error);
    exit(EXIT_FAILURE);
}

static void framebuffer_size_callback(GLFWwindow* window, int fb_width, int fb_height)
{
    // Tell OpenGL about the new viewport size.
    glViewport(0, 0, fb_width, fb_height);
    gl_check_error("glViewport");

    // Render a frame to update the window while resizing.
    draw_frame(window);
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    user_data_t* user_data = glfwGetWindowUserPointer(window);

    // Store the new window size (we don't use it yet, though).
    user_data->window_width = width;
    user_data->window_height = height;
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // Specify the remaining callbacks:
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    // Init everything related to OpenGL.
    init_gl(window);

    // Main draw loop
    while (!glfwWindowShouldClose(window))
    {
        // Draw a frame.
        draw_frame(window);

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
