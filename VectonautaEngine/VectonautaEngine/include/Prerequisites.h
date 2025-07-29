#pragma once

/**
 * @file Prerequisites.h
 * @brief Includes standard libraries, project utilities, ImGui and SFML headers, and defines common macros.
 */

 // === Standard Libraries ===
#include <iostream>     // std::cerr, std::cout
#include <string>       // std::string
#include <sstream>      // std::ostringstream
#include <vector>       // std::vector
#include <thread>       // std::thread
#include <map>          // std::map
#include <fstream>      // std::ifstream, std::ofstream
#include <unordered_map>// std::unordered_map

// === Engine Utilities ===
#include "Memory/TSharedPointer.h"  // EngineUtilities::TSharedPointer
#include "Memory/TStaticPtr.h"     // EngineUtilities::TStaticPtr (if used)
#include "Memory/TUniquePtr.h"     // EngineUtilities::TUniquePtr

// === ImGui ===
#include <imgui.h>
#include <imgui-SFML.h>          // ImGui SFML binding

// === SFML ===
#include <SFML/Graphics.hpp>      // SFML graphics module (RenderWindow, Drawable, etc.)

// === Macros ===

/// Safely deletes a raw pointer and nulls it
#define SAFE_PTR_RELEASE(x) if ((x) != nullptr) { delete (x); (x) = nullptr; }

/// Logs a resource creation message
#define MESSAGE(classObj, method, state)                      \
    do {                                                      \
        std::ostringstream os_;                              \
        os_ << classObj << "::" << method                   \
            << " : [CREATION OF RESOURCE: " << state << "]"; \
        std::cerr << os_.str();                               \
    } while (0)

  /// Logs an error message and exits
#define ERROR(classObj, method, errorMSG)                             \
    do {                                                             \
        std::ostringstream os_;                                      \
        os_ << "ERROR: " << classObj << "::" << method            \
            << " : Error in data from params [" << errorMSG << "]"; \
        std::cerr << os_.str();                                      \
        std::exit(1);                                                \
    } while (0)

  // === Enumerations ===
  enum ShapeType {
  EMPTY = 0,  ///< No shape
  CIRCLE = 1,  ///< Circle
  RECTANGLE = 2,  ///< Rectangle
  TRIANGLE = 3,  ///< Triangle (ConvexShape)
  POLYGON = 4   ///< Polygon (>=5 points)
};
