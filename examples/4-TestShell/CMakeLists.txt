add_executable(testshell testshell.c)

target_link_libraries(testshell PRIVATE stefi::hal)
