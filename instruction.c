#include "instruction.h"

char* instruction_strings[] = {
	"ora",
	"and",
	"eor",
	"adc",
	"sta",
	"lda",
	"cmp",
	"sbc",
	"asl",
	"rol",
	"lsr",
	"ror",
	"stx",
	"ldx",
	"dec",
	"inc",
	"bit",
	"jmp",
	"sty",
	"ldy",
	"cpy",
	"cpx",
	"bpl",
	"bmi",
	"bvc",
	"bvs",
	"bcc",
	"bcs",
	"bne",
	"beq",
	"brk",
	"jsr",
	"rti",
	"rts",
	"php",
	"plp",
	"pha",
	"pla",
	"dey",
	"tay",
	"iny",
	"inx",
	"clc",
	"sec",
	"cli",
	"sei",
	"tya",
	"clv",
	"cld",
	"sed",
	"txa",
	"txs",
	"tax",
	"tsx",
	"dex",
	"nop"
};
/*
int (*instruction_pointers[])() = {
	&instruction_ora,
	&instruction_and,
	&instruction_eor,
	&instruction_adc,
	&instruction_sta,
	&instruction_lda,
	&instruction_cmp,
	&instruction_sbc,
	&instruction_asl,
	&instruction_rol,
	&instruction_lsr,
	&instruction_ror,
	&instruction_stx,
	&instruction_ldx,
	&instruction_dec,
	&instruction_inc,
	&instruction_bit,
	&instruction_jmp,
	&instruction_sty,
	&instruction_ldy,
	&instruction_cpy,
	&instruction_cpx,
	&instruction_bpl,
	&instruction_bmi,
	&instruction_bvc,
	&instruction_bvs,
	&instruction_bcc,
	&instruction_bcs,
	&instruction_bne,
	&instruction_beq,
	&instruction_brk,
	&instruction_jsr,
	&instruction_rti,
	&instruction_rts,
	&instruction_php,
	&instruction_plp,
	&instruction_pha,
	&instruction_pla,
	&instruction_dey,
	&instruction_tay,
	&instruction_iny,
	&instruction_inx,
	&instruction_clc,
	&instruction_sec,
	&instruction_cli,
	&instruction_sei,
	&instruction_tya,
	&instruction_clv,
	&instruction_cld,
	&instruction_sed,
	&instruction_txa,
	&instruction_txs,
	&instruction_tax,
	&instruction_tsx,
	&instruction_dex,
	&instruction_nop
};*/

char instruction_codes_01[][8] = {
	{0x01, 0x05, 0x09, 0x0D, 0x11, 0x15, 0x19, 0x1D}, // ora
	{0x21, 0x25, 0x29, 0x2D, 0x31, 0x35, 0x39, 0x3D}, // and
	{0x41, 0x45, 0x49, 0x4D, 0x51, 0x55, 0x59, 0x5D}, // eor
	{0x61, 0x65, 0x69, 0x6D, 0x71, 0x75, 0x79, 0x7D}, // adc
	{0x81, 0x85, 0x00, 0x8D, 0x91, 0x95, 0x99, 0x9D}, // sta
	{0xA1, 0xA5, 0xA9, 0xAD, 0xB1, 0xB5, 0xB9, 0xBD}, // lda
	{0xC1, 0xC5, 0xC9, 0xCD, 0xD1, 0xD5, 0xD9, 0xDD}, // cmp
	{0xE1, 0xE5, 0xE9, 0xED, 0xF1, 0xF5, 0xF9, 0xFD}, // sbc
};

char instruction_has_addressing_mode_01[] = {
	0b11111111, // ora
	0b11111111, // and
	0b11111111, // eor
	0b11111111, // adc
	0b11111011, // sta
	0b11111111, // lda
	0b11111111, // cmp
	0b11111111, // sbc
};

char instruction_codes_10[][6] = {
	{0x00, 0x06, 0x0A, 0x0E, 0x16, 0x1E},
	{0x00, 0x26, 0x2A, 0x2E, 0x36, 0x3E},
	{0x00, 0x46, 0x4A, 0x4E, 0x56, 0x5E},
	{0x00, 0x66, 0x6A, 0x6E, 0x76, 0x7E},
	{0x00, 0x86, 0x00, 0x8E, 0x96, 0x00},
	{0xA2, 0xA6, 0x00, 0xAE, 0xB6, 0xBE},
	{0x00, 0xC6, 0x00, 0xCE, 0xD6, 0xDE},
	{0x00, 0xE6, 0x00, 0xEE, 0xF6, 0xFE} 	
};

char instruction_has_addressing_mode_10[] = {
	0b111110,
	0b111110,
	0b111110,
	0b111110,
	0b011010,
	0b111011,
	0b111010,
	0b111010,
};
