#include "uni_util.h"

#pragma GCC diagnostic ignored "-Wformat"

bool uni_check_digit(const char* str, size_t str_len) {
	bool digit_flag = false;
	bool dot_flag = false;
	for (size_t i = 0; i < str_len; i++)
		if (isdigit(str[i])) digit_flag = true;
		else if (str[i] == '.') dot_flag = true;
		else return false;


	if (dot_flag && !digit_flag) return false;
	return true;
}

uni_token uni_make_token(char* src, size_t src_len, uni_pos pos) {
	uni_token_type type;

	if (uni_check_digit(src, src_len)) type = UNI_TOKEN_NUMBER;
	else if (src[src_len - 1] == '\"') {
		type = UNI_TOKEN_STR;
		src_len--;
	}
	else type = UNI_TOKEN_WORD;

	return (uni_token) { type, pos, src, src_len };
}

void uni_print_token(uni_token token) {
	printf(
		"Token{src=\"%.*s\", length=%zu, type=%s, pos=(%zu, %zu, %zu)}",
		token.src_len, token.src, token.src_len, uni_token_type_string[token.type],
		token.pos.line, token.pos.col, token.pos.serial
	);
}

uni_op uni_make_op(uni_token token) {
	switch (token.type) {
	case UNI_TOKEN_NUMBER: {
		char* dot_occ = strstr(token.src, ".");
		if (dot_occ) return (uni_op) {
			UNI_OP_PUSHF, token.pos, token.src, token.src_len
		};
		else return (uni_op) {
			UNI_OP_PUSHI, token.pos, token.src, token.src_len
		};
	} case UNI_TOKEN_STR:
		return (uni_op) {UNI_OP_PUSHS, token.pos, token.src, token.src_len};
	case UNI_TOKEN_WORD: {
		char intr_str[UNI_INTRINSIC_BUFFER_LEN];
		strncpy(intr_str, token.src, token.src_len);

		uni_op_type intr_type = UNI_OP_PUSHW;

		for(
			size_t i = UNI_OP_ADD;	// UNI_OP_ADD is start of intrinsics
			i < UNI_OP_TYPE_COUNT;	// UNI_OP_TYPE_COUNT is end of intrinsics
			i++
		) if(strcmp(uni_intrinsics[i], intr_str) == 0) intr_type = i;

		return (uni_op) {intr_type, token.pos, token.src, token.src_len};
	}
	default:
		return UNI_OP_NULL;
	}
}

void uni_print_op(uni_op op) {
    printf(
        "Op{src=\"%.*s\", length=%zu, type=%s, pos=(%zu, %zu, %zu)}",
        op.src_len, op.src, op.src_len, uni_op_type_string[op.type],
        op.pos.line, op.pos.col, op.pos.serial
    );
}
