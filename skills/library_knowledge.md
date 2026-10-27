---
name: library_knowledge
description: Working reference for the shakhbat_cv C++ image-processing library (qlm namespace) - repo structure, build, API conventions, testing/docs workflow, performance rules, and debugging playbook. Read before any task in this repo.
---

# library_knowledge - shakhbat_cv

Working reference for the **shakhbat_cv** C++ image-processing library (`qlm` namespace).
Read this before any task in this repo. Last updated: 2026-10.

## 1. Identity & build

- Static lib target `shakhbat_cv` (alias `qlm::shakhbat_cv`); umbrella header `include/shakhbat_cv.hpp`.
- CMake; deps via `cmake/FetchDependencies.cmake` (FetchContent, pinned git **tags** in user-owned repos):
  `qlm::ThreadPool` (v1.1.0) and `qlm::PixelImage` (= the `Image`/`Pixel`/`BorderMode` classes, v2.0.0);
  tests pull googletest v1.15.0 (`tests/CMakeLists.txt`).
- Options: `SHAKHBAT_CV_BUILD_TESTING` (ON), `SHAKHBAT_CV_BUILD_EXAMPLES` (ON); also honor `BUILD_TESTING`.
- Existing build dir: `build/gnu_release` (MinGW Makefiles generator, g++ from MSYS2/mingw64,
  Release + `-DNDEBUG` -> `assert()` is compiled out there).
- Build: `cmake --build build/gnu_release --target shakhbat_cv|run|Test_<Name> -jN`.
- One gtest exe per test file; run an exe directly (`build/gnu_release/tests/Test_<Name>.exe`)
  or via CTest: `ctest --test-dir build/gnu_release -R <Name>` (running the exe directly from any
  cwd works because `LIBRARY_PATH` is an absolute macro).

## 2. Layout

- `include/<Function>.hpp` - public API, one header per function; `include/shakhbat_cv.hpp` includes all.
- `include/common/` - `types.hpp` (TermCriteria, `ROI()` helper), `types/containers.hpp` (Point, Rectangle,
  Circle, KeyPoint, Cluster, Pyramid, Line, LinePolar, Size), `types/enums.hpp`, `types/Kernel.hpp`
  (Kernel, SepKernel, Kernel1D), `types/Interpolation.hpp` (NN/bilinear/bicubic helpers),
  `types/matrix.hpp`, `types/TransformationMatrix.hpp`, `types/KDTree.hpp`, `common/draw.hpp`,
  `common/solve.hpp` (Solve, MinEigenValue).
- `source/<Function>.cpp` - implementation + **explicit template instantiations** at the bottom;
  `source/common/` holds shared implementation helpers used by several functions.
- `tests/source/<category>/Test_<Name>.cpp`; shared `tests/include/test_common.hpp`.
- `examples/run.cpp` - single runnable demo; keep function READMEs' example code identical to it when applicable.
- `doc/README.md` - function index; `doc/Functions/<Category>/<Name>/README.md` per function
  (input/reference images live in the same folder and double as test data). Some categories deviate:
  e.g. Drawing is one combined README page, not per-function subpages.
- `other-data-gray/` - dataset frame pairs for tests/examples.
- `cmake/` - `FetchDependencies.cmake` + package-config templates (`*Config.cmake.in`).
- `dependencies/` - optional vendored private dir; CMake includes it and GLOBs `dependencies/*.cpp`
  into the lib, but the folder currently does not exist (no-op hook).
- `skills/` - agent skills, incl. this file.
- Fetched dep sources are materialized under `build/gnu_release/_deps/*-src`
  - read dependency internals there (do not edit; changes are lost on re-configure).

## 3. Core types

- Formats: `ImageFormat::GRAY | RGB | HLS | HSV | YCrCb`; `pixel_t`: `uint8_t`, `int16_t`, `float`.
- `Image<frmt, T>`: `Create(w,h[,pix])`, `GetPixel(x,y[,border_mode])`, `SetPixel(x,y,pix)`,
  `.Width()/.Height()`, `Read`, `Write(file, alpha=true, quality=100)` (codec by extension).
- `Image` also exposes public fields (`width`, `height`, `stride`); prefer the accessors, but beware
  `stride` may exceed `width` (GetPixel(x,y) = `data[y*stride + x]`).
- **`GetPixel(int i)` (row-major index) is bounds-checked and SILENTLY returns a default pixel**
  (channels 0, alpha max) instead of throwing/asserting. Out-of-range bugs therefore surface as
  "odd black pixels / values", never as crashes or logs.
