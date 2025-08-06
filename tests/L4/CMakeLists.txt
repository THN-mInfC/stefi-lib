
add_executable(test_main test_main.c test.c)

target_link_libraries(testshell PRIVATE libstefi)

# Add internal headers to include path with 'internal' subdirectory
#target_include_directories(${TARGET_NAME} PRIVATE
#        ${CMAKE_SOURCE_DIR}/src  # Parent directory of 'internal'
#)
