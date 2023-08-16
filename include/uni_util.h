#ifndef UNI_UTIL_H_INCLUDED_
#define UNI_UTIL_H_INCLUDED_

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define UNI_ALLOC(T) (T*)malloc(sizeof(T))

#define UNI_PACK_POS(l, c, s) (uni_pos){l, c, s}
#define UNI_UNPACK_POS(p) p.line, p.col, p.serial
#define UNI_POS_NULL (uni_pos) {0, 0, 0}

#define UNI_OP_NULL (uni_op) {UNI_OP_TYPE_NULL, UNI_POS_NULL, NULL, 0};

#define UNI_INTRINSIC_BUFFER_LEN 16

typedef enum uni_error_t {
	UNI_SUCCESS,
	UNI_FILE_OPEN_ERROR,
	UNI_FILE_LENGTH_ERROR,
	UNI_ALLOC_ERROR,
	UNI_FILE_READ_ERROR,
	UNI_ERROR_COUNT
} uni_error;

static const char* uni_error_display_msg[] = {
	"Success",
	"Failed to open file",
	"Failed to get file length",
	"Allocation failure",
	"Failed to read file"
};

typedef struct uni_pos_t {
	size_t line, col, serial;
} uni_pos;

typedef enum uni_token_type_t {
	UNI_TOKEN_TYPE_NULL,
	UNI_TOKEN_NUMBER,
	UNI_TOKEN_STR,
	UNI_TOKEN_WORD,
	UNI_TOKEN_TYPE_COUNT
} uni_token_type;

static const char* uni_token_type_string[] = {
	"NULL",
	"NUMBER",
	"STR",
	"WORD"
};

typedef struct uni_token_t {
	uni_token_type type;
	uni_pos pos;

	char* src;
	size_t src_len;
} uni_token;

uni_token uni_make_token(char*, size_t, uni_pos);
void uni_print_token(uni_token);


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

#endif	//UNI_UTIL_H_INCLUDED_
