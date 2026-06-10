# MCU build settings, attached to the libstefi target so they travel with it.
# Anything that links stefi::hal — in-tree examples or executables in a
# consuming application project — inherits the correct compile and link flags.
# Included from the top-level CMakeLists.txt after add_library(libstefi).

# compiler options (PUBLIC: ABI-relevant, needed by all code linked with the HAL)
target_compile_options(libstefi PUBLIC
        -mcpu=cortex-m4 -mthumb
        -ffunction-sections -fdata-sections
        -g -Wall -std=c99 -ffreestanding
)

# Linker script: overridable by an application with a different memory layout.
set(STEFI_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/../configs/link.ld
        CACHE FILEPATH "Linker script for executables linking stefi::hal")

# Optional float support in printf, adds ~12K to code size.
option(STEFI_PRINTF_FLOAT "Enable %f support in printf (~12K code size)" ON)

# linker options (INTERFACE: apply when an executable links stefi::hal)
target_link_options(libstefi INTERFACE
        -mcpu=cortex-m4 -mthumb
        -nostartfiles #use my startup file to init stack, mem, ...
        --specs=nano.specs #use newlib-nano, a lightweight standard c lib
        -lc -lgcc #links newlib nano, use dummy sys calls
        -Wl,--gc-sections
        -Wl,--print-memory-usage
        -T${STEFI_LINKER_SCRIPT}
)

if(STEFI_PRINTF_FLOAT)
    target_link_options(libstefi INTERFACE -u _printf_float)
endif()

# Relink executables when the linker script changes.
set_property(TARGET libstefi APPEND PROPERTY INTERFACE_LINK_DEPENDS ${STEFI_LINKER_SCRIPT})
