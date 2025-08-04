
cmake_minimum_required(VERSION 3.18)
project(VectonautaEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Buscar SFML 3
find_package(SFML 3 COMPONENTS graphics window system REQUIRED)

# --- ImGui / ImGui-SFML ---
add_subdirectory(imgui-master)
add_subdirectory(imgui-sfml-master)

# Includes principales
target_include_directories(${PROJECT_NAME} 
    PRIVATE
      ${CMAKE_SOURCE_DIR}/VectonautaEngine
      ${CMAKE_SOURCE_DIR}/VectonautaEngine/ECS
      ${CMAKE_SOURCE_DIR}/VectonautaEngine/imgui
      ${CMAKE_SOURCE_DIR}/VectonautaEngine/Memory
)

# Fuentes del motor
file(GLOB_RECURSE VECTO_SOURCES
    "${CMAKE_SOURCE_DIR}/VectonautaEngine/*.cpp"
)

add_executable(${PROJECT_NAME} ${VECTO_SOURCES})

# Vincular librerías
target_link_libraries(${PROJECT_NAME}
    PRIVATE
      sfml-graphics
      sfml-window
      sfml-system
      imgui
      imgui-sfml
)

# Salida clara
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
