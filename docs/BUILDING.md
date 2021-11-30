# Building Lilac from Source

## Pre-requisities

 * Up-to-date [VS Code](https://code.visualstudio.com/) (or your preferred IDE, if you know what you're doing)
 * [CMake](https://cmake.org/) (or VS Code's [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension)
 * [Git](https://git-scm.com) (or VS Code's [Remote Repositories](https://marketplace.visualstudio.com/items?itemName=github.remotehub) extension)
 * [Visual Studio](https://visualstudio.microsoft.com/downloads/) (or your preferred C++ compiler)

## 1. Clone the Repository

If you have [git](https://git-scm.com/downloads) installed, use `git clone https://github.com/lilac-sdk/loader --recursive` to clone the repository and all submodules. If you forgot the `--recursive` flag, do `git submodule update --init` in the cloned folder.

If you don't have git, you can manually clone the repository by downloading the source files as a ZIP or use the [Remote Repositories](https://marketplace.visualstudio.com/items?itemName=github.remotehub) extension for VS Code.

## 2. Configure with CMake

If you're operating from the command line, do:
 * `mkdir build`
 * `cd build`
 * `cmake .. -A Win32 -THost=x86`

If you're using the CMake Tools extension for VS Code, press F1 and run `CMake: Configure` to configure the project.

## 3. Build

If you're operating from the command line, run your preferred C++ compiler, for example `msbuild lilac_loader.sln /p:Configuration=RelWithDebInfo /p:PlatformTarget=x86`.

If you're using the CMake Tools extension for VS Code, click `Build` on the bottom toolbar.
