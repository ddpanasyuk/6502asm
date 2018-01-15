#ifndef _ASSEMBLY_H_
#define _ASSEMBLY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"

struct LABEL_ENTRY_T{
	char* name;
	unsigned int offset;
};

#define ASSEMBLY_FLAG_NULL 0
#define ASSEMBLY_FLAG_ZERO_PAGE 0x1
#define ASSEMBLY_FLAG_ABSOLUTE 0x2
#define ASSEMBLY_FLAG_IMMEDIATE 0x4
#define ASSEMBLY_FLAG_X 0x8
#define ASSEMBLY_FLAG_Y 0x10
#define ASSEMBLY_FLAG_INDIRECT_INDEXED 0x20
#define ASSEMBLY_FLAG_INDEXED_INDIRECT 0x40
#define ASSEMBLY_FLAG_INDIRECT 0x80
#define ASSEMBLY_FLAG_ACCUMULATOR 0x100

#define ASSEMBLY_OP_FLAG_NULL 0
#define ASSEMBLY_OP_FLAG_LABEL 1
#define ASSEMBLY_OP_FLAG_NUMBER 2

#define ASSEMBLY_TYPE_ORA 0
#define ASSEMBLY_TYPE_AND 1
#define ASSEMBLY_TYPE_EOR 2
#define ASSEMBLY_TYPE_ADC 3
#define ASSEMBLY_TYPE_STA 4
#define ASSEMBLY_TYPE_LDA 5
#define ASSEMBLY_TYPE_CMP 6
#define ASSEMBLY_TYPE_SBC 7

#define ASSEMBLY_TYPE_01 8

#define ASSEMBLY_TYPE_ASL 8
#define ASSEMBLY_TYPE_ROL 9
#define ASSEMBLY_TYPE_LSR 10
#define ASSEMBLY_TYPE_ROR 11
#define ASSEMBLY_TYPE_STX 12
#define ASSEMBLY_TYPE_LDX 13
#define ASSEMBLY_TYPE_DEC 14
#define ASSEMBLY_TYPE_INC 15

#define ASSEMBLY_TYPE_10 16

#define ASSEMBLY_TYPE_BIT 16
#define ASSEMBLY_TYPE_JMP 17
#define ASSEMBLY_TYPE_STY 18
#define ASSEMBLY_TYPE_LDY 19
#define ASSEMBLY_TYPE_CPY 20
#define ASSEMBLY_TYPE_CPX 21
#define ASSEMBLY_TYPE_BPL 22
#define ASSEMBLY_TYPE_BMI 23
#define ASSEMBLY_TYPE_BVC 24
#define ASSEMBLY_TYPE_BVS 25
#define ASSEMBLY_TYPE_BCC 26
#define ASSEMBLY_TYPE_BCS 27
#define ASSEMBLY_TYPE_BNE 28
#define ASSEMBLY_TYPE_BEQ 29
#define ASSEMBLY_TYPE_BRK 30
#define ASSEMBLY_TYPE_JSR 31
#define ASSEMBLY_TYPE_RTI 32
#define ASSEMBLY_TYPE_RTS 33
#define ASSEMBLY_TYPE_PHP 34
#define ASSEMBLY_TYPE_PLP 35
#define ASSEMBLY_TYPE_PHA 36
#define ASSEMBLY_TYPE_PLA 37
#define ASSEMBLY_TYPE_DEY 38
#define ASSEMBLY_TYPE_TAY 39
#define ASSEMBLY_TYPE_INY 40
#define ASSEMBLY_TYPE_INX 41
#define ASSEMBLY_TYPE_CLC 42
#define ASSEMBLY_TYPE_SEC 43
#define ASSEMBLY_TYPE_CLI 44
#define ASSEMBLY_TYPE_SEI 45
#define ASSEMBLY_TYPE_TYA 46
#define ASSEMBLY_TYPE_CLV 47
#define ASSEMBLY_TYPE_CLD 48
#define ASSEMBLY_TYPE_SED 49
#define ASSEMBLY_TYPE_TXA 50
#define ASSEMBLY_TYPE_TXS 51
#define ASSEMBLY_TYPE_TAX 52
#define ASSEMBLY_TYPE_TSX 53
#define ASSEMBLY_TYPE_DEX 54
#define ASSEMBLY_TYPE_NOP 55

struct LABEL_ENTRY_T* make_label_entry(char* name, int offset);
int delete_label_entry(struct LABEL_ENTRY_T* src);

int add_label_entry(struct LABEL_ENTRY_T* src); // adds label entry to table 

int create_new_program();
int cleanup_program();
int save_program_to_file(char* filename);
int add_to_program(unsigned short flags, char type, char op1_type, void* op1, char op2_type, void* op2, int line);

int assembly_flags_to_addressing_offset_01(unsigned short asm_flags);
int assembly_flags_to_addressing_offset_10(unsigned short asm_flags);

#endif