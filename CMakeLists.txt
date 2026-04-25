cmake_minimum_required(VERSION 3.31...4.3.1)
project(LazyQR)

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
    opencv_world
    Osqrlib
)

# Copies files if windows

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()

# Install Rules

include(InstallRequiredSystemLibraries)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
 
if(WIN32)
    install(DIRECTORY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/"
        DESTINATION bin
        FILES_MATCHING PATTERN "*.dll"
        PATTERN "*d.dll" EXCLUDE
    )
elseif(APPLE)
    install(DIRECTORY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/"
        DESTINATION lib/${PROJECT_NAME}
        FILES_MATCHING
            PATTERN "*.dylib"
            PATTERN "*debug*" EXCLUDE
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "@executable_path/../lib/${PROJECT_NAME}"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
elseif(UNIX)
    install(DIRECTORY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/"
        DESTINATION lib/${PROJECT_NAME}
        FILES_MATCHING
            PATTERN "*.so"
            PATTERN "*.so.*"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib/${PROJECT_NAME}"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()


# CPack Data

set(CPACK_PACKAGE_NAME "LazyQR")
set(CPACK_PACKAGE_VENDOR "GaneshS")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple QR code tool")

if(WIN32)
    set(CPACK_GENERATOR "NSIS64")
    set(CPACK_NSIS_INSTALL_ROOT "$LOCALAPPDATA")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "LazyQR")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
elseif(UNIX)
    set(CPACK_GENERATOR "DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "GaneshS")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.17), libstdc++6 (>= 9)")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.17, libstdc++ >= 9")
endif()
 
include(CPack)
