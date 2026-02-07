cmake_minimum_required(VERSION 3.20)
project(Froxgel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Add executable AND compile glad.c with it
add_executable(Froxgel
        main.cpp
        external/glad/src/glad.c
        window.h
        shader.h
        mesh.h
        helpers.cpp
        helpers.h
        camera.h
        controls.cpp
        controls.h
        camera.cpp
        window.cpp
        polygons.cpp
        polygons.h
        lighting.cpp
        lighting.h
        skybox.cpp
        skybox.h
        stb_image.h
        filesystem.h
        mesh.cpp
        generator.h
        generator.cpp
        marching_cubes.cpp
        marching_cubes.h
        marching_cubes.cpp
)
set_source_files_properties(external/glad/src/glad.c PROPERTIES LANGUAGE CXX)
# Include directories
target_include_directories(Froxgel
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# Link libraries
target_link_libraries(Froxgel
        PRIVATE
        glfw
        OpenGL::GL
)