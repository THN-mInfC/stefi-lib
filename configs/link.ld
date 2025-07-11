ENTRY(Reset_Handler)

MEMORY
{
  RAM      (xrw) : ORIGIN = 0x20000000, LENGTH = 32K
  FLASH    (rx)  : ORIGIN = 0x8000000,  LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM); /* ram end */

SECTIONS
{

	.isr_vector     : { KEEP(*(.isr_vector)) }  > FLASH

	.text :
        {
            *(.text*)   /* All .text and .text.function_name, no keep */
            *(.rodata*) /* Read-only data */
        } > FLASH

	.data :
	{
		_sdata = .;
		*(.data*)
		_edata = .;
	} > RAM AT > FLASH
	_sidata = LOADADDR(.data);

	.bss (NOLOAD):
	{
		_sbss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	} > RAM

	. = ALIGN(8);
    _end = .;
}