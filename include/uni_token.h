#ifndef UNI_TOKEN_H_INCLUDED_
#define UNI_TOKEN_H_INCLUDED_

#include <stdio.h>

#include "uni_util.h"

#define UNI_TOKEN_NULL (uni_token){UNI_TOKEN_TYPE_NULL, UNI_POS_NULL, NULL, 0}

#pragma GCC diagnostic ignored "-Wunused-variable"

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

#endif  //UNI_TOKEN_H_INCLUDED_