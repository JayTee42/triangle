#include "gl_calls.h"

#include <stdio.h>
#include <stdlib.h>

void check_error(int condition, const char* error_text)
{
    if (!condition)
    {
        fprintf(stderr, "%s\n", error_text);
        exit(EXIT_FAILURE);
    }
}

void gl_check_error(const char* error_text)
{
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
    {
        fprintf(stderr, "GLError: %s (%d)\n", error_text, error);
        exit(EXIT_FAILURE);
    }
}

static char* string_from_file(const char* path)
{
    // Open the file:
    FILE* file = fopen(path , "rb");
    check_error(file != NULL, "Failed to open file.");

    // Seek to the end:
    int success = fseek(file, 0, SEEK_END);
    check_error(success == 0, "Failed to forward file pointer.");

    // Obtain the size of the file from the position of the file pointer:
    long size = ftell(file);
    check_error(size >= 0, "Failed to determine file size.");

    // Rewind the file pointer back to the start:
    rewind(file);

    // Allocate the output buffer:
    char* buffer = malloc(size + 1);
    check_error(buffer != NULL, "Failed to allocate buffer.");

    // Read into it:
    size_t blocks_read = fread(buffer, size, 1, file);
    check_error(blocks_read == 1, "Failed to read file.");

    // Close the file:
    success = fclose(file);
    check_error(success == 0, "Failed to close file.");

    // Append a null-terminator and return the buffer:
    buffer[size] = '\0';

    return buffer;
}

static GLuint compile_shader(GLenum type, const char* shader_path, const char* shader_tag)
{
    // Create an empty shader.
    GLuint shader = glCreateShader(type);
    gl_check_error("glCreateShader");

    // Read the specific shader source.
    char* shader_source = string_from_file(shader_path);

    glShaderSource(shader, 1, (const char**)&shader_source, NULL);
    gl_check_error("glShaderSource");

    free(shader_source);

    // Compile the shader.
    glCompileShader(shader);
    gl_check_error("glCompileShader");

    // Check the compile status.
    GLint success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    gl_check_error("glGetShaderiv");

    if (success)
    {
        return shader;
    }

    // Extract the length of the error message (incl. '\0').
    GLint info_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);

    if (info_length > 1)
    {
        // Extract the error message.
        char* info = malloc(info_length);
        check_error(info != NULL, "Failed to alloc memory for shader error\n");

        glGetShaderInfoLog(shader, info_length, NULL, info);
        gl_check_error("glGetShaderInfoLog");

        fprintf(stderr, "Error compiling shader (%s): %s", shader_tag, info);
        free(info);
    }
    else
    {
        fprintf(stderr, "No info log from the shader compiler :(\n");
    }

    exit(EXIT_FAILURE);
}

static void init_shader_program(user_data_t* user_data)
{
    // Create the vertex and the fragment shader.
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, "../shader/vertex.glsl", "Vertex shader");
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, "../shader/fragment.glsl", "Fragment shader");

    // Create an empty shader program object.
    GLuint shader_program = glCreateProgram();
    gl_check_error("glCreateProgram");

    // Attach both shaders to the program.
    glAttachShader(shader_program, vertex_shader);
    gl_check_error("glAttachShader [vertex]");

    glAttachShader(shader_program, fragment_shader);
    gl_check_error("glAttachShader [fragment]");

    // Link shader program.
    glLinkProgram(shader_program);
    gl_check_error("glLinkProgram");

    // Detach both shaders.
    glDetachShader(shader_program, vertex_shader);
    gl_check_error("glDetachShader [vertex]");

    glDetachShader(shader_program, fragment_shader);
    gl_check_error("glDetachShader [fragment]");

    // Delete them both.
    glDeleteShader(vertex_shader);
    gl_check_error("glDeleteShader [vertex]");

    glDeleteShader(fragment_shader);
    gl_check_error("glDeleteShader [fragment]");

    // Check the link status of the shader program.
    GLint success;

    glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
    gl_check_error("glGetShaderiv");

    if (success)
    {
        // Use the program from now on.
        glUseProgram(shader_program);
        gl_check_error("glUseProgram");

        user_data->shader_program = shader_program;

        return;
    }

    // Extract the length of the error message (incl. '\0').
    GLint info_length;
    glGetShaderiv(shader_program, GL_INFO_LOG_LENGTH, &info_length);

    if (info_length > 1)
    {
        // Extract the error message.
        char* info = malloc(info_length);
        check_error(info != NULL, "Failed to alloc memory for shader program error\n");

        glGetProgramInfoLog(shader_program, info_length, NULL, info);
        gl_check_error("glGetShaderInfoLog");

        fprintf(stderr, "Error linking shader: %s", info);
        free(info);
    }
    else
    {
        fprintf(stderr, "No info log from the shader linker :(\n");
    }

    exit(EXIT_FAILURE);
}

static void init_vertex_data(user_data_t* user_data)
{
    // Vertex data for triangle
    vertex_data_t vertex_data[] =
    {
        { .position = { -1, -1 }, .color = { 0xFF, 0x00, 0x00 } }, // left bottom
        { .position = {  1, -1 }, .color = { 0x00, 0xFF, 0x00 } }, // right bottom
        { .position = {  0,  1 }, .color = { 0x00, 0x00, 0xFF } }  // middle top
    };

    // Create and bind a Vertex Array Object (VAO).
    GLuint vao;

    glGenVertexArrays(1, &vao);
    gl_check_error("glGenVertexArrays");

    glBindVertexArray(vao);
    gl_check_error("glBindVertexArray");

    user_data->vao = vao;

    // Generate and bind a Vertex Buffer Object (VBO).
    GLuint vbo;

    glGenBuffers(1, &vbo);
    gl_check_error("glGenBuffers");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gl_check_error("glBindBuffer");

    // Upload the vertex data to the GPU.
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertex_data_t), (const GLvoid*)vertex_data, GL_STATIC_DRAW);
    gl_check_error("glBufferData");

    // Position attribute
    glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data_t), (GLvoid*)offsetof(vertex_data_t, position));
    gl_check_error("glVertexAttribPointer [position]");

    // Color attribute
    glVertexAttribPointer(ATTRIB_COLOR, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex_data_t), (GLvoid*)offsetof(vertex_data_t, color));
    gl_check_error("glVertexAttribPointer [color]");

    // Enable the attributes.
    glEnableVertexAttribArray(ATTRIB_POSITION);
    gl_check_error("glEnableVertexAttribArray [position]");

    glEnableVertexAttribArray(ATTRIB_COLOR);
    gl_check_error("glEnableVertexAttribArray [position]");

    user_data->vbo = vbo;
}

void init_gl(GLFWwindow* window)
{
    printf("Initializing OpenGL ...\n");
    user_data_t* user_data = glfwGetWindowUserPointer(window);

    // Init the shader program.
    init_shader_program(user_data);

    // Init our vertex data.
    init_vertex_data(user_data);
}

void draw_gl(GLFWwindow* window)
{
    // TODO
}

void teardown_gl(GLFWwindow* window)
{
    printf("Tearing down ...\n");
    user_data_t* user_data = glfwGetWindowUserPointer(window);

    glDeleteProgram(user_data->shader_program);
    gl_check_error("glDeleteProgram");

    glDeleteVertexArrays(1, &user_data->vao);
    gl_check_error("glDeleteVertexArrays");

    glDeleteBuffers(1, &user_data->vbo);
    gl_check_error("glDeleteBuffers");
}
