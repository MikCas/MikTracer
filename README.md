# MikTracer

A CPU path tracer written in C++17, designed for performance and usability. Features include progressive sampling, depth of field, physically based materials (glass, metal, diffuse), and a modular scene composition system. The current roadmap focuses on scaling this foundation into a highly parallelisable rendering engine.

![Three spheres hero render](renders/three_spheres.png)

## Quick render

```bash
./render.sh three_spheres            # build + render
./render.sh three_spheres --open     # also opens the PNG
```

`render.sh` configures CMake on first run, builds only the requested scene target, runs it, and writes the PNG to `renders/<scene>.png`.

## Scenes

| Scene             | Showcases                                                                  |
| ----------------- | -------------------------------------------------------------------------- |
| `three_spheres`   | Hero shot — diffuse, glass, polished metal side-by-side                    |
| `sphere_grid`     | Stress test — many metal spheres with varied fuzz                          |
| `cornell_spheres` | Depth-of-field — receding spheres with progressive defocus blur            |

Renders live under `renders/`.

## Architecture

- Scene (Scene.h): The world. A pure data container for objects.
- Camera (Camera.h): View geometry. Generates rays, zero rendering logic.
- Renderer (Renderer.h): The path tracer. Takes a Scene and Camera, writes pixels.

`SceneRunner.h`'s `runScene()` wires the three together so individual scene files stay short.

## Adding a new scene

Create `scenes/my_scene.cpp`:

```cpp
#include "Camera.h"
#include "Material.h"
#include "Scene.h"
#include "SceneRunner.h"
#include "Vec3.h"

#include <memory>

void buildScene(Scene& scene) {
    scene.world.add(std::make_shared<Sphere>(
        Vec3(0, 0, -1), 0.5,
        std::make_shared<Lambertian>(Vec3(0.7, 0.3, 0.3))));
    // ... add more objects
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom    = Vec3(0, 0, 1);
    cs.lookAt      = Vec3(0, 0, -1);
    cs.imageWidth  = 1920;
    cs.aspectRatio = 16.0 / 9.0;
    cs.verticalFOV = 30.0;
    return Camera(cs);
}

int main() {
    return runScene("my_scene", buildCamera(),
                    {.samplesPerPixel = 200, .maxDepth = 50},
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}
```

Add the target to `scenes/CMakeLists.txt`:

```cmake
add_miktracer_scene(my_scene)
```

And run:

```bash
./render.sh my_scene
```

For high-resolution renders, increase `imageWidth` (e.g. `1920` or `3840`) and `samplesPerPixel` (e.g. `200`–`1000`). Render time scales linearly in both.

## Build manually using CMake

```bash
cmake -B build -DBUILD_TESTING=ON   # configure (Catch2 is fetched on demand)
cmake --build build                 # build all scenes + tests
./build/scenes/three_spheres        # run a scene
(cd build && ctest --output-on-failure)
```

## Conventions

- **`struct` vs `class`:** `struct` for plain data with no invariants (e.g. `Vec3`, `Hit`, `Scene`, `CameraSettings`); `class` for types with private state and invariants (`Camera`, `Renderer`, `ImageBuffer`, `Material`).
- **Coordinates:** right-handed; camera looks down `-Z` by default; `lookAt` is the point the camera is aimed at.
- **Color:** stored linearly in `Vec3` channels; gamma correction (sqrt) and `[0, 0.999]` clamping happen exactly once on write inside `ImageBuffer::setPixel`.
- **RNG:** `randomDouble()` in `Utility.h` is default-seeded for reproducibility — the same scene produces the same PNG every run.

## Third-party

- `include/third_party/stb_image_write.h` — vendored from [nothings/stb](https://github.com/nothings/stb), public domain. Used for PNG output.
- [Catch2](https://github.com/catchorg/Catch2) — fetched at configure time when `-DBUILD_TESTING=ON`.