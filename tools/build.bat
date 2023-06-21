cd ..
cmake --build build || echo CMAKE BUILD ERROR && pause && exit /b
copy /v "dep\bin\*.dll" "build\Debug\" || echo ERROR COPYING DLL DEPS && pause && exit /b
cd tools || echo ERROR FINDING TOOLS DIR && pause && exit /b

echo BUILD SUCCESSFUL
