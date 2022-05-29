cmake_minimum_required(VERSION 3.1)

project(triangle)

set(TRIANGLE_INCLUDE_DIR include)
set(TRIANGLE_SRC_DIR src)
set(TRIANGLE_LIB_DIR lib)
set(TRIANGLE_SHADER_DIR shader)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
add_subdirectory( ${TRIANGLE_LIB_DIR}/glfw)

if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

add_executable(triangle WIN32
    ${TRIANGLE_SRC_DIR}/glad.c
    ${TRIANGLE_INCLUDE_DIR}/gl_calls.h
    ${TRIANGLE_SRC_DIR}/gl_calls.c
    ${TRIANGLE_SRC_DIR}/main.c
    ${TRIANGLE_SHADER_DIR}/vertex.glsl
    ${TRIANGLE_SHADER_DIR}/fragment.glsl
)

target_include_directories(triangle PRIVATE ${TRIANGLE_INCLUDE_DIR})
target_link_libraries(triangle ${OPENGL_LIBRARIES} glfw)

if (NOT WIN32)
    target_link_libraries(triangle m)
ENDIF()

if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'triangle' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT triangle)
    endif()
endif()
