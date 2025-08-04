
cmake_minimum_required(VERSION 3.18)
project(imgui-sfml-combined VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Buscar SFML 3
find_package(SFML 3 COMPONENTS graphics window system REQUIRED)

# --- ImGui core ---
set(IMGUI_CORE_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    imgui_demo.cpp
)

add_library(imgui STATIC ${IMGUI_CORE_SOURCES})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# silenciamiento de warnings si se desea podría agregarse aquí

# --- ImGui-SFML binding ---
set(IMGUI_SFML_SOURCES
    imgui-SFML.cpp
)

add_library(imgui-sfml STATIC ${IMGUI_SFML_SOURCES})
target_include_directories(imgui-sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui-sfml
    PRIVATE
      imgui
      sfml-graphics
      sfml-window
      sfml-system
)

# Export both for parent projects
