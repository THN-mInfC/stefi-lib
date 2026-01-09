set(TARGET_NAME test_wwdg) #filename change here

add_executable(${TARGET_NAME} ${TARGET_NAME}.c)

target_link_libraries(${TARGET_NAME} PRIVATE libstefi)
# Add internal headers to include path with 'internal' subdirectory
target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src  # Parent directory of 'internal'
)