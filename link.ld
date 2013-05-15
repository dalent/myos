OUTPUT_FORMAT("binary")
SECTIONS
{
  .text :  
  {
    code = .;
    *(.text) 
  }
  data = .;
  .data : AT(data - code)
  {
	*(.rodata)
    *(.data)
	*(.bss)
	*(.*)
  }

}