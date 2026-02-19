.PHONY: default win x11 clean

# Run compiled script
default:
	./build/debug

# Compile WIN
win: clean
	clear
	mkdir -p build/
	cmake -B build/ . -DWIN=ON
	cmake --build build/
	cp -r add/ build/

# Compile X11
x11: clean
	clear
	mkdir -p build/
	cmake -B build/ . -DX11=ON
	cmake --build build/

# Remove build/ directory
clean:
	rm -rf build/
