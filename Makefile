# |\____/| Example {,} Makefile
# |  o o |
.PHONY: default win x11 clean

# Run Compiled Script
default:
	./build/debug

# Compile WIN
win: clean
	mkdir -p build/
	cmake -B build/ . -DWIN=ON
	cmake --build build/

# Compile X11
x11: clean
	mkdir -p build/
	cmake -B build/ . -DX11=ON
	cmake --build build/

# Remove build/ Directory
clean:
	rm -rf build/
