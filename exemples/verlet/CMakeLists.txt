set(EXECUTABLE_NAME verlet-ecs)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/exemples/verlet/include)

target_link_libraries(${EXECUTABLE_NAME} youpi-engine)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Copy executable to root directory
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXECUTABLE_NAME}> ${CMAKE_SOURCE_DIR}/$<TARGET_FILE_NAME:${EXECUTABLE_NAME}>)
