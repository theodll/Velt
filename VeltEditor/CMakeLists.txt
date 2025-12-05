file(GLOB_RECURSE SANDBOX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(VeltEditor ${SANDBOX_SOURCES} "src/EditorLayer.cpp" "src/EditorLayer.h")

target_include_directories(VeltEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Velt
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

