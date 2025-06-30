#pragma once

/**
 * @file Prerequisites.h
 * @brief Includes common libraries, defines macros for error handling, and declares shared enums.
 */

 // === Standard Libraries ===
#include <iostream>     ///< Input/output stream.
#include <string>       ///< std::string handling.
#include <sstream>      ///< String stream for constructing messages.
#include <vector>       ///< Dynamic array container.
#include <thread>       ///< Multi-threading support.
#include <map>          ///< Sorted associative container.
#include <fstream>      ///< File input/output.
#include <unordered_map>///< Hash table-based associative container.

#include <Memory/TSharedPointer.h>
#include <Memory/TStaticPtr.h>
#include <Memory/TUniquePtr.h>



// === Third Party Libraries ===
#include <SFML/Graphics.hpp> ///< SFML graphics module.

// === ImGui ===
// Add ImGui headers here if used in the future

// === Macros ===

/**
 * @brief Safely deletes a pointer and sets it to nullptr to avoid dangling references.
 */
#define SAFE_PTR_RELEASE(x) if(x != nullptr) { delete x; x = nullptr; }

 /**
  * @brief Logs a resource creation message to standard error.
  *
  * @param classObj Name of the class.
  * @param method Name of the method.
  * @param state Message indicating resource state.
  */
#define MESSAGE(classObj, method, state)                      \
{                                                             \
    std::ostringstream os_;                                   \
    os_ << classObj << "::" << method << " : "                \
        << "[CREATION OF RESOURCE" << ": " << state "] \n";   \
    std::cerr << os_.str();                                   \
}

  /**
   * @brief Logs an error message and terminates the program.
   *
   * @param classObj Name of the class.
   * @param method Name of the method.
   * @param errorMSG Description of the error.
   */
#define ERROR(classObj, method, errorMSG)                         \
{                                                                 \
    std::ostringstream os_;                                       \
    os_ << "ERROR : " << classObj << "::" << method << " : "      \
        << "  Error in data from params [" << errorMSG"] \n";     \
    std::cerr << os_.str();                                       \
    exit(1);                                                      \
}

   // === Enumerations ===

   /**
    * @enum ShapeType
    * @brief Types of shapes that can be created in the system.
    */
enum
  ShapeType {
  EMPTY = 0,    ///< No shape.
  CIRCLE = 1,   ///< Circle shape.
  RECTANGLE = 2,///< Rectangle shape.
  TRIANGLE = 3, ///< Triangle shape using a convex polygon.
  POLYGON = 4   ///< General polygon with 5 or more points.
};