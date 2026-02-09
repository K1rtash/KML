# ---------------------      ---------------------
#                       KML
#           Kirtash's Multimedia Library
#               (c) David Sánchez 2026
# ---------------------      ---------------------
cmake_minimum_required(VERSION 3.28)
project(KML VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---------------------      ---------------------
#                       OPTS
# ---------------------      ---------------------
option(KML_SHARED_LIBS "Build KML as shared library" OFF)
option(KML_BUILD_DEMO "Build a demo" OFF)

if(KML_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()
# ---------------------      ---------------------
#                       DEPS
# ---------------------      ---------------------

# --------------------- GLFW ---------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)
# --------------------- GLM  ---------------------
set(GLM_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0
)
FetchContent_MakeAvailable(glm)
# ------------------- OPENGL --------------------
find_package(OpenGL REQUIRED)
# ------------------ FREETYPE -------------------
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG   ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  freeType
  GIT_REPOSITORY https://github.com/freetype/freetype/
  GIT_TAG        VER-2-13-2
)
FetchContent_MakeAvailable(freeType)
# -----------------------------------------------

# ---------------------      --------------------
#                       SRC
# ---------------------      --------------------
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)

add_library(${PROJECT_NAME}
    ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/glad.c
)

add_library(KML::KML ALIAS KML)


# ---------------------      ---------------------
#                       ABI
# ---------------------      ---------------------
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ext
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm::glm
    OpenGL::GL
    freetype
)

# ---------------------      ---------------------
#                    DLL EXPORT
# ---------------------      ---------------------
if(KML_SHARED_LIBS)
    # Cuando compilamos KML, definimos KML_BUILD_DLL
    target_compile_definitions(${PROJECT_NAME} PRIVATE KML_SHARED_LIBS KML_BUILD_DLL)
    # Para consumidores externos solo KML_SHARED_LIBS
    target_compile_definitions(${PROJECT_NAME} INTERFACE KML_SHARED_LIBS)
endif()

# ---------------------      ---------------------
#                       DEMO
# ---------------------      ---------------------
if(KML_BUILD_DEMO)
    add_executable(${PROJECT_NAME}demo demo/Main.cxx)
    target_link_libraries(${PROJECT_NAME}demo PRIVATE KML::KML)
    target_include_directories(${PROJECT_NAME}demo PRIVATE include)

        add_executable(${PROJECT_NAME}test demo/Test.cxx)
    target_link_libraries(${PROJECT_NAME}test PRIVATE KML::KML)
    target_include_directories(${PROJECT_NAME}test PRIVATE include)
endif()

# --- Copiar assets ---
#if(KML_COPY_ASSETS)
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#            ${CMAKE_CURRENT_SOURCE_DIR}/assets
#            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
#)
#endif()

#if(KML_STATIC_RUNTIME)
#target_link_options(${PROJECT_NAME} PRIVATE
#    -static-libstdc++
#    -static-libgcc
#)
#endif()