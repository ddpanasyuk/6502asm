#include "token.h"

static FILE* f_global = 0;
static unsigned int line = 0;
char last = 0;

int open_file_to_tokenize(char* filename)
{
	f_global = fopen(filename, "r+b");
	line = 0;
	last = 0;
	if(f_global)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

int tok_cleanup()
{
	fclose(f_global);
}

struct TOKEN_T* tok_get()
{
	printf("6502asm: entered tok_get()\n");
	
	if(!f_global)
		return 0;
	
	unsigned short tok_number;
	char* token_string;
	char tok_tell = 0;
	
	if(last && !isblank(last))
	{
		tok_tell = last;
		last = 0;
	}
	else
	{
		do{
			int num_read = fread(&tok_tell, 1, 1, f_global);
			if(num_read <= 0)
			{
				tok_tell = EOF;
				break;
			}
		}while(isblank(tok_tell) && tok_tell != EOF);
	}
	
	if(tok_tell == EOF)
	{
		printf("6502asm: tok_get(): found EOF token\n");
		
		return tok_make(TOK_FILE_END, 0, line);
	}
	else if(isdigit(tok_tell))
	{
		printf("6502asm: tok_get(): found digit token\n");
		
		if(!tok_num_read(tok_tell, 10, &tok_number))
			return 0;
		
		printf("6502asm: tok_get(): token: %i\n", tok_number);
		
		return tok_make(TOK_NUM, (void*)(unsigned int)tok_number, line);
	}
	else if(isalpha(tok_tell))
	{
		printf("6502asm: tok_get(): found string token\n");
		
		token_string = malloc(sizeof(char)*(MAX_STRING_LENGTH)+1);
		
		if(!tok_string_read(tok_tell, token_string))
		{
			free(token_string);
			return 0;
		}
		
		printf("6502asm: tok_get(): token: %s\n", token_string);
		
		return tok_make(TOK_STRING, (void*)token_string, line);
	}
	else if(isgraph(tok_tell))
	{
		printf("6502asm: tok_get(): found graph token\n");
		
		switch(tok_tell)
		{
			case '$':
				printf("6502asm: tok_get(): hex token\n");
				if(!tok_num_read(0, 16, &tok_number))
					return 0;
				
				printf("6502asm: tok_get(): token: %i\n", tok_number);
				return tok_make(TOK_NUM, (void*)(unsigned int)tok_number, line);
			case ',':
				printf("6502asm: tok_get() token \',\'\n");				
				return tok_make(TOK_COMMA, 0, line);
			case ':':
				printf("6502asm: tok_get() token \':\'\n");
				return tok_make(TOK_SEMI, 0, line);
			case '>':
				printf("6502asm: tok_get() token >\n");
				return tok_make(TOK_GEQ, 0, line);
			case '<':
				printf("6502asm: tok_get() token <\n");
				return tok_make(TOK_LEQ, 0, line);
			case '#':
				printf("6502asm: tok_get() token #\n");
				return tok_make(TOK_POUND, 0, line);
			case '(':
				printf("6502asm: tok_get() token (\n");
				return tok_make(TOK_LEFT_PARA, 0, line);
			case ')':
				printf("6502asm: tok_get() token )\n");
				return tok_make(TOK_RIGHT_PARA, 0, line);
			case '.':
				printf("6502asm: tok_get() token .\n");
				return tok_make(TOK_DIRECTIVE, 0, line);
			default:
				return 0;
		}
	}
	else if(iscntrl(tok_tell))
	{		
		printf("6502asm: tok_get(): found cntrl token\n");
		
		switch(tok_tell)
		{
			case '\r':
				if(fread(&tok_tell, 1, 1, f_global))
				{
					line++;
					return tok_make(TOK_NEW_LINE, 0, line-1);
				}
				else
				{
					return tok_make(TOK_FILE_END, 0, line-1);
				}
			case '\n':
				line++;
				return tok_make(TOK_NEW_LINE, 0, line-1);
			default:
				return 0;
		}
	}
	
	return 0;
}

int tok_string_read(char first, char* dest)
{
	printf("6502asm: entered tok_string_read()\n");
	
	dest[0] = first;
	
	char tok_tell = 0;
	int i;
	
	i = (first ? 1:0); 
	do
	{
		if(fread(&tok_tell, 1, 1, f_global))
		{
			if(isalpha(tok_tell))
				dest[i++] = tok_tell;
			else
			{
				last = tok_tell;
				dest[i] = 0;
				return 1;
			}
		}
		else
			break;
	}
	while(i < 79);
	
	if(!first && !i)
		return 0;
	else if(first && i)
	{
		dest[i] = 0;
		return 1;
	}
	
	//if(tok_tell == EOF)
	//{
	//	dest[i] = 0;
	//	return 1;
	//}
	return 0;
}

int tok_num_read(char first, int base, unsigned short* dest)
{
	printf("6502asm: entered tok_num_read()\n");
	
	char* buffer = malloc(MAX_STRING_LENGTH);
	
	buffer[0] = first;
	
	char tok_tell = 0;
	int i;
	
	i = (first ? 1:0); 
	do
	{
		if(fread(&tok_tell, 1, 1, f_global))
		{
			if(base == 16 && isxdigit(tok_tell))
			{
				buffer[i++] = tok_tell;
			}
			else if(base == 10 && isdigit(tok_tell))
			{
				buffer[i++] = tok_tell;
			}
			else
			{
				last = tok_tell;
				buffer[i] = 0;
				*dest = (unsigned short)strtoul(buffer, 0, base);
				free(buffer);
				return 1;
			}
		}
		else
			break;
	}
	while(i < 79);
	
	if(!first && !i)
	{
		free(buffer);
		return 0;
	}
	else if(i)
	{
		buffer[i] = 0;
		*dest = (unsigned short)strtoul(buffer, 0, base);
		free(buffer);
		return 1;
	}
	free(buffer);
	return 0;
}

struct TOKEN_T* tok_make(unsigned int type, void* data, unsigned int line)
{
	struct TOKEN_T* token = (struct TOKEN_T*)calloc(1, sizeof(struct TOKEN_T));
	
	token->type = type;
	token->data = data;
	token->line = line;

	return token;
}

int tok_release(struct TOKEN_T* tok)
{
	if(tok)
	{
		if(tok->type == TOK_STRING)
			free(tok->data);
		free(tok);
	}
	
	return 0;
}

struct TOKEN_T* tok_get_expects(char type)
{
	struct TOKEN_T* tok = tok_get();
	if(tok)
	{
		if(tok->type != type)
		{
			printf("6502asm: err: expected %s on line %i, found %s instead!\n", tok_type_to_string(type), tok->line, tok_type_to_string(tok->type));
			tok_release(tok);
			return 0;
		}
	}
	else
	{
		printf("6502asm: tok_get_expects(): could not read next token type!\n");
	}
	return tok;
}

static char* tok_type_strings[] = {
	"null token",
	"type string",
	"type integer",
	"\"(\"",
	"\")\"",
	"new line",
	"\":\"",
	"\",\"",
	"\"#\"",
	"\"<\"",
	"\">\"",
	"type digit",
	"type char",
	"type graph",
	"type whitespace",
	"type directive",
	"END OF FILE"
};

static char* tok_type_unknown = "unknown type";

char* tok_type_to_string(char type)
{
	if(type > TOK_TYPE_COUNT)
		return tok_type_unknown;
	else
		return tok_type_strings[type];
}