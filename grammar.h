#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include "token.h"
#include "instruction.h"
#include "assembly.h"

int begin();
int expr_instruction();
int expr_label();
int expr_directive();
int expr_start();
int expr_byte();
int expr_word();
int expr_new_line();

int expr_set_x_y_flags();

#endif