#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include "token.h"

int accepts(unsigned int type);
int expects(unsigned int type);

int begin();
int expr_instruction();
int expr_label();
int expr_directive();
int expr_start();
int expr_byte();
int expr_word();
int expr_new_line();

#endif