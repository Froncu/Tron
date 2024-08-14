cmake_minimum_required(VERSION 3.15)
project(Tron)

add_subdirectory(External/Fronge)

if (NOT MSVC)
   message(FATAL_ERROR "This project currently can be build only using Microsoft Visual C++ (MSVC).")
endif ()

add_executable(Tron
   Source/Components/Navigator.cpp
   Source/NavigationMesh/NavigationMesh.cpp
   Source/Prefabs/Prefabs.cpp
   Source/Systems/AIMoveControllerSystem.cpp
   Source/Systems/AIShootControllerSystem.cpp
   Source/Systems/NavigatorSystem.cpp
   Source/Systems/PlayerCanonControllerSystem.cpp
   Source/Systems/PlayerMoveControllerSystem.cpp
   Source/Systems/PlayerShootControllerSystem.cpp
   Source/Tron.cpp)

target_include_directories(Tron PRIVATE
   Source)

target_link_libraries(Tron PRIVATE
   Fronge
   FrongeEntry)

set_target_properties(Tron PROPERTIES
   CXX_STANDARD 20
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(Tron PRIVATE
   /W4
   /WX)

add_custom_command(TARGET Tron
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
   $<TARGET_FILE_DIR:Fronge>/Fronge.dll
   $<TARGET_FILE_DIR:Tron>)

add_custom_command(TARGET Tron
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
   ${CMAKE_SOURCE_DIR}/Data
   $<TARGET_FILE_DIR:Tron>/Data)