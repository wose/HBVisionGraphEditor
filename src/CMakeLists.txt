set(hbvisiongrapheditor_SRC
  main.cpp
  imgui_impl_glfw_gl3.cpp
  ../3rdparty/imgui/examples/libs/gl3w/GL/gl3w.c
  ../3rdparty/imgui/imgui_demo.cpp
  ../3rdparty/imgui/imgui_draw.cpp
  ../3rdparty/imgui/imgui.cpp
  )

set(hbvisiongrapheditor_HDR
  )

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui/examples/libs/gl3w)

add_definitions(-DIMGUI_DISABLE_TEST_WINDOWS)

add_executable(hbvisiongrapheditor ${hbvisiongrapheditor_SRC})
target_link_libraries(hbvisiongrapheditor ${OPENGL_LIBRARIES} ${OpenCV_LIBS} glfw dl)

configure_file(Lenna.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Lenna.png COPYONLY)
