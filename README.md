cmake
libx11-dev
libxrandr-dev
libxinerama-dev
libxcursor-dev
libxi-dev
libgl1-mesa-dev

cmake -S . -B build && cd build && make -j4 && cd .. && ./build/liquid-simulation

git clone https://github.com/glfw/glfw library/glfw

git clone https://github.com/g-truc/glm library/glm

git clone https://github.com/ocornut/imgui library/imgui

https://glad.dav1d.de/

```CMake
cmake_minimum_required(VERSION 3.0)
project(glad)

add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)
```