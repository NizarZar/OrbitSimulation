cmake_minimum_required(VERSION 3.28)
project(BodyProblem)

# find opengl package
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/glad)
include_directories(include/GLFW)
include_directories(include/glm)
include_directories(include/KHR)

add_executable(BodyProblem
        src/Body.cpp
        src/glad.c
        src/Main.cpp
        src/Shader.cpp)

# load shaders folder
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# load opengl and glfw
target_link_libraries(${PROJECT_NAME} OpenGL::GL ${CMAKE_SOURCE_DIR}/lib/libglfw3.a)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32 ${CMAKE_SOURCE_DIR}/lib/libglfw3.a)
endif()
