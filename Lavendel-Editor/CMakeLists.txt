
add_executable(LavendelEditor 
    "src/EditorApp.cpp"
    "src/EditorLayer/EditorLayer.cpp"
    "src/EditorLayer/EditorLayer.h"
)

target_include_directories(LavendelEditor PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Lavendel/src
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
)

if (WIN32)
    target_compile_definitions(LavendelEditor PRIVATE LV_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(LavendelEditor PRIVATE LV_PLATFORM_LINUX)
elseif (APPLE)
    target_compile_definitions(LavendelEditor PRIVATE LV_PLATFORM_OSX)
endif()

if (NOT TARGET Lavendel)
    add_subdirectory("${CMAKE_SOURCE_DIR}/Lavendel")
endif()

target_link_libraries(LavendelEditor PRIVATE Lavendel)
