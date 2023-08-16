#ifndef UNI_OP_H_INCLUDED_
#define UNI_OP_H_INCLUDED_

#include <stdio.h>
#include <string.h>

#include "uni_util.h"
#include "uni_token.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

#define UNI_OP_NULL (uni_op) {UNI_OP_TYPE_NULL, UNI_POS_NULL, NULL, 0};

#define UNI_INTRINSIC_BUFFER_LEN 16

typedef enum uni_op_type_t {
	UNI_OP_TYPE_NULL,
	UNI_OP_PUSHI,
	UNI_OP_PUSHF,
	UNI_OP_PUSHS,
	UNI_OP_PUSHW,
	UNI_OP_ADD,
	UNI_OP_SUB,
	UNI_OP_MULT,
	UNI_OP_DIV,
	UNI_OP_PRINT,
	UNI_OP_TYPE_COUNT
} uni_op_type;

static const char* uni_op_type_string[] = {
    "NULL",
    "PUSHI",
    "PUSHF",
    "PUSHS",
	"PUSHW",
    "ADD",
    "SUBTRACT",
    "MULTIPLY",
    "DIVIDE",
    "PRINT"
};

static const char* uni_intrinsics[] = {
	"", "", "", "", "",		//  Empty strings so that index into array correlates
							// with the type of that intrinsic 
	"+", "-", "*", "/",
	"print"
};

typedef struct uni_op_t {
	uni_op_type type;
	uni_pos pos;

	char* src;
	size_t src_len;
} uni_op;

uni_op uni_make_op(uni_token);
void uni_print_op(uni_op);

#endif  //UNI_OP_H_INCLUDED_