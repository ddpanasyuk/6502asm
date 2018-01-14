#include "grammar.h"

static struct TOKEN_T* tok_curr = 0;

#define TOK_TYPE() tok_curr->type
#define TOK_DATA() tok_curr->data
#define TOK_LINE() tok_curr->line
#define TOK_GET_NEXT() tok_release(tok_curr); tok_curr = tok_get()
#define TOK_EXPECTS_NEXT(x) tok_release(tok_curr); tok_curr = tok_get_expects(x)
#define TOK_IS_VALID() tok_curr

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
	TOK_GET_NEXT();
	if(TOK_IS_VALID())
	{
		switch(TOK_TYPE())
		{
			case TOK_STRING:
				break;
			case TOK_DIRECTIVE:
				if(expr_directive())
					return -1; // error
				break;
			default:
				printf("6502asm: INVALID TOKEN ON LINE %i\n", TOK_LINE());
				return -1;
		}
		return expr_new_line();
	}
	else
	{
		return -1;
	}
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
		else
		{
			printf("6502asm: begin(): expected new line after line %i\n", TOK_LINE());
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int expr_instruction()
{
	return 0;
}

int expr_directive()
{
	printf("6502asm: entered expr_directive()\n");
	TOK_EXPECTS_NEXT(TOK_STRING);
	if(TOK_IS_VALID())
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
	else
	{
		return -1;
	}
	return 0;
}

int expr_label()
{
	printf("6502asm: entered expr_label()\n");
	TOK_EXPECTS_NEXT(TOK_STRING);
	if(TOK_IS_VALID())
	{
		printf("6502asm: expr_label(): label %s found\n", TOK_DATA());
		return 0;
	}
	else
	{
		return -1;
	}
}

int expr_start()
{
	return 0;
}

int expr_byte()
{
	printf("6502asm: entered expr_byte()\n");
	TOK_EXPECTS_NEXT(TOK_NUM);
	if(TOK_IS_VALID())
	{
		printf("6502asm: expr_byte() byte %2X found\n", TOK_DATA());
		return 0;
	}
	else
	{
		return -1;
	}
}

int expr_word()
{
	printf("6502asm: entered expr_word()\n");
	TOK_EXPECTS_NEXT(TOK_NUM);
	if(TOK_IS_VALID())
	{
		printf("6502asm: expr_word() word %4X found\n", TOK_DATA());
		return 0;
	}
	else
	{
		return -1;
	}
}
