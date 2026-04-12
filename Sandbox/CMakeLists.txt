file(GLOB_RECURSE SANDBOX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Velt/src
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
)

if (WIN32)
    target_compile_definitions(Sandbox PRIVATE VT_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(Sandbox PRIVATE VT_PLATFORM_LINUX)
elseif (APPLE)
    target_compile_definitions(Sandbox PRIVATE VT_PLATFORM_OSX)
endif()

if (NOT TARGET Velt)
    add_subdirectory("${CMAKE_SOURCE_DIR}/Velt")
endif()

if(APPLE)

    find_package(SDL3 REQUIRED)

endif()

target_link_libraries(VeltEditor PRIVATE Velt SDL3::SDL3)

target_link_libraries(Sandbox PRIVATE Velt)


