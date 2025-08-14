add_library(DisplayOLED STATIC ssd1306.c)

target_include_directories(DisplayOLED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(DisplayOLED PUBLIC libstefi)