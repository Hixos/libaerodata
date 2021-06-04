# libaerodata

Provides functions for interpolating multidimensional aerodata tables, to be used in Modelica.

## Compiling
Compiling libaerodata requires the Meson Build System (https://mesonbuild.com/Getting-meson.html)

1. Edit `modelica-mingw-win64.ini` to point to the correct modelica toolchain folder
2. Edit `setup.ps1` to point to the correct library installation folder (usually the "Resources" directory of your modelica project)
3. Run `setup.ps1`
4. Compile using `meson compile -C build_win64_release`
5. Install using `meson install -C build_win64_release`
