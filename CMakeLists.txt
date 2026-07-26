#[[*
*       |\____/| example cmake file
*       |      | used for compiling
*  __   | .  . |
* /  \__\ =, = / not covered by the license!!
* \__         /  modify it how you want!!
*    \ \____\ \
*    {,{,} {,},}
*]]

cmake_minimum_required(VERSION 3.5)

if(POLICY CMP0069)
	cmake_policy(SET CMP0069 NEW)
endif()
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

project(debug VERSION 0.0.0 LANGUAGES C CXX)

set(CODE "debug.c")
set(HEADERS "")

set(PRZECINEK "przecinek.h")
set(PRZECINEK_WIN "src/win.c")
set(PRZECINEK_X11 "src/x11.c")

if(UNIX)
	add_executable(
		${PROJECT_NAME} ${PRZECINEK} ${PRZECINEK_X11}
		${CODE} ${HEADERS}
	)

	set_target_properties(${PROJECT_NAME} PROPERTIES
		C_STANDARD 23
		C_STANDARD_REQUIRED YES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
	)

	find_package(X11 REQUIRED)
	find_package(Freetype REQUIRED)
	find_package(OpenGL REQUIRED)

	target_link_libraries(
		${PROJECT_NAME} PRIVATE
		Freetype::Freetype OpenGL::GL X11::X11
		m png jpeg
	)

	target_compile_options(${PROJECT_NAME} PRIVATE -O3)
	target_link_options(${PROJECT_NAME} PRIVATE -flto -static-libgcc -static-libstdc++)

	add_compile_options(-Wall -Werror)
else()
	add_executable(
		${PROJECT_NAME} ${PRZECINEK} ${PRZECINEK_WIN}
		${CODE} ${HEADERS}
	)

	set_target_properties(${PROJECT_NAME} PROPERTIES
		C_STANDARD 23
		C_STANDARD_REQUIRED YES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
	)

	if(MSVC)
		target_compile_options(${PROJECT_NAME} PRIVATE /O2)
	else()
		target_compile_options(${PROJECT_NAME} PRIVATE -O3)

		include(CheckIPOSupported)
		check_ipo_supported(RESULT IPO_SUPPORTED)

		if(IPO_SUPPORTED)
			set_property(
				TARGET ${PROJECT_NAME}
				PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
			)
		endif()
	endif()

	set(OpenGL_GL_PREFERENCE LEGACY)
	find_package(OpenGL REQUIRED)
	target_link_libraries(
		${PROJECT_NAME} PRIVATE
		OpenGL::GL
		winmm gdiplus
	)

	if(MINGW)
		target_link_options(
			${PROJECT_NAME} PRIVATE
			-static -static-libgcc -static-libstdc++
		)
	endif()

	if(MSVC)
		add_compile_options(/W4 /WX)
	else()
		add_compile_options(-Wall -Werror)
	endif()
endif()