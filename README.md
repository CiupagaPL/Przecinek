![Przecinek](https://github.com/CiupagaPL/CiupagaPL/blob/main/src/Przecinek.png)
# {,} Przecinek -> [[stable]](https://github.com/CiupagaPL/Przecinek/tree/stable) [[unstable]](https://github.com/CiupagaPL/Przecinek/tree/unstable)

### General

**Przecinek is a minimal multimedia library** for Windows and Linux being developed in C.
Its purpose is to make it easier for game/app developers to create window applications while keeping their projects small in size and easy to read.
Przecinek uses minimal amount of libraries.
It's under active development.

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

| Feature                     | WIN | X11 |
|-----------------------------|-----|-----|
| Loading images              | ❌  | ❌  |
| Rendering images            | ❌  | ❌  |
| Loading audio               | ❌  | ❌  |
| Managing audio              | ❌  | ❌  |
| Playing audio               | ❌  | ❌  |

`*` > Unfinished Features

### Used libraries

| Library                                           | In       | Purpose                                                         |
|---------------------------------------------------|----------|-----------------------------------------------------------------|
| WIN API                                           | **WIN**  | Managing Windows session, Creating windows and managing events  |
| GDI+ Flat                                         | **WIN**  | Loading fonts, Rendering complex graphics                       |
| [X11 lib](https://github.com/mirror/libX11)       | **X11**  | Managing X11 session, Creating windows and managing events      |
| [FreeType2](https://github.com/freetype/freetype) | **X11**  | Loading fonts                                                   |
| [Cairo](https://github.com/s-u/Cairo)             | **X11**  | Rendering complex graphics                                      |

### External

You can find all the information about managing, configuring and setting up Przecinek in the **Wiki**.
Start using it by clicking [[this]](https://github.com/CiupagaPL/Przecinek/wiki) link.<br>

### Version

> Newest Version (Alpha): **[a5,0]**;<br>
> Newest Version (Unstable): **[u,17]**;<br>
> Last Updated: **08-02-2026**;

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
4.:
a) "Fork" refers to creating a derivative work based on the
original software.
b) "Comments" refer to any messages in the source code.

This software is provided "as-is" without any warranties. The author
is not liable for any damages arising from its use.
```
