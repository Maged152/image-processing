# Image processing
Implementation of some image processing algorithms in c++.

For more details about the supported functions and examples read the [documentation](./doc).

# Dependencies
- [PixelImage](https://github.com/Maged152/PixelImage): A library to deal with images in various formats.
- [ThreadPool](https://github.com/Maged152/ThreadPool): A simple thread pool library for multi-threading support.

# Build & Targets

## Configure 
    $ cmake -S <source_dir> -B <build_dir>

You can use `presets`

    $ cmake -S <source_dir> --preset <preset_name>

To know the existing presets

    $ cmake -S <source_dir> --list-presets

## Build
    $ cmake --build <build_dir>

## Install
    $ cmake --install <build_dir> --prefix <install_dir>