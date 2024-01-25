set(EXECUTABLE_NAME verlet-ecs)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ypi/src)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/exemples/verlet/include)
# Find all subfolders in ypi/src and add them to include directories
file(GLOB_RECURSE header_files ${CMAKE_SOURCE_DIR}/ypi/src/*.hpp)
foreach(_headerFile ${header_files})
    get_filename_component(_dir ${_headerFile} DIRECTORY)
    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${_dir})
endforeach()

target_link_libraries(${EXECUTABLE_NAME} youpi-engine)

# Copy executable to root directory
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXECUTABLE_NAME}> ${CMAKE_SOURCE_DIR}/$<TARGET_FILE_NAME:${EXECUTABLE_NAME}>)
