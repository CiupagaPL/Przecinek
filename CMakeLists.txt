cmake_minimum_required(VERSION 3.5)
project(debug LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Create options
option(WIN "Przecinek for WIN" OFF)
option(X11 "Przecinek for X11" OFF)

# Select source files
set(CODE "main.c")
set(HEADERS "przecinek.h")

if(X11)
  # Check for packages
  find_package(X11 REQUIRED)
  find_package(Freetype REQUIRED)

  # Set X11 location
  add_executable(${PROJECT_NAME} ${CODE} "src/x11.c" ${HEADERS})

  # Set compiler flags
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

  # Set target libraries
  include_directories(${X11_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} Xft Xrender ${FREETYPE_LIBRARIES})
else()
  # Set WIN location
  add_executable(${PROJECT_NAME} WIN32 ${CODE} "src/win.c" ${HEADERS})

  # Set compiler flags
  set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static")

  set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

  # Set target libraries
  target_link_libraries(${PROJECT_NAME} user32 gdi32 msimg32 winmm gdiplus)
endif()

# Set compile options
target_compile_options(${PROJECT_NAME} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
add_compile_options(-Wall -Werror)
