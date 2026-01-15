cmake_minimum_required(VERSION 3.21)
project(KML LANGUAGES C CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_DIR "" CACHE PATH "Path to GLFW")

option(KML_COPY_ASSETS "Copia la carpeta assets dentro de build" OFF)

# Para que funcione find pkg
list(APPEND CMAKE_PREFIX_PATH ${GLFW_DIR})

find_package(glfw3 REQUIRED CONFIG)
find_package(OpenGL REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third-party/glad/include 
    ${PROJECT_SOURCE_DIR}/third-party/glm/include 
    ${PROJECT_SOURCE_DIR}/third-party/stb/include 
)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)

add_executable(${PROJECT_NAME}
    ${SRC_FILES} 
    ${CMAKE_SOURCE_DIR}/third-party/stb/src/stb_image.c
    ${CMAKE_SOURCE_DIR}/third-party/glad/src/glad.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third-party/glad/include 
    ${PROJECT_SOURCE_DIR}/third-party/glm/include 
    ${PROJECT_SOURCE_DIR}/third-party/stb 
    ${GLFW_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw 
    OpenGL::GL
)

if(KML_COPY_ASSETS)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()