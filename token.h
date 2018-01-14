#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOK_NULL		0
#define TOK_STRING 		1
#define TOK_NUM			2
#define TOK_LEFT_PARA 	3
#define TOK_RIGHT_PARA	4
#define TOK_NEW_LINE	5
#define TOK_SEMI		6
#define TOK_COMMA		7
#define TOK_POUND		8
#define	TOK_LEQ			9
#define TOK_GEQ			10
#define TOK_DIGIT		11
#define TOK_ALPHA		12
#define TOK_GRAPH		13
#define TOK_WHITESPACE	14
#define TOK_DIRECTIVE	15
#define TOK_FILE_END	16

#define TOK_TYPE_COUNT 16

#define MAX_STRING_LENGTH 80

struct TOKEN_T{
	unsigned int type;
	void* data;
	unsigned int line;
};

int	open_file_to_tokenize(char* filename);
int tok_cleanup();

struct TOKEN_T* tok_get();
struct TOKEN_T* tok_get_expects(char type);
struct TOKEN_T* tok_make(unsigned int type, void* data, unsigned int line);
int	tok_release(struct TOKEN_T* tok);

int tok_string_read(char first, char* dest);
int tok_num_read(char first, int base, unsigned short* dest);

char* tok_type_to_string(char type);
int tok_print(struct TOKEN_T* tok);

#endif