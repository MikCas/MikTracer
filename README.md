# MikTracer

A CPU-based raytracer written in C++17.

![Metal spheres render](renders/image.png)

## Features

- **Materials:** Lambertian (diffuse), Metal (with configurable fuzz), Dielectric (glass with refraction)
- **Anti-aliasing:** Multisampling per pixel
- **Depth of field:** Configurable defocus blur
- **Gamma correction**
- **Configurable camera:** FOV, aspect ratio, look-at positioning

## Build & Run

```bash
cmake -B build      # Configure (only need to do this once)
cmake --build build # Compile the project

./build/miktracer   # Runs the executable and outputs the image

rm -rf build        # Clean up the build folder
```

## Acknowledgements

Based on [Ray Tracing in One Weekend](https://raytracing.github.io) by Peter Shirley.
