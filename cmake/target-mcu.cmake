# set linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/configs/link.ld)

# compiler and linker options
add_compile_options(-mcpu=cortex-m4 -mthumb)
add_compile_options(-ffunction-sections -fdata-sections)
add_compile_options(-g -Wall -std=c99 -ffreestanding)
add_compile_options(-ffunction-sections -fdata-sections)


add_link_options(-Wl,--print-memory-usage)
add_link_options(-mcpu=cortex-m4 -mthumb)
add_link_options(-nostartfiles) #use my startup file to init stack, mem, ...
add_link_options(--specs=nano.specs) #use newlib-nano, a lightweight standard c lib
add_link_options(-lc -lgcc) #links newlib nano, use dummy sys calls

add_link_options(-Wl,--gc-sections)
add_link_options(-u _printf_float) #_printf_floatOptional print float, add 12K to code size


add_link_options(-T ${LINKER_SCRIPT})
