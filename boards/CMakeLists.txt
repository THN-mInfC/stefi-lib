add_library(board STATIC board.c)
add_library(stefi::board ALIAS board)

target_include_directories(board PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(board PUBLIC libstefi)
# board_init() brings up the SimpleIO devices and the OLED, so board.c
# depends on the component layer above it (a deliberate static-lib cycle
# with SimpleIO, which needs board.h for the config tables).
target_link_libraries(board PRIVATE SimpleIO DisplayOLED)
