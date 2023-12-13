set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/imgui)
set(SFML_DIR ${CMAKE_CURRENT_LIST_DIR}/SFML)
set(IMGUI_SFML_FIND_SFML OFF)

add_subdirectory(imgui-sfml)
add_subdirectory(SFML)