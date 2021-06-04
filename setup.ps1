Remove-Item 'build_w64_release' -Recurse -ErrorAction SilentlyContinue
Remove-Item 'build_w64_debug' -Recurse -ErrorAction SilentlyContinue

# Where to install the libraries
$MODELICA_RES_PATH = "C:\Users\Hixos\Documents\TesiCode\modelica\RocketControl\RocketControl\Resources\"

meson build_w64_release --native-file modelica-mingw-win64.ini --buildtype release `
     --prefix=$MODELICA_RES_PATH --includedir="Include" --libdir="Library\win64"

meson build_w64_debug --native-file modelica-mingw-win64.ini