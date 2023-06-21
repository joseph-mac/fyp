cd tools || echo ERROR FINDING TOOLS DIR && pause && exit /b

call gen.bat || echo GENERATION ERROR && pause && exit /b
call build.bat || echo BUILD ERROR && pause && exit /b

cd ..

echo GENERATION AND BUILD SUCCESSFUL && pause
