cd ..
if not exist build mkdir build || echo ERROR CREATING BUILD FILE && pause && exit /b

cd build || echo ERROR FINDING BUILD DIR && pause && exit /b
cmake .. || echo CMAKE ERROR && pause && exit /b
cd ..\tools || echo ERROR FINDING TOOLS DIR && pause && exit /b

echo GENERATION SUCCESSFUL
