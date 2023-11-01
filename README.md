## Build process

First you need to install all the necessary system headers and building tools:

```Bash
sudo apt install cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev -y
```

Then you need to clone all required external libraries:

```Bash
git clone https://github.com/glfw/glfw library/glfw
git clone https://github.com/g-truc/glm library/glm
git clone https://github.com/ocornut/imgui library/imgui
```

You will also need generated glad files (link between C++ code and OpenGL headers). You can download glad `zip` package file [here](https://glad.dav1d.de/) (C++ language,OpenGL specification,gl version 4.6+). Unpack it and move it's contents into `library/glad` folder. Save the following declarations into `library/glad/CMakeLists.txt`:

```CMake
cmake_minimum_required(VERSION 3.0)
project(glad)
add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)
```

Now the entire code environment is well configured and ready for build process:

```Bash
cmake -S . -B build
cd build
make -j4
```

After successful compilation, you can start simulator with:

```Bash
./build/liquid-simulation
```