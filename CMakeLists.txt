# |\____/| Example {,} CMakeLists.txt
# |  o o |
cmake_minimum_required(VERSION 3.5)
project(debug LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Create Options
option(WIN "Przecinek Build for Windows" OFF)
option(X11 "Przecinek Build for Linux(X11)" OFF)

set(CODE "example.cpp")
set(HEADERS "head/window.h")

if(WIN)
  # Add Source
  add_executable(${PROJECT_NAME} WIN32 ${CODE} "src/win/window.c" ${HEADERS})

  # Set Compiler Flags
  set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
  set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

  # Target Libraries
  target_link_libraries(${PROJECT_NAME} user32 gdi32 winmm)
elseif(X11)
  # Add Source
  add_executable(${PROJECT_NAME} ${CODE} "src/x11/window.c" ${HEADERS})

  # Check for X11 Package
  find_package(X11 REQUIRED)

  # Set Compiler Flags
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

  # Target Libraries
  target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
else()
  message(FATAL_ERROR "[Error] No Build Tag selected,\n")
endif()

add_compile_options(-Wall -Werror)
