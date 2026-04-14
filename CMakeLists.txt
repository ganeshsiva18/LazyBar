cmake_minimum_required(VERSION 4.3.1)
project(LazyBar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(ZXing CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt ZXing::Core ZXing::ZXing ${OpenCV_LIBS})
