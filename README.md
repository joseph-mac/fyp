# Engine Project

NOTE: All commands begin and end at this root.

---

To generate and build project:

  Option A:
  
    Run _gen_and_build.bat

  Option B:
  
    Follow separate instructions below to generate and then build

---

To generate project (first step):

  Option A:
  
    Run tools/gen.bat

  Option B:
  
    1. 'cd build' (make sure that a directory named 'build' exists!)
    2. 'cmake ..'
    3. 'cd ..'

---

To build project (second step):

  Option A:
  
    Run tools/build.bat

  Option B:
  
    1. 'cmake --build build' (again, make sure that a directory named 'build' exists!)
    2. Copy .dll files from "dep/bin" to the same location as the produced .exe : build/release/ or build/debug/

---
