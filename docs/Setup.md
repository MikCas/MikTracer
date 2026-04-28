# MikTracer — Cleanup, Testing, and Scene Generator Plan

## Context

MikTracer is a working CPU raytracer at the "Ray Tracing in One Weekend" baseline: Vec3/Ray/Interval math, Sphere primitives, Lambertian/Metal/Dielectric materials, a Camera with DoF, and PPM output. The current entry point ([src/main.cpp](src/main.cpp)) hardcodes a single 6×10 sphere grid scene.

The user wants three things, in order:
1. **Finish the testing foundation** started on branch `feat/testing-system` (Catch2 is already wired up).
2. **Stabilize the project** so it can render a handful of portfolio-quality images.
3. **Add a simple scene generator** so multiple scenes can be produced without editing `main.cpp`.

This is a stepping stone toward a future GPU-based raytracer rewrite, so this plan deliberately keeps scope small and avoids architectural commitments that would be thrown away during that rewrite. The user will write all the code themselves — this plan only describes **what** needs to happen and **why**.

**Decisions confirmed with user:**
- Test scope: core math only (Vec3, Ray, Interval).
- Scene format: C++ builder API — each portfolio render is its own `.cpp` under `scenes/`.
- Image output: keep PPM, add PNG via `stb_image_write.h`.
- Scenes: separate CMake targets per scene, each linking `miktracer_lib`.

---

## Phase 1 — Cleanup (small, do first)

### 1.1 Fix defocus bug
[src/Camera.cpp:48](src/Camera.cpp:48) assigns `m_defocusDiskU` twice. Second line should set `m_defocusDiskV`:
```cpp
m_defocusDiskV = m_basisV * defocusRadius;
```
This is silently breaking depth-of-field — any non-zero `defocusAngle` samples only along U.

### 1.2 Repo hygiene
- Remove empty `Testing/` directory.
- Fix include style in `src/*.cpp`: replace `#include "../include/Vec3.h"` with `#include "Vec3.h"` (CMake already exposes `include/` via `target_include_directories`, so the relative paths are redundant and brittle).
- Commit the already-in-progress test rename changes in [tests/test_Vec3.cpp](tests/test_Vec3.cpp) (uncommitted per git status).

### 1.3 `.gitignore` check
Ensure `build/`, `*.ppm`, `*.png` (except under `renders/`) are ignored so accidental outputs don't get committed.

---

## Phase 2 — Testing (finish what's started)

**Scope:** Vec3 (already done, just polish) + Ray + Interval. No tests for Camera/Material/Sphere — those are rewrite candidates for GPU and testing them now buys little.

### 2.1 Ray tests — new file `tests/test_Ray.cpp`
- Construction: origin and direction stored correctly.
- `at(t)` returns `origin + t * direction` for `t = 0`, `t = 1`, negative `t`.
- Const-correctness of getters.

### 2.2 Interval tests — new file `tests/test_Interval.cpp`
- Default interval behaviour.
- `contains(x)` vs `surrounds(x)` edge cases (boundary values — inclusive vs exclusive matters for the hit logic).
- `clamp(x)` behaviour below/above/inside range.
- Empty interval (if defined) and `universe` (if defined) constants.

### 2.3 Vec3 cleanup
Keep current coverage. Add:
- `refract()` — verify Snell behaviour at a known angle/ratio (e.g. 45° into n=1.5).
- `randomUnitVector()` — statistical sanity: length ≈ 1, spread over 1000 samples covers all octants.

### 2.4 Wire new test files
Add `test_Ray.cpp` and `test_Interval.cpp` to [tests/CMakeLists.txt](tests/CMakeLists.txt). Run with:
```bash
cmake --build build && (cd build && ctest --output-on-failure)
```

---

## Phase 3 — PNG output

Add portfolio-ready output without losing the simple PPM path.

### 3.1 Drop in `stb_image_write.h`
- Single-header public-domain library from `nothings/stb`.
- Place at `include/third_party/stb_image_write.h`.
- Add note in README that the file is vendored (no submodule needed).

### 3.2 New `ImageBuffer` abstraction
Introduce a tiny `ImageBuffer` class in `include/ImageBuffer.h` / `src/ImageBuffer.cpp`:
- Stores pixels as `std::vector<uint8_t>` (width × height × 3, RGB8).
- `setPixel(i, j, Vec3 linearColor)` — does gamma correction + clamping once, here.
- `writePPM(path)` and `writePNG(path)`.

### 3.3 Refactor Camera::render
Currently writes PPM directly inline ([src/Camera.cpp:53](src/Camera.cpp:53)). Change signature to:
```cpp
void Camera::render(const Object& world, ImageBuffer& out, const Vec3& skyTop, const Vec3& skyBottom);
```
Caller decides the output format. Removes `std::ofstream` from Camera entirely — cleaner, and the GPU rewrite will want to own the pixel buffer anyway.

