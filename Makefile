#Orginal {,} Made By Ciupaga
#https://github.com/CiupagaPL/Przecinek
#NOTE: You of course can remove top comment from this file :-PP

.PHONY: default win x11 clean

default:
	./build/debug
win: clean
	mkdir -p build/
	cmake -B build/ . -DWIN=ON
	cmake --build build/
x11: clean
	mkdir -p build/
	cmake -B build/ . -DX11=ON
	cmake --build build/
clean:
	rm -rf build/
