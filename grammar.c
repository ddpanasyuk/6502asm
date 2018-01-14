#include "grammar.h"

static struct TOKEN_T* tok_curr = 0;
static int instruction_curr = 0;
static unsigned short flags_curr = 0;
static void* op1 = 0;
static void* op2 = 0;
static char op1_type = 0;
static char op2_type = 0;

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
	flags_curr = 0;
	instruction_curr = 0;
	op1 = 0;
	op2 = 0;
	op1_type = 0;
	op2_type = 0;
	int i;
	for(i = 0; i < INSTRUCTION_COUNT; i++)
	{
		if(!strcmp(TOK_DATA(), instruction_strings[i])) // if match
		{
			//return (*instruction_pointers[i])();
			instruction_curr = i;
			if(TOK_GET_NEXT())
			{
				switch(TOK_TYPE())
				{
					case TOK_LEFT_PARA:
						return expr_indirect_operand();
					case TOK_POUND:
						return expr_immediate_operand();
					case TOK_STRING:
						flags_curr |= ASSEMBLY_FLAG_ABSOLUTE;
						op1_type = ASSEMBLY_OP_FLAG_LABEL;
						op1 = strdup(TOK_DATA());
						return expr_address_operand();
					case TOK_NUM:
						if((int)TOK_DATA() > 0xFF)
						{
							flags_curr |= ASSEMBLY_FLAG_ABSOLUTE;
							op1 = (void*)((int)TOK_DATA() & 0xFFFF);
							op1_type = ASSEMBLY_OP_FLAG_NUMBER;
						}
						else
						{
							flags_curr |= ASSEMBLY_FLAG_ZERO_PAGE;
							op1 = (void*)((int)TOK_DATA() & 0xFF);
							op1_type = ASSEMBLY_OP_FLAG_NUMBER;
						}
						return expr_address_operand();
					default:
						return -1;
				}
			}
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
					if(expr_set_x_y_flags())
						return -1;
					if(TOK_EXPECTS_NEXT(TOK_RIGHT_PARA))
					{
						flags_curr |= ASSEMBLY_FLAG_INDIRECT_INDEXED;
						return expr_new_line();
					}
					return -1;
				}
				return -1;
			}
			else if(TOK_TYPE() == TOK_RIGHT_PARA)
			{
				if(TOK_GET_NEXT())
				{
					if(TOK_TYPE() == TOK_COMMA)
					{
						flags_curr |= ASSEMBLY_FLAG_INDEXED_INDIRECT;
						if(TOK_EXPECTS_NEXT(TOK_STRING))
						{
							if(expr_set_x_y_flags())
								return -1;
							return expr_new_line();
						}
						return -1;
					}
					else if(TOK_TYPE() == TOK_NEW_LINE)
					{
						flags_curr |= ASSEMBLY_FLAG_INDIRECT;
						return begin();
					}
					else if(TOK_TYPE() == TOK_FILE_END)
					{
						flags_curr |= ASSEMBLY_FLAG_INDIRECT;
						return 0;
					}
					return -1;
				}
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
	flags_curr |= ASSEMBLY_FLAG_IMMEDIATE;
	if(TOK_EXPECTS_NEXT(TOK_NUM))
	{
		op1_type = ASSEMBLY_OP_FLAG_NUMBER;
		op1 = TOK_DATA();
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
				if(expr_set_x_y_flags())
					return -1;
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

int expr_set_x_y_flags()
{
	if(!strcmp(TOK_DATA(), "X"))
	{
		flags_curr |= ASSEMBLY_FLAG_X;
	}
	else if(!strcmp(TOK_DATA(), "Y"))
	{
		flags_curr |= ASSEMBLY_FLAG_Y;
	}
	else
	{
		printf("6502asm: expr_set_x_y_flags(): unknown register \"%s\" used on line %i\n", TOK_DATA(), TOK_LINE());
		return -1;
	}
	return 0;
}