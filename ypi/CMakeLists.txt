set(EXECUTABLE_NAME youpi-engine)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ypi/include)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ypi/src)
# Find all subfolders in ypi/src and add them to include directories
file(GLOB_RECURSE header_files ${CMAKE_SOURCE_DIR}/ypi/src/*.hpp)
foreach(_headerFile ${header_files})
    get_filename_component(_dir ${_headerFile} DIRECTORY)
    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${_dir})
endforeach()

# target dependencies (sfml)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${SFML_INCLUDE_DIR})

target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

# Copy executable to root directory
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXECUTABLE_NAME}> ${CMAKE_SOURCE_DIR}/$<TARGET_FILE_NAME:${EXECUTABLE_NAME}>)
