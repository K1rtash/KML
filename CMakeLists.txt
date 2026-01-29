cmake_minimum_required(VERSION 3.28)
project(KML VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Build type por defecto ---
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(KML_COPY_ASSETS "Copia la carpeta assets dentro de build" OFF)
option(KML_BUILD_SHARED "Build KML as shared library" OFF)
option(KML_STATIC_RUNTIME "Static runtime for KML" OFF)

# GLFW
set(GLFW_BUILD_EXAMPLES "OFF")
set(GLFW_BUILD_TESTS "OFF")
set(GLFW_BUILD_DOCS "OFF")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0
)
FetchContent_MakeAvailable(glm)

#FMT
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281
) # 10.2.1
FetchContent_MakeAvailable(fmt)

#OPENGL
find_package(OpenGL REQUIRED)

# --- Archivos fuente ---
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)

add_library(${PROJECT_NAME}
    ${SRC_FILES}
    ${CMAKE_SOURCE_DIR}/ext/glad/glad.c
)

add_library(KML::KML ALIAS KML)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ext
        ${glfw_SOURCE_DIR}/include
        ${fmt_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    fmt::fmt
    OpenGL::GL
)

# --- Copiar assets ---
if(KML_COPY_ASSETS)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
endif()

if(KML_STATIC_RUNTIME)
target_link_options(${PROJECT_NAME} PRIVATE
    -static-libstdc++
    -static-libgcc
)
endif()