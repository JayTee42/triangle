# OpenGL Triangle Template

This is a template for rendering a triangle with OpenGL and [GLFW](https://www.glfw.org/). To load the OpenGL library functions, we use [GLAD](https://glad.dav1d.de/).

## Cloning the project

Because the project references GLFW as a git submodule, you need the `--resursive` switch for cloning:
```
git clone --recursive https://github.com/JayTee42/triangle
```

## Creating the project with CMake

From a command prompt in the `triangle` directory:
1. `mkdir build`
1. `cd build`
1. `cmake ..`
1. Either run `make all` or for Visual Studio open `triangle.sln`

## Credits

The CMake template is based on [github.com/juliettef/GLFW-CMake-starter](https://github.com/juliettef/GLFW-CMake-starter). Check the [license](LICENSE.TXT) for details.

More resources to GLFW:

GLFW homepage: [glfw.org](https://www.glfw.org/)  
GLFW on GitHub: [github.com/glfw/glfw](https://github.com/glfw/glfw)