---

## Phase 4 — Scene generator

### 4.1 Scene builder API
New files `include/Scene.h` / `src/Scene.cpp`. A `Scene` bundles a world + camera + sky colors so a scene file is self-contained.

```cpp
class Scene {
public:
    ObjectList world;
    Camera camera;
    Vec3 skyTop = Vec3(1, 1, 1);
    Vec3 skyBottom = Vec3(0.5, 0.7, 1.0);

    // Convenience builders — thin wrappers over make_shared
    void addSphere(Vec3 center, double radius, std::shared_ptr<Material> mat);
    std::shared_ptr<Lambertian> lambertian(Vec3 albedo);
    std::shared_ptr<Metal>      metal(Vec3 albedo, double fuzz);
    std::shared_ptr<Dielectric> dielectric(double ior);
};
```

The material helpers aren't strictly necessary but make scene files read nicely:
```cpp
scene.addSphere({0, 0, -1}, 0.5, scene.metal({0.8, 0.8, 0.9}, 0.1));
```

Keep the existing `createRandomSphere` logic from [src/main.cpp:21](src/main.cpp:21) — move it into a free helper in `Scene.h` so scene files can reuse it.

### 4.2 `scenes/` directory
Each scene is a tiny `.cpp` with a `main()` that builds a `Scene`, renders it, and writes a PNG to `renders/`. Starter scenes:

- `scenes/three_spheres.cpp` — classic RTIOW hero shot (glass + matte + metal).
- `scenes/sphere_grid.cpp` — port the existing `createWorld()` from main.cpp.
- `scenes/cornell_spheres.cpp` — closed box made of large spheres (approximation; no planes yet), to show DoF + dielectric caustic hints.

Each scene file is ~40 lines of declarative setup.

### 4.3 CMake wiring
New `scenes/CMakeLists.txt` with a helper function:
```cmake
function(add_miktracer_scene name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE miktracer_lib)
endfunction()

add_miktracer_scene(three_spheres)
add_miktracer_scene(sphere_grid)
add_miktracer_scene(cornell_spheres)
```
Include it from the root [CMakeLists.txt](CMakeLists.txt) behind an option `BUILD_SCENES` (default ON).

### 4.4 Retire `src/main.cpp`
Once `sphere_grid` scene exists, `main.cpp` is redundant. Either delete it and drop the `miktracer` executable, or keep it as a minimal "hello sphere" smoke-test binary. User's call — deleting is cleaner.

---

## Files created / modified

**New:**
- `include/ImageBuffer.h`, `src/ImageBuffer.cpp`
- `include/Scene.h`, `src/Scene.cpp`
- `include/third_party/stb_image_write.h` (vendored)
- `tests/test_Ray.cpp`, `tests/test_Interval.cpp`
- `scenes/CMakeLists.txt`, `scenes/three_spheres.cpp`, `scenes/sphere_grid.cpp`, `scenes/cornell_spheres.cpp`

**Modified:**
- [src/Camera.cpp](src/Camera.cpp) — fix defocus bug (line 48); change `render()` signature to take `ImageBuffer&`.
- [include/Camera.h](include/Camera.h) — updated signature.
- [CMakeLists.txt](CMakeLists.txt) — add `ImageBuffer.cpp`, `Scene.cpp` to `miktracer_lib`; add_subdirectory(scenes).
- [tests/CMakeLists.txt](tests/CMakeLists.txt) — register new test files.
- All `src/*.cpp` — switch `#include "../include/X.h"` to `#include "X.h"`.

**Deleted:**
- `Testing/` (empty directory)
- optionally `src/main.cpp` after scenes land

---

## Verification

Run through this sequence after each phase to confirm nothing regressed:

1. **Build:** `cmake -B build -DBUILD_TESTING=ON && cmake --build build`
2. **Tests pass:** `(cd build && ctest --output-on-failure)` — expect Vec3 + Ray + Interval suites all green.
3. **Defocus fix visually confirmed:** render `sphere_grid` with `defocusAngle = 2.0` and verify the image actually blurs out-of-focus regions in both axes (previously blurred only along U).
4. **PNG output:** each scene produces a `.png` in `renders/` that opens in Preview/any image viewer.
5. **All three scenes render without crashing** at 400×400, 50 samples/px (fast dev settings) and produce visually distinct images.

---

## Explicitly out of scope (for GPU rewrite phase)

- BVH / acceleration structures — listed as a cleanup candidate but linear search is fine for portfolio scene sizes (< 500 objects).
- JSON scene loader — the builder API covers the user's current need.
- Multithreading — will be superseded by GPU parallelism.
- Triangle / mesh primitives — GPU-era feature.
- Replacing `shared_ptr`/virtual dispatch — GPU rewrite will need flat POD layouts; don't pre-emptively refactor.
- Integration / image-hash tests on Camera — too brittle given upcoming rewrite.
