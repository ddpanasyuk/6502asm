#include "grammar.h"

static struct TOKEN_T* tok_curr = 0;

#define TOK_TYPE() tok_curr->type
#define TOK_DATA() tok_curr->data
#define TOK_LINE() tok_curr->line
#define TOK_IS_VALID() tok_curr

int TOK_GET_NEXT()
{
	tok_release(tok_curr);
	tok_curr = tok_get();
	return (tok_curr ? 1 : 0);
}

int TOK_EXPECTS_NEXT(char type)
{
	tok_release(tok_curr);
	tok_curr = tok_get_expects(type);
	return (tok_curr ? 1 : 0);
}

int accepts(unsigned int type)
{
	return 0;
}

int expects(unsigned int type)
{
	tok_curr = tok_get();
	if(tok_curr)
	{
		if(tok_curr->type == type)
			return 1;
	}
	return 0;
}

int begin()
{
	printf("6502asm: entered begin()\n");
	if(TOK_GET_NEXT())
	{
		switch(TOK_TYPE())
		{
			case TOK_STRING:
				return expr_instruction();
			case TOK_DIRECTIVE:
				return expr_directive();
			case TOK_FILE_END:
				return 0;
			case TOK_NEW_LINE:
				return begin();
			default:
				printf("6502asm: INVALID TOKEN ON LINE %i\n", TOK_LINE());
				return -1;
		}
	}
	return -1;
}

int expr_new_line()
{
	TOK_GET_NEXT();
	if(TOK_IS_VALID())
	{
		if(TOK_TYPE() == TOK_NEW_LINE)
			return begin();
		else if(TOK_TYPE() == TOK_FILE_END)
			return 0;
		printf("6502asm: begin(): expected new line after line %i\n", TOK_LINE());
		return -1;
	}
	return -1;
}

int expr_directive()
{
	printf("6502asm: entered expr_directive()\n");
	if(TOK_EXPECTS_NEXT(TOK_STRING))
	{
		if(!strcmp(TOK_DATA(), "label"))
		{
			return expr_label();
		}
		else if(!strcmp(TOK_DATA(), "start"))
		{
			return expr_start();
		}
		else if(!strcmp(TOK_DATA(), "byte"))
		{
			return expr_byte();
		}
		else if(!strcmp(TOK_DATA(), "word"))
		{
			return expr_word();
		}
	}
	return -1;
}

int expr_label()
{
	printf("6502asm: entered expr_label()\n");
	if(TOK_EXPECTS_NEXT(TOK_STRING))
	{
		printf("6502asm: expr_label(): label %s found\n", TOK_DATA());
		return expr_new_line();
	}
	return -1;
}

int expr_start()
{
	printf("6502asm: entered expr_start()\n");
	if(TOK_EXPECTS_NEXT(TOK_NUM))
	{
		printf("6502asm: expr_start(): start address set to %4X\n", TOK_DATA());
		return expr_new_line();
	}
	return -1;
}

int expr_byte()
{
	printf("6502asm: entered expr_byte()\n");
	if(TOK_EXPECTS_NEXT(TOK_NUM))
	{
		printf("6502asm: expr_byte() byte %2X found\n", TOK_DATA());
		return expr_new_line();
	}
	return -1;
}

int expr_word()
{
	printf("6502asm: entered expr_word()\n");
	if(TOK_EXPECTS_NEXT(TOK_NUM))
	{
		printf("6502asm: expr_word() word %4X found\n", TOK_DATA());
		return expr_new_line();
	}
	return -1;	
}

int expr_instruction()
{
	printf("6502asm: entered expr_instruction\n");
	int i;
	for(i = 0; i < INSTRUCTION_COUNT; i++)
	{
		if(!strcmp(TOK_DATA(), instruction_strings[i])) // if match
		{
			return (*instruction_pointers[i])();
		}
	}
	printf("6502asm: expr_instruction(): invalid instruction \"%s\" on line %i\n", TOK_DATA(), TOK_LINE());
	return -1;
}

int expr_indirect_operand()
{
	printf("6502asm: entered expr_indirect_operand()\n");
	if(TOK_EXPECTS_NEXT(TOK_NUM))
	{
		if(TOK_GET_NEXT()) 
		{
			if(TOK_TYPE() == TOK_COMMA)
			{
				if(TOK_EXPECTS_NEXT(TOK_STRING))
				{
					if(TOK_EXPECTS_NEXT(TOK_RIGHT_PARA))
					{
						return expr_new_line();
					}
					return -1;
				}
				return -1;
			}
			else if(TOK_TYPE() == TOK_RIGHT_PARA)
			{
				if(TOK_EXPECTS_NEXT(TOK_COMMA))
				{
					if(TOK_EXPECTS_NEXT(TOK_STRING))
					{
						return expr_new_line();
					}
					return -1;
				}
				return -1;
			}
			return -1;
		}
		return -1;
	}
	return -1;
}

int expr_immediate_operand()
{
	printf("6502asm: entered expr_immediate_operand()\n");
	if(TOK_EXPECTS_NEXT(TOK_NUM))
	{
		return expr_new_line();
	}
	return -1;
}

int expr_address_operand()
{
	printf("6502asm: entered expr_address_operand()\n");
	if(TOK_GET_NEXT())
	{
		if(TOK_TYPE() == TOK_COMMA)
		{
			if(TOK_EXPECTS_NEXT(TOK_STRING))
			{
				return expr_new_line();
			}
		}
		else if(TOK_TYPE() == TOK_NEW_LINE)
		{
			return begin();
		}
		else if(TOK_TYPE() == TOK_FILE_END)
		{
			return 0;
		}
		return -1;
	}
	return -1;
}

// 01 instructions
int instruction_ora()
{
	printf("6502asm: entered expr_ora()\n");
	if(TOK_GET_NEXT())
	{
		switch(TOK_TYPE())
		{
			case TOK_LEFT_PARA:
				return expr_indirect_operand();
			case TOK_POUND:
				return expr_immediate_operand();
			case TOK_NUM:
			case TOK_STRING:
				return expr_address_operand();
			default:
				return -1;
		}
	}
	return -1;
}

int instruction_and()
{
	printf("6502asm: entered expr_and()\n");
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
