#include "assembly.h"

static char* asm_buffer;
static unsigned short asm_size;
static unsigned short asm_ptr;
static int asm_line;
static char asm_type;

int add_byte_to_program(char byte)
{
	if(asm_buffer)
	{
		asm_buffer[asm_ptr++] = byte;
		asm_buffer = realloc(asm_buffer, sizeof(char)*(++asm_size));
		return 0;
	}
	return -1;
}

int add_address_absolute_to_program(unsigned short word)
{
	if(add_byte_to_program(word & 0xFF))
		return -1;
	if(add_byte_to_program((word >> 8) & 0xFF))
		return -1;
	return 0;
}

int create_new_program()
{
	asm_buffer = malloc(sizeof(char)*1);
	asm_size = 1;
	asm_ptr = 0;
}

int cleanup_program()
{
	if(asm_buffer)
		free(asm_buffer);
	return 0;
}
int save_program_to_file(char* filename)
{
	FILE* f = fopen(filename, "w");
	
	if(f)
	{
		fwrite(asm_buffer, 1, asm_size, f);
		fclose(f);
		return 0;
	}
	return -1;
}

int add_to_program(unsigned short flags, char type, char op1_type, void* op1, char op2_type, void* op2, int line)
{
	printf("6502asm: entered add_to_program()\n");
	asm_line = line;
	asm_type = type;
	if(type < ASSEMBLY_TYPE_01)
	{
		int offset = assembly_flags_to_addressing_offset_01(flags);
		
		if(offset < 0)
			return -1;
		
		if((instruction_has_addressing_mode_01[type] >> offset) & 1)
		{
			add_byte_to_program(instruction_codes_01[type][offset]);
			printf("6502asm: add_to_program(): %2X\n", instruction_codes_01[type][offset]);
			if(flags & ASSEMBLY_FLAG_ABSOLUTE)
			{
				add_address_absolute_to_program((int)op1);
			}
			else if(flags & ASSEMBLY_FLAG_IMMEDIATE || flags & ASSEMBLY_FLAG_ZERO_PAGE)
			{
				add_byte_to_program((int)op1);
			}
			return 0;
		}
		return -1;
	}
	else if(type < ASSEMBLY_TYPE_10)
	{
		int offset = assembly_flags_to_addressing_offset_10(flags);
		
		if(offset < 0)
			return -1;
		
		type -= ASSEMBLY_TYPE_01;
		
		if((instruction_has_addressing_mode_10[type] >> offset) & 1)
		{
			add_byte_to_program(instruction_codes_10[type][offset]);
			printf("6502asm: add_to_program(): %2X\n", instruction_codes_10[type][offset]);
			if(flags & ASSEMBLY_FLAG_ABSOLUTE)
			{
				add_address_absolute_to_program((int)op1);
			}
			else if(flags & ASSEMBLY_FLAG_IMMEDIATE || flags & ASSEMBLY_FLAG_ZERO_PAGE)
			{
				add_byte_to_program((int)op1);
			}
			return 0;
		}
		return -1;
	}
	printf("6502asm: err: unknown error on line %i\n", asm_line);
	return -1;
}

int assembly_flags_to_addressing_offset_01(unsigned short asm_flags)
{
	if(asm_flags & ASSEMBLY_FLAG_ZERO_PAGE)
	{
		if(asm_flags & ASSEMBLY_FLAG_INDIRECT_INDEXED)
		{
			return 0b100;
		}
		else if(asm_flags & ASSEMBLY_FLAG_INDEXED_INDIRECT)
		{
			return 0b000;
		}
		else
		{
			if(asm_flags & ASSEMBLY_FLAG_X)
			{
				return 0b101;
			}
			else
			{
				return 0b001;
			}
		}
		return -1;
	}
	else if(asm_flags & ASSEMBLY_FLAG_IMMEDIATE)
	{
		return 0b010;
	}
	else if(asm_flags & ASSEMBLY_FLAG_ABSOLUTE)
	{
		if(asm_flags & ASSEMBLY_FLAG_X)
		{
			return 0b111;
		}
		else if(asm_flags & ASSEMBLY_FLAG_Y)
		{
			return 0b110;
		}
		else
		{
			return 0b011;
		}
	}
	return -1;
}

