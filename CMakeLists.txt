project(Assignment_1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/FileSystem.h
        include/Files.h
        include/Commands.h
        include/GlobalVariables.h
        include/Environment.h
        src/Files.cpp
        src/FileSystem.cpp
        src/Commands.cpp
        src/Main.cpp
        src/Environment.cpp)

add_executable(Assignment_1 ${SOURCE_FILES})