
cmake_minimum_required(VERSION 3.18)
project(imgui VERSION 1.0 LANGUAGES CXX)

# ImGui is header-only plus the implementation .cpp files
set(IMGUI_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    # Optional: imgui_demo.cpp if you want the demo window
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Recommended: disable warnings from ImGui if needed
