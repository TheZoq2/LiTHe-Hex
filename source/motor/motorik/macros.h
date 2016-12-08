#ifndef H_MACROS
#define H_MACROS

#ifndef SHOULD_PRINT
	#define printf
#endif

#ifndef IS_X86
	//Sets and clears bit bit n in reg
	#define set_bit(reg,bit) reg |= (1<<bit)
	#define clear_bit(reg,bit) reg &= ~(1<<bit)

	#define set_ddr(reg, mask) reg=mask
#else
	
	#define set_bit(reg,bit) printf("Setting %c %#010x\n", reg, bit)
	#define clear_bit(reg,bit) printf("Clearing %c %#010x\n", reg, bit)

	#define set_ddr(reg, mask) printf("Setting DDR%c to %#010x\n", reg, mask)
#endif

#endif

