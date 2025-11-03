# |\____/| Example {,} CMakeLists.txt
# |  o o |
cmake_minimum_required(VERSION 3.5)
project(debug LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Create options
option(WIN "Przecinek Build for Windows" OFF)
option(X11 "Przecinek Build for Linux(X11)" OFF)

# Select all source files
set(CODE "main.c")
set(HEADERS "przecinek.h")

if(WIN)
  # Set WIN location
  add_executable(${PROJECT_NAME} WIN32 ${CODE} "src/win.c" ${HEADERS})

  # Set compiler flags
  set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
  set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

  # Set target libraries
  target_link_libraries(${PROJECT_NAME} user32 gdi32 winmm)
elseif(X11)
  # Set X11 location
  add_executable(${PROJECT_NAME} ${CODE} "src/x11.c" ${HEADERS})

  # Check for packages
  find_package(X11 REQUIRED)

  # Set compiler flags
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

  # Set target libraries
  include_directories(/usr/include/freetype2  -I/usr/local/include/freetype2  -I/usr/include/libpng16)
  target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} Xft freetype)
else()
  message(FATAL_ERROR "[pError, C001] \"No build Tag selected\",\n")
endif()

# Set compile options
add_compile_options(-Wall -Werror)
