#ifndef GLCALLS_H
#define GLCALLS_H

// Include the GLAD loader *before* including GLFW!
#include "glad/glad.h"

// Include the GLFW library (should be the same for all OS):
#include <GLFW/glfw3.h>

// Our struct for all kinds of user data
typedef struct
{
	// Dimensions of the window
	int window_width;
	int window_height;

	// The shader program
	GLuint shader_program;

	// The VAO (blackbox, just ignore it ...)
	GLuint vao;

	// The VBO (GPU memory that holds the vertex data)
	GLuint vbo;
} user_data_t;

#define ATTRIB_POSITION 0
#define ATTRIB_COLOR 1

typedef struct
{
	GLfloat position[2];
	GLubyte color[3];
} vertex_data_t;

// Generic error checks:
void check_error(int condition, const char* error_text);
void gl_check_error(const char* error_text);

// GL functions:
void init_gl(GLFWwindow* window);
void draw_gl(GLFWwindow* window);
void teardown_gl(GLFWwindow* window);

#endif
