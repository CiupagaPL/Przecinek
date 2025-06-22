#Orginal {,} Made By Ciupaga
#https://github.com/CiupagaPL/Przecinek
#NOTE: You can remove top comment from this file :-PP

cmake_minimum_required(VERSION 3.5)
project(debug LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

option(WIN "Przecinek for Windows" OFF)
option(X11 "Przecinek for Linux" OFF)

set(CODE "example.cpp")

set(HEADERS "src/window.h")

if(WIN)
  add_executable(${PROJECT_NAME} WIN32 ${CODE} "src/windows/window.c" ${HEADERS})
  set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
  set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
  target_link_libraries(${PROJECT_NAME} user32 gdi32)
elseif(X11)
  add_executable(${PROJECT_NAME} ${CODE} "src/x11/window.c" ${HEADERS})
  find_package(X11 REQUIRED)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
  target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
else()
  message(FATAL_ERROR "[Error] No Build Tag selected,")
endif()

add_compile_options(-Wall -Werror)
