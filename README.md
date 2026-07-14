![Przecinek](https://github.com/ciupagacz/ciupagacz/blob/main/src/Przecinek.png)

<div align="center">
    branch:
    <a href="https://github.com/ciupagacz/przecinek/tree/stable">[stable]</a>
    <a href="https://github.com/ciupagacz/przecinek/tree/unstable">[unstable]</a>
</div>

### About the project

**Przecinek is a small multimedia library for Windows and Linux.**
Simply put - it's a collection of functions and structures that allows you to create windows, load fonts and images, draw objects and text and play audio. Originally, it was created for C, but it can also be used with C++.<br>
You can read more about it on the [wiki page](https://ciupagacz.github.io/przecinek/).

### Current features

Most of the planned features have already been implemented. I still plan to add and modify some of the existing ones. I aim to finalize most of the stuff with the release of the first Beta. Until then, some function names and features might change between Przecinek versions.

### Planned stuff list

| planned feature | win | x11 |
|-----------------|-----|-----|
| loading / managing / playing audio            | ✅ | ❌ |
| auto-repeating audio                          | ❌ | ❌ |
| filtering textures                            | ? | ? |
| blending colors when rendering stuff          | ❌ | ❌ |
| generating texts with diffrent options        | ❌ | ❌ |
| setting window icon                           | ❌ | ❌ |
| checking vsync value                          | ❌ | ❌ |
| limiting window frames more efficiently       | ❌ | ❌ |
| rendering default texture when failed to load | ❌ | ❌ |
| managing window resize                        | ❌ | ❌ |
| generating lines                              | ❌ | ❌ |
| compiling projects on all operating systems   | ✅ | ✅ |

`?` -  unfinished feature

### Used 3rd party libraries

| Library | In |
|---------|----|
| [WIN api](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list)           | *win* |
| [GDI+ flat (C)](https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-flatapi-flat) | *win* |
| [GLU](https://learn.microsoft.com/en-us/windows/win32/opengl/opengl-reference)                 | *win* |
| [CoreAudio api](https://learn.microsoft.com/en-us/windows/win32/api/_coreaudio)                | *win* |
| [X11 library](https://x.org/releases/current/doc/libX11/libX11/libX11.html)                    | *x11* |
| [GLX](https://registry.khronos.org/OpenGL/specs/gl/glx1.4.pdf)                                 | *x11* |
| [FreeType2](https://freetype.org/freetype2/docs/documentation.html)                            | *x11* |
| [PNG library](https://www.libpng.org/pub/png/pngdocs.html)                                     | *x11* |
| [JPEG turbo library](https://libjpeg-turbo.org/)                                               | *x11* |

### Current branch version

> Newest stable version: **a6,1d**<br>
> Newest unstable version: **u,27**<br>
> Last updated: **14-07-2026**

### Project license

```
Copyright(C) 2025-2026 ciupaga
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