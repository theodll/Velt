add_library(Lavendel SHARED
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Core.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Application.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Application.cpp"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Entrypoint.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Renderer/Renderer.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Renderer/Renderer.cpp"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Log.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Log.cpp"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/ApplicationEvent.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/Event.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/KeyEvent.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/MouseEvent.h"
)

target_compile_definitions(Lavendel PRIVATE LV_BUILD_DLL)
add_definitions(-DLV_BUILD_DLL)


add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/volk)
add_subdirectory(vendor/glm)


find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Vulkan include dir: ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(Lavendel PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(Lavendel PRIVATE ${Vulkan_LIBRARIES})
endif()

target_include_directories(Lavendel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_definitions(Lavendel PRIVATE
        $<$<CONFIG:Debug>:LV_DEBUG>
        $<$<CONFIG:Release>:LV_RELEASE>
        $<$<CONFIG:Dist>:LV_DIST>
)


target_include_directories(Lavendel
        PUBLIC
        ${CMAKE_SOURCE_DIR}/Lavendel/vendor/spdlog/include/
        ${CMAKE_SOURCE_DIR}/Lavendel/vendor/glfw/include/
        ${CMAKE_SOURCE_DIR}/Lavendel/vendor/glm/
        ${CMAKE_SOURCE_DIR}/Lavendel/src/
)

message(STATUS "LV_BUILD_DLL is defined: ${LV_BUILD_DLL}")
get_target_property(COMPILE_DEFS Lavendel COMPILE_DEFINITIONS)
message(STATUS "Lavendel compile definitions: ${COMPILE_DEFS}")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
