.PHONY: default win x11 clean

# Run compiled script
default:
	./build/debug

# Compile WIN
win: clean
	mkdir -p build/
	cmake -B build/ . -DWIN=ON
	cmake --build build/
	for c in $(wildcard add/*.ttf); do \
		cp $$c build/; \
	done

# Compile X11
x11: clean
	mkdir -p build/
	cmake -B build/ . -DX11=ON
	cmake --build build/

# Remove build/ directory
clean:
	rm -rf build/
