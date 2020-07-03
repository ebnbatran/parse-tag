# parse-tag
C++ library to parse different tags of different media formats

## Building
**Prerequisites**:
1. **CMake**: version 3.10 or more.
2. **GoogleTest library**: installed where your compiler can find it.

**Steps**:
1. Create a folder named *build* in the top-level folder.
2. From within *build*, open a command line and type: 

    ```cmake ..```

    This will configure CMake settings for the project.

3. Type: 

    ```cmake --build .```

## Running Unit Tests
After building, CMake will make an executable named *tests* inside a folder with the same name,

Run *tests* executable from your terminal.

It will run all Google Tests of the project and indicate if there are failures with their reasons.