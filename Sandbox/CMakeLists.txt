file(GLOB_RECURSE SANDBOX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Lavendel/src
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
)

# Define the platform so Entrypoint.h includes main()
if (WIN32)
    target_compile_definitions(Sandbox PRIVATE LV_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(Sandbox PRIVATE LV_PLATFORM_LINUX)
elseif (APPLE)
    target_compile_definitions(Sandbox PRIVATE LV_PLATFORM_OSX)
endif()

# Ensure the Lavendel target exists. If the Lavendel project defines the target
# in Lavendel/CMakeLists.txt, add that subdirectory so the target is created
# and exported for linking. If your top-level CMakeLists already does this,
# this conditional will do nothing.
if (NOT TARGET Lavendel)
    add_subdirectory("${CMAKE_SOURCE_DIR}/Lavendel")
endif()

target_link_libraries(Sandbox PRIVATE Lavendel)

