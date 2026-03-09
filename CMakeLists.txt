cmake_minimum_required(VERSION 3.16)
project(projet_CXX VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# récupérer tous les fichiers .cpp du projet
file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(projet ${SOURCES})

# où sont les .hpp
target_include_directories(projet PRIVATE
    ${PROJECT_SOURCE_DIR}       
    ${PROJECT_SOURCE_DIR}/include   # et/ou dans include/ si tu en crées
)