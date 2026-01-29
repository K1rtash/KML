# KML - Kirtash's Multimedia Library

**KML** is a lightweight, cross-platform, multi-purpose C++ library designed to make game development faster and easier. It abstracts common tasks like window management, input handling, rendering, and basic mathematics, allowing developers to focus on building games rather than boilerplate low level coding.

---

## Features

- **Window & Context Management** using GLFW.
- **OpenGL Rendering** wrappers for uniform setting and abstraction for textures, shapes, and shaders creation.
- **Math Utilities**: Vector types, color conversions (RGB/HSV), clamping.
- **Cross-Platform**: Windows & Linux (to-do) MacOS
- **Plug & Play**: Easily integrated into other projects using CMake's FetchContent

---

All dependencies are automatically handled using CMake `FetchContent`.

---

## Building KML

```bash
git clone https://github.com/K1rtash/KML
cd KML
cmake -S . -B build 
cmake --build build
```
The following CMake options are used in this proyect
`KML_BUILD_DEMO` OFF
`KML_SHARED_LIBS` OFF

You may also use CMake presets