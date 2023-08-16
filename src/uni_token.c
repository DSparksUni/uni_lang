#include "uni_token.h"

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"

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
		token.src_len, token.src, token.src_len,
		uni_token_type_string[token.type],
		token.pos.line, token.pos.col, token.pos.serial
	);
}