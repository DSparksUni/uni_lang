#include <stdio.h>
#include <stdlib.h>

#include "uni_io.h"
#include "uni_file_iter.h"
#include "uni_token.h"
#include "uni_op.h"

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wmain"

int main(size_t argc, char** argv) {
	char* file_content;
	size_t file_size, bytes_read;

	uni_error file_parse_error = uni_read_file(
		argv[2], &file_content, &file_size, &bytes_read
	);
	if (file_parse_error) {
		fprintf(stderr, "[ERROR] File parse error: ");

		if(file_parse_error < UNI_ERROR_COUNT)
			fprintf(stderr, uni_error_display_msg[file_parse_error]);
		else fprintf(stderr, "Unknown");

		fputs("...", stderr);
		return -1;
	}
	size_t effective_size = (file_size > bytes_read) ? bytes_read : file_size;

	puts(file_content);
	puts("\n");

	uni_file_iter* file_iter;
	uni_error file_iter_error = uni_file_iter_init(
		file_content, effective_size, &file_iter
	);
	if (file_iter_error) {
		printf("[ERROR] File iterator initialization error: ");

		switch (file_iter_error) {
		case UNI_ALLOC_ERROR:
			printf("Failed to allocate iterator"); break;
		default:
			printf("Unknown");
		}
		puts("...");

		free(file_content);
		return -1;
	}

	while (!file_iter->done) {
		size_t tok_size;
		uni_pos tok_pos;
		char* tok_src;

		uni_file_iter_skip_whitespace(file_iter);

		if (file_iter->c == '\"')
			tok_src = uni_file_iter_collect_til_quote(
				file_iter, &tok_size, &tok_pos
			);
		else tok_src = uni_file_iter_collect_til_whitespace(
			file_iter, &tok_size, &tok_pos
		);

		uni_token token = uni_make_token(tok_src, tok_size, tok_pos);

		uni_op op = uni_make_op(token);
		uni_print_op(op);
		putchar('\n');

		uni_file_iter_advance(file_iter);
	}

	uni_file_iter_destroy(file_iter);
	return 0;
}