int assembly_flags_to_addressing_offset_10(unsigned short asm_flags)
{
	if((asm_type == ASSEMBLY_TYPE_STX || asm_type == ASSEMBLY_TYPE_LDX) && asm_flags & ASSEMBLY_FLAG_X)
	{
		printf("6502asm: err: line %i: x register cannot be used with %s\n", asm_line, instruction_strings[asm_type]);
		return -1;
	}
	
	if(asm_flags & ASSEMBLY_FLAG_ZERO_PAGE)
	{
		if(asm_flags & ASSEMBLY_FLAG_X || asm_flags & ASSEMBLY_FLAG_Y)
		{
			return 0b100;
		}
		return 0b001;
	}
	else if(asm_flags & ASSEMBLY_FLAG_IMMEDIATE)
	{
		return 0b000;
	}
	else if(asm_flags & ASSEMBLY_FLAG_ABSOLUTE)
	{
		if(asm_flags & ASSEMBLY_FLAG_X || asm_flags & ASSEMBLY_FLAG_Y)
		{
			return 0b101;
		}
		return 0b011;
	}
	else if(asm_flags & ASSEMBLY_FLAG_ACCUMULATOR)
	{
		return 0b010;
	}
	return -1;
}

/*
// 01 instructions
int instruction_ora()
{
	
}

int instruction_and()
{
	return 0;
}

int instruction_eor()
{
	return 0;
}

int instruction_adc()
{
	return 0;
}

int instruction_sta()
{
	return 0;
}

int instruction_lda()
{
	return 0;
}

int instruction_cmp()
{
	return 0;
}

int instruction_sbc()
{
	return 0;
}

// 10 instructions
int instruction_asl()
{
	return 0;
}

int instruction_rol()
{
	return 0;
}

int instruction_lsr()
{
	return 0;
}

int instruction_ror()
{
	return 0;
}

int instruction_stx()
{
	return 0;
}

int instruction_ldx()
{
	return 0;
}

int instruction_dec()
{
	return 0;
}

int instruction_inc()
{
	return 0;
}

// 00 instructions
int instruction_bit()
{
	return 0;
}

int instruction_jmp()
{
	return 0;
}

int instruction_jmp_abs()
{
	return 0;
}

int instruction_sty()
{
	return 0;
}

int instruction_ldy()
{
	return 0;
}

int instruction_cpy()
{
	return 0;
}

int instruction_cpx()
{
	return 0;
}

// branch instructions
int instruction_bpl()
{
	return 0;
}

int instruction_bmi()
{
	return 0;
}

int instruction_bvc()
{
	return 0;
}

int instruction_bvs()
{
	return 0;
}

int instruction_bcc()
{
	return 0;
}

int instruction_bcs()
{
	return 0;
}

int instruction_bne()
{
	return 0;
}

int instruction_beq()
{
	return 0;
}

int instruction_brk()
{
	return 0;
}

int instruction_jsr()
{
	return 0;
}

int instruction_rti()
{
	return 0;
}

int instruction_rts()
{
	return 0;
}

int instruction_php()
{
	return 0;
}

int instruction_plp()
{
	return 0;
}

int instruction_pha()
{
	return 0;
}

int instruction_pla()
{
	return 0;
}

int instruction_dey()
{
	return 0;
}

int instruction_tay()
{
	return 0;
}

int instruction_iny()
{
	return 0;
}

int instruction_inx()
{
	return 0;
}

int instruction_clc()
{
	return 0;
}

int instruction_sec()
{
	return 0;
}

int instruction_cli()
{
	return 0;
}

int instruction_sei()
{
	return 0;
}

int instruction_tya()
{
	return 0;
}

int instruction_clv()
{
	return 0;
}

int instruction_cld()
{
	return 0;
}

int instruction_sed()
{
	return 0;
}

int instruction_txa()
{
	return 0;
}

int instruction_txs()
{
	return 0;
}

int instruction_tax()
{
	return 0;
}

int instruction_tsx()
{
	return 0;
}

int instruction_dex()
{
	return 0;
}

int instruction_nop()
{
	return 0;
}
*/