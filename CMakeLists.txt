cmake_minimum_required(VERSION 3.15)
project(WobblyRunner VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Trouver les packages
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Ajouter les sous-répertoires pour les dépendances (à installer via vcpkg ou git submodules)
# find_package(glfw3 3.3 REQUIRED)
# find_package(glm REQUIRED)

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
)

# Link libraries
target_link_libraries(WobblyRunner PRIVATE 
    OpenGL::GL
    # glfw
    # glm::glm
)

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
message(STATUS "==================================")
message(STATUS "")
message(STATUS "⚠️  N'oublie pas d'installer les dépendances:")
message(STATUS "   - GLFW3 (fenêtre et input)")
message(STATUS "   - GLAD (OpenGL loader)")
message(STATUS "   - GLM (mathématiques)")
message(STATUS "")
message(STATUS "Via vcpkg: vcpkg install glfw3 glad glm")
message(STATUS "==================================")