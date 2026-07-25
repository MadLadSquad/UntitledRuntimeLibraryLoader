# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**UntitledRuntimeLibraryLoader (urll)** — a MadLadSquad micro-library that wraps the POSIX `dlopen`/`dlsym`/`dlclose`/`dlerror` family and its Win32 equivalents (`LoadLibrary`/`GetProcAddress`/`FreeLibrary`/`FormatMessage`) behind one API. Four source files, no dependencies beyond libc++ and the platform loader.

End-user documentation lives on the [GitHub wiki](https://github.com/MadLadSquad/UntitledRuntimeLibraryLoader/wiki), not in this repo.

This repo is normally consumed as a git submodule. Its main consumer is UntitledImGuiFramework (`Framework/ThirdParty/source-libraries/urll`), whose plugin loader (`Framework/Core/Interfaces/PluginInterface.cpp`) is the reference usage.

## Layout

- `urll.h` / `urll.cpp` — C++ API in `namespace URLL`. Header-only templates (`dlsym_var`, `dlsym_func`, and the `std::function` `dlsym` overload) plus four exported functions.
- `curll.h` / `curll.cpp` — `extern "C"` wrapper (`urll_dlopen`, `urll_dlsym`, `urll_dlsym_func`, `urll_dlsym_var`, `urll_dlclose`, `urll_dlerror`) that forwards to the C++ layer. Anything added to `urll.h` should get a matching `curll` entry point.

## Building & verifying

There is **no build system in this repo** — `CMakeLists.txt` is in `.gitignore` on purpose, as are `main.cpp`, `main.c`, `libtest.cpp`, and `libtest.hpp`. Those names are reserved for untracked local scratch harnesses; use them if you need a throwaway test program rather than adding new files to the repo.

Consumers compile the two `.cpp` files directly into their own target. UntitledImGuiFramework picks them up via a `GLOB_RECURSE` over `Framework/ThirdParty/source-libraries/*.cpp` in `Framework/cmake/SetupSources.cmake`, and defines `URLL_USE_FUNCTIONAL` + `MLS_EXPORT_LIBRARY` on each target.

There are no tests. To check a change compiles cleanly on Linux:

```bash
g++ -std=c++17 -Wall -Wextra -DURLL_USE_FUNCTIONAL -c urll.cpp curll.cpp
```

Windows-only branches (`_WIN32`) cannot be verified locally; they need MSVC. Read them carefully instead — every function in `urll.cpp` is a two-branch `#ifdef _WIN32` split, so a change on one side almost always needs the mirror on the other.

## Preprocessor contract

Consumers control the build through three macros; the library never defines them itself.

- `URLL_USE_FUNCTIONAL` — pulls in `<functional>` and enables the `std::function`-taking `dlsym` overload. Off by default so the header stays cheap.
- `MLS_EXPORT_LIBRARY` — opt into dllexport/dllimport annotation. Without it, `MLS_PUBLIC_API` expands to nothing on every platform.
- `MLS_LIB_COMPILE` — set only while building urll itself, selects `__declspec(dllexport)` over `__declspec(dllimport)`. Meaningful only on Win32 and only alongside `MLS_EXPORT_LIBRARY`.

The `MLS_PUBLIC_API` cascade is duplicated verbatim in `urll.h` and `curll.h` — keep the two copies identical.

Note that `urll.h` includes `<windows.h>` on Win32, so it leaks the Win32 namespace into every translation unit that includes it.

## API conventions worth preserving

These are deliberate and easy to break:

- **`dlsym_var` / `dlsym_func` / the `std::function` overload return the *handle*, not the symbol.** On success they return the `handle` argument; on failure `nullptr`. The symbol goes into the out-parameter. Callers test with `if (URLL::dlsym(handle, "sym", fn) != handle)`. Same for `urll_dlsym_func` / `urll_dlsym_var` in the C API. Plain 2-arg `dlsym` is the exception — it returns the symbol pointer directly.
- **`dlopen` is always lazy.** `RTLD_LAZY` on Unix, because Windows offers no eager equivalent. Don't add a flags parameter without a matching Win32 story.
- **`dlclose` returns 0 on success**, following the Unix convention. The Win32 branch inverts `FreeLibrary`, whose non-zero means success.
- **`dlerror` returns `nullptr` when there is no error.** The Win32 branch caches into a function-local `static std::string`, so the returned pointer is neither thread-safe nor stable across calls — the next `dlerror()` invalidates it. Unix delegates straight to `::dlerror()`, which has its own similar caveats.
- Every public function is `noexcept` in the C++ API, and every declaration carries a `// UntitledImGuiFramework Event Safety - Any time` comment. That tag is a convention inherited from the parent framework; new declarations should carry it too.

## Releases

Pushing a `v*` tag triggers `.github/workflows/release.yaml`, which strips `.git/`, tars the tree as `untitled-runtime-library-loader-<version>.tar.xz`, and publishes a GitHub release. Versions are four-part (`v4.0.0.0`).

## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).
