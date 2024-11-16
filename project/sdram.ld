/* Specify the memory areas */
MEMORY
{
    SDRAM (xrw)      : ORIGIN = 0xC0000000, LENGTH = 32M  /* 外部SDRAM容量为32M */ 
}


/* Define output sections */
SECTIONS
{

  .sdram (NOLOAD) :
  {
      . = ALIGN(4);
      KEEP(*(.sdram)) /* KEEP: Ensure that startup code is not garbage recyle */
      . = ALIGN(4);
  } >SDRAM

}

