#include "uni_commands.h"

bool uni_get_file_iter(
    const char* file_path, uni_file_iter** out_file_iter
) {
    char* file_content;
	size_t file_size, bytes_read;

	uni_error file_parse_error = uni_read_file(
		file_path, &file_content, &file_size, &bytes_read
	);
	if (file_parse_error) {
		fprintf(stderr, "[ERROR] File parse error: ");

		if(file_parse_error < UNI_ERROR_COUNT)
			fprintf(stderr, uni_error_display_msg[file_parse_error]);
		else fprintf(stderr, "Unknown");

		fputs("...", stderr);
		return true;	
	}
	size_t effective_size = (file_size > bytes_read) ? bytes_read : file_size;

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
		return true;
	}

    *out_file_iter = file_iter;
    return false;
}

uni_op uni_get_op(uni_file_iter* iter) {
	size_t tok_size;
	uni_pos tok_pos;
	char* tok_src;

	uni_file_iter_skip_whitespace(iter);

	if(iter->c == '\"')
		tok_src = uni_file_iter_collect_til_quote(
			iter, &tok_size, &tok_pos
		);
	else tok_src = uni_file_iter_collect_til_whitespace(
		iter, &tok_size, &tok_pos	
	);

	uni_token token = uni_make_token(tok_src, tok_size, tok_pos);
	return uni_make_op(token);
}

uni_command uni_get_command(const char* cmd_str) {
	for(size_t i = 1; i < UNI_COMMAND_COUNT; i++)
		if(strcmp(uni_command_strs[i], cmd_str) == 0) return i;

	return UNI_COMMAND_NULL;
}

int uni_comp(const char* file_path) {
    uni_file_iter* file_iter;
    bool file_iter_error = uni_get_file_iter(file_path, &file_iter);
    if(file_iter_error) return file_iter_error;

    while (!file_iter->done) {
		uni_op op = uni_get_op(file_iter);
		uni_print_op(op);
		putchar('\n');

		uni_file_iter_advance(file_iter);
	}

	char* asm_file_path = strdup(file_path);
    uni_get_write_from_input_path(asm_file_path);
	fprintf(stderr, "[DEBUG] Out: %s\n", asm_file_path);
        

    free(asm_file_path);
    uni_file_iter_destroy(file_iter);
    return 0;
}

int uni_run(const char* file_path) {
    uni_file_iter* file_iter;
    bool file_iter_error = uni_get_file_iter(file_path, &file_iter);
    if(file_iter_error) return file_iter_error;

    while (!file_iter->done) {
		uni_op op = uni_get_op(file_iter);
		uni_print_op(op);
		putchar('\n');

		uni_file_iter_advance(file_iter);
	}

    uni_file_iter_destroy(file_iter);
    return 0;
}

int uni_help() {
    puts("--------------------------Unicorn--------------------------");
    puts("\tCommands:");
    puts("\t\tcomp\t<file_path>");
    puts("\t\t    Compile the provided file into a binary");
    puts("\t\trun\t<file_path>");
    puts("\t\t    Interpret the provided file within the command line");
    puts("\t\thelp");
    puts("\t\t    Prints this message");

    return 0;
}