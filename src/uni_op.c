#include "uni_op.h"

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wunused-variable"

uni_op uni_make_op(uni_token token) {
	switch (token.type) {
	case UNI_TOKEN_NUMBER: {
		char* dot_occ = strstr(token.src, ".");
		if (dot_occ - token.src < token.src_len) return (uni_op) {
			UNI_OP_PUSHF, token.pos, token.src, token.src_len
		};
		return (uni_op) {
			UNI_OP_PUSHI, token.pos, token.src, token.src_len
		};
	} break;
	case UNI_TOKEN_STR:
		return (uni_op) {UNI_OP_PUSHS, token.pos, token.src, token.src_len};
	case UNI_TOKEN_WORD: {
		uni_op_type intr_type = UNI_OP_PUSHW;

		if(token.src_len < UNI_INTRINSIC_BUFFER_LEN) {
			// 	If the full string fits into the buffer, then copy it in
			// adding a null terminator as needed. If it doesn't fit,
			// no intrinsic check is done since no intrinsic is longer
			// then the buffer length
			char intr_str[UNI_INTRINSIC_BUFFER_LEN];
			strncpy(intr_str, token.src, token.src_len);
			intr_str[token.src_len] = '\0';

			for(
				size_t i = UNI_OP_ADD;	// UNI_OP_ADD is start of intrinsics
				i < UNI_OP_TYPE_COUNT;	// UNI_OP_TYPE_COUNT is end of intrinsics
				i++
			) if(strcmp(uni_intrinsics[i], intr_str) == 0) {
				intr_type = i;
				break;
			}
		}

		return (uni_op) {intr_type, token.pos, token.src, token.src_len};
	} break;
	default: return UNI_OP_NULL;
	}
}

void uni_print_op(uni_op op) {
    printf(
        "Op{src=\"%.*s\", length=%zu, type=%s, pos=(%zu, %zu, %zu)}",
        op.src_len, op.src, op.src_len, uni_op_type_string[op.type],
        op.pos.line, op.pos.col, op.pos.serial
    );
}