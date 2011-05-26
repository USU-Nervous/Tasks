OUTPUT_FORMAT("binary")
ENTRY (start)
SECTIONS{
	. = 0x7C00;
	.text . :  {
		code = .;
		*(.text)
		. = ALIGN(0x200);
	}

	.data 0x8800 : {
		data = .;
		*(.data)
		. = ALIGN(0x200);
	}

	.rodata . : {
		rodata = .;
		*(.rodata)
		. = ALIGN(0x200);

	}

	.bss : {
		sbss = .;
		*(COMMON)
		*(.bss)
		ebss = .;
		. = ALIGN(0x200);
	}
	end = .;
}