# electron-22.3.27

Configure env.
set CHROMIUM_BUILDTOOLS_PATH C:\Developer\electron-dev\src\buildtools
set PATH C:\devtools\devtools\bin\ninja\ninja-win;D:\src\depot_tools;

patch:
1. src\electron
2. src\chrome\build\pgo_profiles

build:
gn gen out\Debug --args="import(\"//electron/build/args/debug.gn\")"
gn gen out/Debug --args="import(\"//electron/build/args/debug.gn\")" --ide=vs2019
gn gen out/Debug --args="import(\"//electron/build/args/debug.gn\")" --ide=vs2022
gn gen out/Testing --args="import(\"//electron/build/args/testing.gn\")" --ide=vs
gn gen out\Release --args="import(\"//electron/build/args/release.gn\")" --ide=vs2022
gn gen out\Testing --args="import(\"//electron/build/args/testing.gn\")" --ide=vs2022