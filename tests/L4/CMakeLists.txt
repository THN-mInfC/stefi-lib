add_executable(test_main test_main.c test.c tests_gpio.c)

target_link_libraries(test_main PRIVATE stefi::hal)
