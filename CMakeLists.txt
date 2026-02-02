cmake_minimum_required(VERSION 3.10)
project(MyRaylibGame)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_C_STANDARD 11)

# Find the library
find_package(raylib REQUIRED)

# Define your executable
add_executable(${PROJECT_NAME}
  main.c
  collision.c
)

# Link raylib to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
