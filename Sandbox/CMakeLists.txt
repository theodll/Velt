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

if (WIN32)
    target_compile_definitions(Sandbox PRIVATE LV_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(Sandbox PRIVATE LV_PLATFORM_LINUX)
elseif (APPLE)
    target_compile_definitions(Sandbox PRIVATE LV_PLATFORM_OSX)
endif()

if (NOT TARGET Lavendel)
    add_subdirectory("${CMAKE_SOURCE_DIR}/Lavendel")
endif()

target_link_libraries(Sandbox PRIVATE Lavendel)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:Sandbox>/shaders
    COMMENT "Copying shaders to output directory"
)