- `Pixel<frmt, T>`: constructors `Pixel()` (default), `Pixel(r,g,b)`, `Pixel(v)` (one value into all
  channels), `Pixel(..., alpha)`; fields `.v/.a` (GRAY) / `.r .g .b .a` etc. Arithmetic ops are
  per-channel and **clamp** to T's range (e.g. `operator/` uses `std::clamp`); `MAC(pixel, coeff)` helper.
- Pixel-tagged helpers in PixelImage: `L2Norm(pix1, pix2)`, `AbsDiff(pix1, pix2)`, `BlendColors`.
- `BorderMode<frmt,T>{border_type, border_pixel}`; `BorderType`: BORDER_CONSTANT (default),
  BORDER_REPLICATE, BORDER_REFLECT (**edge-repeat**; no BORDER_REFLECT_101 yet - OpenCV parity gap in PixelImage).
- `Rectangle<int>{top_left, width, height}` = ROI; `width/height <= 0` -> whole image; start clamped `max(_, 0)`.
- `ROI(roi, img_w, img_h)` (common/types.hpp) -> `std::tuple(start_x, start_y, end_x, end_y)`
  (inclusive start / exclusive end). Use: `auto [x0, y0, x1, y1] = ROI(roi, w, h);`
- `KeyPoint<T>`: point, scale, angle, response, error, status (`KPStatusFlag::TRACKED/UNTRACKED`), level, class_id.
- `OverFlowFlag`: SATURATE (default) | WRAP. `TermCriteria{max_count, epsilon}`.
- Interpolation helpers read `floor(x)..ceil(x)` per axis with a BorderMode.

## 4. API conventions (mandatory)

1. **Two-API pattern** for pixel ops:
   `void f(in..., Image& out, <params..., const Rectangle<int>& roi = {})` (writes only the ROI)
   and `Image<frmt, TO> f(in..., <params..., const Rectangle<int>& roi = {})` (allocates full-size, delegates).
2. **Parameter order**: `in, out, parameters..., roi` - roi ALWAYS last (Translate puts `pix` before roi).
3. Body pattern: assert equal dims -> `auto [x0, y0, x1, y1] = ROI(roi, w, h);` -> loop `[y0,y1) x [x0,x1)`.
4. **Explicit instantiation**: every supported (frmt, T...) combo for BOTH overloads, at the bottom of the .cpp;
   adding or changing a trailing parameter requires updating existing instantiations.
5. Filters default to `BorderMode{}` (= BORDER_CONSTANT{0}).
6. Refactors must keep results **bit-identical** - tests compare rendered reference images.
7. Deliberate behavior changes (bug fixes that alter output) are an exception to 6 -> see the
   reference-regeneration policy in section 5.

## 5. Testing conventions

- One gtest exe per test file (GLOB_RECURSE; no CMake edit needed for new tests); target = file name;
  CTest registers them with `WORKING_DIRECTORY = repo root` (`gtest_discover_tests`).
- `test::example_folder = LIBRARY_PATH + "/doc/Functions/"`, where `LIBRARY_PATH` is injected per test
  target via `target_compile_definitions(... LIBRARY_PATH="${CMAKE_SOURCE_DIR}")` (absolute path) -
  tests read inputs **from the doc folders** (docs + test data are the same files;
  moving a doc folder breaks test paths).
- Recipe: load input (`EXPECT_EQ(load, true)`) -> compute inside `Timer` -> `test::PrintTime(t)` ->
  draw/convert -> `test::ReReadImage(out, "jpg"|"png")` (write `out.<ext>` in cwd, re-read, delete;
  **`"png"` for lossless refs**; default `"jpg"` is lossy - never mix jpg round-trip with png refs
  under byte-exact comparison) ->
  `test::CompareImages(out, ref, threshold = Pixel{})` (default byte-exact; prints first mismatching pixel).
- Reference images must be rendered by the exact same code path/params as the test.
- For functions without stable refs use property asserts (sizes, bounds, validity, run-twice determinism);
  a tiny synthetic input (hand-built image with known expected values) is a good complement to image refs.
- **Reference-regeneration policy** (deliberate fixes): a checked-in reference image is *not* ground
  truth - it is a snapshot that may encode an old/buggy behavior. When a verified-correct change
  alters output: (a) establish the change is algorithmically correct first (section 6); (b) re-render
  the reference through the same test pipeline (draw/convert + ReReadImage round-trip); (c) overwrite
  the reference in the doc folder; (d) rerun the tests; (e) explain in the commit message why the old
  references encoded incorrect behavior.

