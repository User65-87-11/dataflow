cmake_minimum_required(VERSION 3.28)

project(
  main01
  VERSION 1.0
  LANGUAGES CXX)

set(SOURCES 
src/main.cpp
src/dataflow.cpp
ImNodes/ImNodes.cpp
ImNodes/ImNodesEz.cpp
#ImNodes/example.cpp


ImGui/imgui_draw.cpp
ImGui/imgui_tables.cpp
ImGui/imgui_widgets.cpp
ImGui/imgui.cpp
ImGui/imgui_impl_vulkan.cpp
ImGui/imgui_impl_sdl2.cpp
ImGui/imgui_demo.cpp
)  

set(INCLUDES
    include 
    ImNodes/include
    ImGui/include
)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/VulkanSDK/1.3.275.0")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/SDL2-2.30.1")
 
find_package(SDL2 REQUIRED )
find_package(Vulkan REQUIRED)

 
add_subdirectory(.tests)
 
 
set(CMAKE_CXX_FLAGS "-fpermissive")

include_directories(${Vulkan_INCLUDE_DIRS})



add_executable(main01 ${SOURCES} ${Vulkan_LIBRARIES} )

target_include_directories(main01 PRIVATE ${INCLUDES})

target_link_libraries(main01 ${Vulkan_LIBRARIES} )

target_link_libraries(main01 ${SDL2_LIBRARIES} )
 
 




set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)


