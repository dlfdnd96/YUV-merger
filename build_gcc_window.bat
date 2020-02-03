@echo off
mkdir build
echo.
cd build/
cmake -S .. -B . -G "MinGW Makefiles"
cmake --build . --config Release
echo.
echo.
echo.
echo window message: build complete.
cd ..