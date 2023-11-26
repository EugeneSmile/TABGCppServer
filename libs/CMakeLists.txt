set(TARGET_NAME ${PROJECT_NAME}_LIBS)
message(STATUS "Configuring ${PROJECT_NAME} third party libraries")

set(LIBS_LIST "")
SubDirList(${CMAKE_CURRENT_LIST_DIR} LIBS_LIST)

foreach(LIB_DIR IN LISTS LIBS_LIST)
    message(STATUS "Adding library: ${LIB_DIR}")
    add_subdirectory(${LIB_DIR})
endforeach()
