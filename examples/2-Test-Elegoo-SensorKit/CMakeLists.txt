add_executable(sensor sensor.c)

target_link_libraries(sensor PRIVATE stefi::hal)
