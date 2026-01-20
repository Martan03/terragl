# terragl

## Table of Contents

- [Prerequisities](#prerequisities)
- [Compilation](#compilation)
- [Usage](#usage)
- [Links](#links)

## Prerequisities

You must have the following libraries installed on you system to compile the
project:

- **GLFW3**: window and input management
- **OpenGL**: graphics API (provided by `mesa` or `nvidia-utils`)
- **GLM**: math library for graphics
- **Freetype**: loading font
- **TBB**: parallelization

## Compilation

Use CMake to generate the build files and compile. This is how to build the
project in release mode for optimal performance:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

## Usage

After you built the project, you can now run the generated executable in the
`build` folder:

```bash
./build/terragl
```

Currently no CLI arguments are available.

## Links

- **Author:** [Martan03](https://github.com/Martan03)
- **GitHub repository:** [terragl](https://github.com/Martan03/terragl)
- **Author website:** [martan03.github.io](https://martan03.github.io)
