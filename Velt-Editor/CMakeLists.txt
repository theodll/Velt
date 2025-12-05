
add_executable(VeltEditor 
    "src/EditorApp.cpp"
    "src/EditorLayer/EditorLayer.cpp"
    "src/EditorLayer/EditorLayer.h"
)

target_include_directories(VeltEditor PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Velt/src
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
)

if (WIN32)
    target_compile_definitions(VeltEditor PRIVATE VT_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(VeltEditor PRIVATE VT_PLATFORM_LINUX)
elseif (APPLE)
    target_compile_definitions(VeltEditor PRIVATE VT_PLATFORM_OSX)
endif()

if (NOT TARGET Velt)
    add_subdirectory("${CMAKE_SOURCE_DIR}/Velt")
endif()

target_link_libraries(VeltEditor PRIVATE Velt)
