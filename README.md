 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./euler

 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./memoire 

 g++ -g -Wall -Wextra -Werror trace.cxx -o memoire

 g++ -pg -g -Wall -Wextra -Werror  Particule.cxx -o memoire ( all warnings being treated as errors)


 # My CMake Project

## Overview
This project is a simple CMake-based application that demonstrates the use of CMake for building C++ projects. It includes a main application file, a header file for function declarations, and a configuration file for tests.

## Project Structure
```
my-cmake-project
├── src
│   └── main.cpp          # Entry point of the application
├── include
│   └── my_header.h       # Header file with function declarations
├── tests
│   └── CMakeLists.txt    # CMake configuration for tests
├── CMakeLists.txt        # Main CMake configuration file
└── README.md             # Project documentation
```

## Building the Project
To build the project, follow these steps:

1. Ensure you have CMake installed on your system.
2. Open a terminal and navigate to the project directory.
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   cmake --build .
   ```

## Running the Application
After building the project, you can run the application by executing the generated binary located in the `build` directory.

## Running Tests
To run the tests, ensure that the test configuration in `tests/CMakeLists.txt` is set up correctly, then follow similar steps to build the tests in a separate build directory.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.