add_library(Osqrlib include/osqrlib.hpp)

if(WIN32)
    target_sources(Osqrlib PRIVATE src/winqrlib.cpp)
elseif(APPLE)
    target_sources(Osqrlib PRIVATE src/appleqrlib.cpp)
elseif(UNIX AND NOT APPLE)
    target_sources(Osqrlib PRIVATE src/linqrlib.cpp)
endif()