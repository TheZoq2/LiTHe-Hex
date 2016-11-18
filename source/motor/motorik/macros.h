#ifndef H_MACROS
#define H_MACROS

//Sets and clears bit bit n in reg
#define set_bit(reg,bit) reg |= (1<<bit)
#define clear_bit(reg,bit) reg &= ~(1<<bit)

#endif

