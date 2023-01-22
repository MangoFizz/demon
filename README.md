# Demon

Demon is a project with the goal of reverse engineering and reimplementing the functionality of the Halo PC demo, AKA Halo Trial.

Please see the [Frequently Asked Questions](https://github.com/Aerocatia/demon/wiki/FAQ) for more information.

## Compiling Demon

The current progress of demon can be compiled into a replacement `strings.dll`, where core game functions will be redirected at runtime into our re-implemented versions.

### Dependencies

Demon requires the following software for the build process:

- mingw-w64
- CMake (mingw-w64)
- python

### Compilation steps

First, recursively clone the git repository.
```
git clone https://github.com/Aerocatia/demon --recursive
```
Next, `cd` into the project directory and prepare the build directory using `i686-w64-mingw32-cmake`
```
cd demon
i686-w64-mingw32-cmake -B build -S .
```
The project can now be built using `cmake`.
```
cmake --build build
```

### Build options

Demon has some build time options that change functionality.

Option                    | Default
--------------------------|---------
DEMON_ENABLE_ENHANCEMENTS | OFF

#### DEMON_ENABLE_ENHANCEMENTS
This option enables some enhancements to re-implemented functions, adding extra features to assist in debugging and running the game.
Some examples of of these are enabling the in-game console and enabling "devmode" (unlocked console command access).
In the future it may also enable some core features seen in the mod [Chimera](https://github.com/SnowyMouse/chimera).

By default this setting is off and only direct re-implementations of functions will be compiled by the project.

## Testing Demon

Once `strings.dll` has been compiled, it can be run using an original install of Halo Trial

### Creating a testing environment

First, download a copy of the original Halo Trial Installer. This can be obtained [here](http://vaporeon.io/hosted/halo/original_files/HaloTrialSetup.exe)

Next, extract the contents of this installer using `7-zip` to a desired location. Installing Halo Trial the usual way is not recommended as this Windows XP-era game assumes you are likely to have direct write access to `Program Files`.

Finally, back up the original `strings.dll` and replace it with the one created by compiling this project.

You can now start the game by running `halo.exe`

### Tips

- Use the argument `-window` to start the game in a window
- Use the argument `-vidmode <width>,<height>,<refresh rate>` to set the game resolution.
- Use the argument `-novideo` to skip the promo video on exit or level completion.
