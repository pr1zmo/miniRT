# miniRT

miniRT is a small ray tracer written in C using MinilibX. It reads a `.rt` scene description, renders the scene in a window, and lets you toggle anti-aliasing while the image is displayed.

## Features

- Parses `.rt` scene files
- Renders ambient light, cameras, and multiple light sources
- Supports spheres, planes, cylinders, and triangles
- Uses MinilibX for window management and image output
- Renders with multiple threads

## Requirements

This project targets Linux and depends on MinilibX and X11 libraries.

You need:

- `cc`
- `make`
- X11 development libraries
- MinilibX available in the `mlx/` folder

The project also includes a local `libft/` submodule-style directory that is built automatically by the main Makefile.

## Build

From the project root:

```bash
make
```

Useful extra targets:

```bash
make clean
make fclean
make re
make sanitize
make valgrind
```

The `valgrind` target runs the program with `scenes/mandatory.rt`.

## Run

The executable expects exactly one argument: a `.rt` file.

```bash
./miniRT scenes/mandatory.rt
```

If the argument count is wrong or the file does not end in `.rt`, the program exits with an error.

## Controls

- `ESC` exits the program
- `Space` toggles anti-aliasing and redraws the scene

## Scene File Format

Scene files are plain text. Each non-empty line starts with an identifier followed by values separated by spaces.

Supported entries include:

- `A` ambient light
- `C` camera
- `L` light source
- `sp` sphere
- `pl` plane
- `cy` cylinder
- `tr` triangle

Example:

```text
A 0.3 255,255,255
C 50,10,20 0,0,1 20
L 70,100,20 0.8 255,250,250
sp 50,10,100 10 255,0,0
pl 0,-10,0 0,-1,0 120,20,60
cy 5,0,90 0,1,0 4.2 21.42 0,255,255
```

For more complete examples, see the files in `scenes/`.

## Notes

- The main render resolution is fixed in `minirt.h`
- The parser is strict about scene syntax and ranges
- `scenes/mandatory.rt` is a good starting point for testing changes
