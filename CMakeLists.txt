cmake_minimum_required(VERSION 3.16)
project(LagrangeInterpolationPolynomial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE=ON)

# Try SFML 3 first (preferred on many recent distros), then fall back to SFML 2.5+
set(_sfml_components Graphics Window System)
find_package(SFML 3 QUIET COMPONENTS ${_sfml_components})
if (NOT SFML_FOUND)
    set(_sfml_components graphics window system)
    find_package(SFML 2.5 QUIET COMPONENTS ${_sfml_components})
endif()
if (NOT SFML_FOUND)
    message(FATAL_ERROR "SFML not found. Install SFML 3.x or 2.5+ and set SFML_DIR if needed.")
endif()

add_executable(LagrangeInterpolationPolynomial
    src/main.cpp
    src/Interpolator.cpp
    src/Canvas.cpp
    include/Canvas.hpp
    include/Interpolator.hpp
)

# Use an absolute include path to avoid generator quirks with relative paths
target_include_directories(LagrangeInterpolationPolynomial PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link depending on the SFML major version that was found
if (TARGET SFML::Graphics)
    target_link_libraries(LagrangeInterpolationPolynomial PRIVATE SFML::Graphics SFML::Window SFML::System)
else()
    target_link_libraries(LagrangeInterpolationPolynomial PRIVATE sfml-graphics sfml-window sfml-system)
endif()
