# Final Year Project

This is my submitted digital artifact for my university Final Year Project. My project was titled "Exploring the Viability of Implementing Accessibility Features at a Game Engine Level". [You can read more about it on my website!](https://josephmac.co.uk/final-year-project)

A lot of this engine was developed prior to the project for other university units and on my own time. This repository is intended to show a project that I have worked on over an extended period of time as-is, and as such is only for demonstration purposes and should not be used for anything else.

Please [contact me via my website](https://josephmac.co.uk/contact) if you have any questions or comments!


## How to generate and build the project locally (if needed)

Important notes:
- ALL commands shown here begin and end at this root.
- This project has only been developed and tested on Windows machines.

---

### To generate and build project:

  Option A:
  
    Run _gen_and_build.bat

  Option B:
  
    Follow separate instructions below to generate and then build

---

### To generate project (first step):

  Option A:
  
    Run tools/gen.bat

  Option B:
  
    1. 'cd build' (make sure that a directory named 'build' exists!)
    2. 'cmake ..'
    3. 'cd ..'

---

### To build project (second step):

  Option A:
  
    Run tools/build.bat

  Option B:
  
    1. 'cmake --build build' (again, make sure that a directory named 'build' exists!)
    2. Copy .dll files from "dep/bin" to the same location as the produced .exe : build/release/ or build/debug/

---
