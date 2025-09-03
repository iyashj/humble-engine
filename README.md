# HumbleEngine

A modern C++23 game engine and utility library, wrapping raylib and providing tools like file management, renderer, logger, and more.

![HumbleEngine](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## Features

- [ ] FileManager
- [ ] Renderer
- [ ] Logger
- [ ] SceneManager
- [ ] AudioManager
- [ ] Networking (ambitious!)

## Quick Start

### Prerequisites
- C++23-compatible compiler
- CMake >= 3.20
- Conan >= 2.0
- Ninja (recommended)

### Build Instructions
1. Install dependencies with Conan:
	```sh
	conan install . --output-folder=build --build=missing
	```
2. Configure the project with CMake (using presets):
	```sh
	cmake --preset=default
	```
3. Build:
	```sh
	cmake --build --preset=default
	```

For debug builds, use the `debug` preset:
```sh
cmake --preset=debug
cmake --build --preset=debug
```

## Project Structure

- `src/` — Engine implementation files
- `include/` — Public engine headers
- `cmake/` — Custom CMake scripts
- `examples/` — Example applications
- `tests/` — Unit tests

## Adding Dependencies
Add dependencies to `conandata.yml` and manage them via Conan.

## License
MIT (see LICENSE file)