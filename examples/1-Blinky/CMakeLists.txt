add_executable(blinky blinky.c)

target_link_libraries(blinky PRIVATE stefi::hal)
