cmake_minimum_required(VERSION 3.13)    # CMake version check
project(train_simu)                        # Create project 
set(CMAKE_CXX_STANDARD 14)              # Enable c++14 standard

# Add main.cpp file of project root directory as source file
file(GLOB Project_SRC
     "src/*.h"
     "src/*.cpp"
)
include_directories("include")
add_executable(${PROJECT_NAME} ${Project_SRC})

target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/lib")

# add_executable(xmlparser /home/tanya/projects/railroad/src)
# target_include_directories(xmlparser PRIVATE /usr/local/lib/tinyxml2)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)


 