## 6. Debugging playbook & PixelImage quirks

Approach for "a function fails / test fails / a proposed fix is suspect":

1. **Check git first**: working tree vs HEAD (`git diff HEAD -- source/<Func>.cpp`, `git show <commit>`)
   to see exactly what a proposed fix changed and what the pre-fix code was.
2. **Build + run only the failing test** (`cmake --build build/gnu_release --target Test_<Name>`,
   run the exe). Gtest output names the first mismatching pixel coordinate.
3. **Stand up a scratch harness** (do not commit it): one .cpp that includes `shakhbat_cv.hpp`, runs
   the current AND the old implementation (copied from git history, adapted to the current API), and
   reports how much each output matches the reference (e.g. `% of pixels within a small per-channel
   diff`, sized to absorb jpg re-encode noise). Compile with MinGW g++ (`-std=c++20`), includes
   `-I include -I <pixel_image-src>/include -I <pixel_image-src>/dependencies -I dependencies -I <thread_pool-src>/include`,
   linking `build/gnu_release/libshakhbat_cv.a`, `_deps/pixel_image-build/libPixelImage.a`,
   `_deps/openh264-build/openh264-install/lib/libopenh264.a` (+ standard Windows libs).
   Comparing old vs new behavior against the reference settles "is the fix correct?" objectively.
4. **Determinism check**: run the changed function twice and compare outputs pixel-for-pixel.
5. **Never rely on silent fallbacks for error handling** - see quirks below; a "passing" result can
   come from a default value, not from correct math.

PixelImage quirks (re-check after dependency version bumps):

- **`GetPixel(int)` out-of-range is silent** -> returns `Pixel{}` (all-black pixel), so bad indices
  become "mystery pixels" instead of crashes/logs. When debugging odd colors/values, first verify
  that every sampled index is actually in range.
- **`wider_t<signed_t<T>>` accidentally resolves to `double`** for integral T (`uint8_t` verified;
  same mechanism applies to all integral T): `signed_t` passes `std::make_signed<T>` (the class
  template) rather than `typename std::make_signed<T>::type` into `std::conditional`, so no `wider<>`
  specialization matches and the generic `double` fallback wins.
- Per-channel ops clamp results to T's range -> NaN/inf (e.g. from division by zero) silently
  become extreme values, masking math bugs. Guard denominator validity before dividing
  (or accumulate stats, not per-pixel work).
- Handy sanity math: compute candidate accumulators (sums/weights) in `double` first to check the
  generic accumulation type actually holds the magnitude; overflow would wrap silently.

## 7. Documentation conventions

- `doc/README.md` index entry per function, grouped by category (spaces -> `%20` in links);
  also lists backlog items ("... (backlog)").
- Function README order: `# Name` -> Description (+ link `../../../../source/<Name>.cpp`) ->
  `## C++ API` (verbatim signature) -> `## Parameters` table -> `## Return Value` -> `## Example`
  (complete compilable code; keep identical to `examples/run.cpp` when applicable) ->
  `### The parameters` -> `### The input` / `### The output` images -> `Time = <ms>`.

## 8. Performance guidelines (learned the hard way)

- **Never do whole-image work inside a per-item/per-iteration loop.** Precompute per level/call;
  use the ROI void APIs for inner loops.
- Case study - OpticalFlowPyrLK: whole-image ops per Newton iteration = 145-250 s per frame pair;
  ROI-limited to the key-point window (input ROI = output ROI +/- filter padding) = **0.1-0.3 s**,
  bit-identical.
- ROI bit-identical rules: full-size buffers + ROI-limited writes; every consumer ROI-limited to the same
  window; filter input region = output ROI +/- padding; out-of-source pixels get the fill value.
- Hoist scratch buffers out of inner loops (allocate per level, reuse).
- Multi-pass filters may internally limit the second pass to the columns/rows the first pass reads.
- Watch big-O blowups in init/iteration phases (per-pixel recompute each iteration); prefer caching
  or incremental updates, and spatial indices (e.g. a KD-tree) over linear scans in assignment steps.

## 9. Dependency / parity notes

- BORDER_REFLECT is edge-repeat, not equal to OpenCV BORDER_REFLECT_101 -> small border diffs vs
  OpenCV are expected; check PixelImage's current BorderType set before promising parity.
- Convenience math helpers have precision subtleties tied to the `wider_t` fallback (section 6);
  verify the actual return type before assuming exact-width integer distances.
