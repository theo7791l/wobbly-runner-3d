cmake_minimum_required(VERSION 3.15)
project(WobblyRunner VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Utiliser pkg-config pour trouver les packages
find_package(PkgConfig REQUIRED)

# Trouver OpenGL
find_package(OpenGL REQUIRED)

# Trouver GLEW via pkg-config (plus fiable)
pkg_check_modules(GLEW REQUIRED glew)

# Trouver GLFW via pkg-config
pkg_check_modules(GLFW REQUIRED glfw3)

# GLM est header-only, juste vérifier qu'il existe
find_package(glm QUIET)
if(NOT glm_FOUND)
    # Si pas trouvé via CMake, on suppose qu'il est dans les chemins système
    message(STATUS "GLM trouvé via chemins système")
endif()

# Sources du moteur
set(ENGINE_SOURCES
    engine/physics.cpp
    engine/renderer.cpp
    engine/input.cpp
)

# Sources du jeu
set(GAME_SOURCES
    game/player.cpp
    game/level.cpp
)

# Executable principal
add_executable(WobblyRunner 
    main.cpp
    ${ENGINE_SOURCES}
    ${GAME_SOURCES}
)

# Include directories
target_include_directories(WobblyRunner PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(WobblyRunner PRIVATE 
    OpenGL::GL
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
)

# Link GLM si trouvé via CMake
if(glm_FOUND)
    target_link_libraries(WobblyRunner PRIVATE glm::glm)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(WobblyRunner PRIVATE /W4)
else()
    target_compile_options(WobblyRunner PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation
install(TARGETS WobblyRunner DESTINATION bin)

# Message de configuration
message(STATUS "==================================")
message(STATUS "Wobbly Runner 3D Configuration")
message(STATUS "==================================")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenGL: ${OPENGL_LIBRARIES}")
message(STATUS "GLEW: ${GLEW_LIBRARIES}")
message(STATUS "GLFW: ${GLFW_LIBRARIES}")
message(STATUS "==================================")
