#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"

#define INSTRUCTION_COUNT 56

// 01 instructions
int instruction_ora();
int instruction_and();
int instruction_eor();
int instruction_adc();
int instruction_sta();
int instruction_lda();
int instruction_cmp();
int instruction_sbc();

// 10 instructions
int instruction_asl();
int instruction_rol();
int instruction_lsr();
int instruction_ror();
int instruction_stx();
int instruction_ldx();
int instruction_dec();
int instruction_inc();

// 00 instructions
int instruction_bit();
int instruction_jmp();
int instruction_jmp_abs();
int instruction_sty();
int instruction_ldy();
int instruction_cpy();
int instruction_cpx();

// branch instructions
int instruction_bpl();
int instruction_bmi();
int instruction_bvc();
int instruction_bvs();
int instruction_bcc();
int instruction_bcs();
int instruction_bne();
int instruction_beq();

int instruction_brk();
int instruction_jsr();
int instruction_rti();
int instruction_rts();

int instruction_php();
int instruction_plp();
int instruction_pha();
int instruction_pla();
int instruction_dey();
int instruction_tay();
int instruction_iny();
int instruction_inx();

int instruction_clc();
int instruction_sec();
int instruction_cli();
int instruction_sei();
int instruction_tya();
int instruction_clv();
int instruction_cld();
int instruction_sed();

int instruction_txa();
int instruction_txs();
int instruction_tax();
int instruction_tsx();
int instruction_dex();
int instruction_nop();

int expr_indirect_operand();
int expr_immediate_operand();
int expr_address_operand();

extern char* instruction_strings[];
extern int (*instruction_pointers[])();

#endif