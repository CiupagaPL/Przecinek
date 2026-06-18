![Przecinek](https://github.com/CiupagaPL/CiupagaPL/blob/main/src/Przecinek.png)
# {,} Przecinek -> [[stable]](https://github.com/CiupagaPL/Przecinek/tree/stable) [[unstable]](https://github.com/CiupagaPL/Przecinek/tree/unstable)

### General

**Przecinek is a minimal multimedia library for Windows(WIN) and Linux(X11)** being developed in *C*.
It allows you to create window applications that are fast, optimized, minimal and easy to read.
Przecinek uses a minimal number of libraries, focusing primarily on built-in and OpenGL ones.<br>
It's under active development.
When everything on the TODO list is completed, Przecinek will enter the **Beta** stage.
By then, the documentation will also be finished.
*In the future I also hope to implement Wayland support*.

### Stable Features

| Feature                        | Version    |
|--------------------------------|------------|
| Creating and managing windows  | **[a1,0]** |
| Global input system            | **[a1,0]** |
| Window update system           | **[a2,0]** |
| Window frame control system    | **[a3,0]** |
| Complex render system          | **[a4,0]** |
| Creating and rendering fonts   | **[a4,0]** |
| Creating complex objects       | **[a5,0]** |
| Checking complex collisions    | **[a5,0]** |

### Unstable Features / TODO

| Feature                      | WIN | X11 |
|------------------------------|-----|-----|
| Loading images               | ✅  | ✅  |
| Rendering images             | ✅  | ✅  |
| Loading audio                | ✅  | ❌  |
| Managing audio               | ✅  | ❌  |
| Playing audio                | ✅  | ❌  |
| Threads support              | ✅  | ❌  |
| Creating custom shapes       | ✅  | ❌  |
| Rendering color gradients    | ❌  | ❌  |
| More window options          | ❌  | ❌  |
| Fully polished window resize | ❌  | ❌  |
| Final code standard          | ✅  | ❌  |

`*` > Unfinished Features

### Used libraries

| Library                                                                                    | In      | Purpose                                                         |
|--------------------------------------------------------------------------------------------|---------|-----------------------------------------------------------------|
| [WIN API](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list)       | **WIN** | Managing Windows session, Creating windows and managing events  |
| [GDI+ Flat](https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-flatapi-flat) | **WIN** | Loading fonts and images                                        |
| [CoreAudio API](https://learn.microsoft.com/en-us/windows/win32/api/_coreaudio)            | **WIN** | Loading, managing and playing sounds                            |
| [GL/GLU](https://learn.microsoft.com/en-us/windows/win32/opengl/opengl-reference)          | **WIN** | Rendering complex graphics                                      |
| [X11 LIB](https://x.org/releases/current/doc/libX11/libX11/libX11.html)                    | **X11** | Managing X11 session, Creating windows and managing events      |
| [FreeType2](https://freetype.org/freetype2/docs/documentation.html)                        | **X11** | Loading fonts                                                   |
| [PNG LIB](https://www.libpng.org/pub/png/pngdocs.html)                                     | **X11** | Loading .png images                                             |
| [JPEG TURBO LIB](https://libjpeg-turbo.org/)                                               | **X11** | Loading .jpeg images                                            |
| [GL/GLX](https://registry.khronos.org/OpenGL/specs/gl/glx1.4.pdf)                          | **X11** | Rendering complex graphics                                      |

### Version

> Newest Version (Alpha): **[None]**;<br>
> Newest Version (Unstable): **[u,22]**;<br>
> Last Updated: **18-06-2026**;

### LICENSE

```
Copyright(C) 2025-2026 Ciupaga
{,} PRZECINEK
Custom License

By obtaining this software, you are allowed to use it for any
project you create and to fork it.

1. Contributors are encouraged to submit improvements or modifications
   to the original project.
2. For programs that are made in this software you don't need to
   mention that you have used it.
3. You may fork and publish the source code as your own, but you must
   retain the original author, license, and website information
   located in the comments.
4. Consequences of Non-Compliance:
a) If you fail to comply with any of the terms outlined in this license,
   you may lose the rights to use, modify, and distribute this software.
b) The author reserves the right to take legal action against any party
   that does not adhere to the conditions of this license.
5.:
a) "Fork" refers to creating a derivative work based on the
   original software.
b) "Comments" refer to messages located in the source code.

This software is provided "as-is" without any warranties. The author
is not liable for any damages arising from its use.
```
