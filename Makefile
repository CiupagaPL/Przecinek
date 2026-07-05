.PHONY: default win x11

default:
	./build/x11/debug

win:
	cmake -B build/win/ . -D win=ON -DCMAKE_TOOLCHAIN_FILE=mingw-toolchain.cmake
	cmake --build build/win/

x11:
	cmake -B build/x11/ . -D x11=ON
	cmake --build build/x11/