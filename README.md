# ThunderLib

ThunderLib is a C++ OpenGL engine made for learning purposes.

It spun off from the Hazel2D engine created by TheCherno, available at [https://github.com/TheCherno/Hazel](https://github.com/TheCherno/Hazel).

This repository contains the main engine inside the `ThunderLib` folder as well as some example projects in the `Games` folder.
ThunderLib builds into a static library and is meant to contain common code intended for use in multiple applications. Games builds into an executable and links the Core static library, as well as provides an include path to Core's code.

The `build_scripts/` directory contains build scripts for Windows and Linux, and the `vendor/` directory contains Premake binaries and all the third-party dependencies.

## Getting Started
1. Clone this repository
2. Open the `build_scripts/` directory and run the appropriate `Setup` script to generate projects files.
3. Open the project files to build the project.

You can edit the setup scripts to change the type of project that is generated - out of the box they are set to Visual Studio 2022 for Windows and gmake2 for Linux (using the Clang compiler).
Note that no macOS setup script is currently provided; you can duplicate the Linux script and adjust accordingly.

## License and Credits
- MIT License for this repository (see `LICENSE.md` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)
- stb is licensed under the MIT License (see included LICENSE.txt file for more details)
- miniaudio is licensed under the MIT License (see included LICENSE file for more details)
- ImGui is licensed under the MIT License (see included LICENSE.txt file for more details)
- GLM is licensed under the MIT License (see included copying.txt file for more details)
- GLFW is licensed under the Zlib License (see included LICENSE.md file for more details)
- glad generated code is Public Domain.
