cmake_minimum_required(VERSION 4.3.1)
project(LazyQR)
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(ZXing CONFIG REQUIRED)

add_subdirectory(libs/osqrlib)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/libs/osqrlib"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt
    ZXing::Core
    ZXing::ZXing
    ${OpenCV_LIBS}
    Osqrlib
)

include(CPack)
set(CPACK_PACKAGE_NAME "LazyQR")
set(CPACK_PACKAGE_VENDOR "GaneshS")
set(CPACK_PACKAGE_VERSION "1.0.0")

include(InstallRequiredSystemLibraries)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/"
    DESTINATION bin
    FILES_MATCHING PATTERN "*.dll"
)
