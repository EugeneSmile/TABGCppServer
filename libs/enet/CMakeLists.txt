cmake_minimum_required(VERSION 3.5)
project(enet C)
include(GNUInstallDirs)

add_library(enet STATIC enet.c)

if(WIN32)
    target_link_libraries(enet PUBLIC winmm ws2_32)

    if(MSVC)
        target_compile_options(enet PRIVATE -W3)
    endif()
else()
    target_compile_options(enet PRIVATE -Wno-error)
endif()

target_include_directories(enet
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS enet
    EXPORT enet
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES enet.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT enet
    FILE enet.cmake
    NAMESPACE enet::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enet
)