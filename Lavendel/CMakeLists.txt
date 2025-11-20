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
 "src/Lavendel/Renderer/Window.cpp" "src/Lavendel/Renderer/Window.h" "src/Lavendel/Renderer/Pipeline/Pipeline.h" 
"src/Lavendel/Renderer/Pipeline/Pipeline.cpp" "src/Lavendel/Renderer/Core/Device.cpp" "src/Lavendel/Renderer/Core/Device.h"
"src/Lavendel/Renderer/Core/Swapchain.cpp" "src/Lavendel/Renderer/Core/Swapchain.h" "src/Lavendel/Renderer/Renderer.h" "src/Lavendel/Renderer/Renderer.cpp"
        src/Lavendel/Renderer/Model/Model.cpp
        src/Lavendel/Renderer/Model/Model.h
        src/lvpch.cpp
        src/lvpch.h
        src/Lavendel/Layers/Layer.cpp
        src/Lavendel/Layers/Layer.h
        src/Lavendel/Layers/LayerStack.cpp
        src/Lavendel/Layers/LayerStack.h
        "src/Lavendel/ImGui/ImGuiLayer.h"
        "src/Lavendel/ImGui/ImGuiLayer.cpp"
        "src/Lavendel/ImGui/ImGuiRenderer.h"
        "src/Lavendel/ImGui/ImGuiRenderer.cpp"
        "src/Lavendel/ImGui/Widgets/DemoWidget.h"
        "src/Lavendel/ImGui/Widgets/DemoWidget.cpp"
        "src/Lavendel/Utils/Profiling/Instrumentor.h"
)

if(WIN32)
    target_compile_definitions(Lavendel PRIVATE LV_BUILD_DLL)
    add_definitions(-DLV_BUILD_DLL)
endif()

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/volk)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/sdl3)


find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Vulkan include dir: ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(Lavendel PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(Lavendel PRIVATE ${Vulkan_LIBRARIES}
                                   SDL3::SDL3)
endif()

target_include_directories(Lavendel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl3/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui

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
        ${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel
)

target_precompile_headers(Lavendel PUBLIC ${CMAKE_SOURCE_DIR}/Lavendel/src/lvpch.h)

message(STATUS "LV_BUILD_DLL is defined: ${LV_BUILD_DLL}")
get_target_property(COMPILE_DEFS Lavendel COMPILE_DEFINITIONS)
message(STATUS "Lavendel compile definitions: ${COMPILE_DEFS}")

if(WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/SDL3/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/SDL3/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/SDL3/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/SDL3/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/SDL3/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/SDL3/imgui_impl_sdl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/SDL3/imgui_impl_vulkan.cpp
)

target_sources(Lavendel PRIVATE ${IMGUI_SRC})

target_include_directories(Lavendel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/backends
)