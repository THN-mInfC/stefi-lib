add_executable(bitbang bitbang.c)

target_link_libraries(bitbang PRIVATE stefi::hal)
