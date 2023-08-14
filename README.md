## Compilation
### On Windows
NOTE: The installation guide is for Visual Studio ONLY! If you want to use a different IDE/editor, this guide doesn't apply

 1. Make sure you have C++ installed in Visual Studio - https://irendering.net/how-to-install-c-and-c-support-in-visual-studio/
 2. Download the necessary dependencies (SDL2, SDL2_image and SDL2_ttf) from the following links: https://github.com/libsdl-org/SDL/releases/tag/release-2.26.5, https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.3, https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2. Make sure to download the `VC` file. Extract the downloaded files, and place the extracted files to a location that you'll remember (don't leave it in Downloads!)
 3. Follow the guides here to add the necessary libraries to your project: https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php, https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/msvc2019/index.php (NOTE: if you do not have admin rights on the PC you're trying to install the dependencies on, instead of editing the system environment variables you can paste the required `dll` files into the same folder as the compiled executable)
 5. If you installed the libraries according to the guide listed above, you will have to remove the `SDL2/` prefix in all the `#include` paths in the project
 6. In the `common.hpp` file, change all the variables relating to the file paths of game assets, to the absolute path of the game assets on your computer (e. g. change `"assets/player.png"` to `"C:\\Users\\John\\OneDrive\\Documents\\Visual Studio 2022\\Projects\\Spiked-Latte\\assets\\player.png"`). Instead of this you can also simply copy and paste the `assets` folder into the same folder as the executable that you'll be running. If using this method on Windows you might also need to change the `/` in the file paths to `\\`.

That's it! You should now be able to compile the code and run it!

### On Linux
 1. Install the following dependencies: libsdl2, libsdl2-dev, libsdl2-image, libsdl2-image-dev, libsdl2-ttf, libsdl2-ttf-dev. On Debian based distributions (such as Ubuntu) the following command can be used to install the required dependencies:
    `sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev`
 4. To compile the code run `g++ main.cpp -g -o spiked-latte -lSDL2 -lSDL2_image -lGL -lSDL2_ttf` from terminal in the same directory where `main.cpp` file is located.

The compiled binary can now be run by typing `./spiked-latte` in the directory in which you compiled it.
## Installation
### On Windows
To install on Windows, go to the *Releases* section on the GitHub page, and select the latest release. Download `.msi` file and run it. Continue with the installation through the installation wizard, and after you are done, the game should be installed. You should be able to run it from the start menu.

### On Linux (Debian based distros only)
To install on Debian-based Linux distros, go to the *Releases* section on the GitHub page, and select the latest release. Download the `.deb` file, and from the directory where the downloaded file is located run `sudo apt install ./spiked-latte.deb`. You should be able to launch the game using the `.desktop` launcher, or by running `spiked-latte` in the terminal.
