#include "uni_file_iter.h"

uni_error uni_file_iter_init(
	char* src, size_t src_len, uni_file_iter** out_file_iter
) {
	uni_error error_code = UNI_SUCCESS;

	uni_file_iter* file_iter = UNI_ALLOC(uni_file_iter);
	if (!file_iter) {
		error_code = UNI_ALLOC_ERROR;
		goto fail;
	}

	file_iter->src = src;
	file_iter->src_len = src_len;

	file_iter->i = 0;
	file_iter->c = src[0];
	file_iter->done = false;

	file_iter->pos = UNI_PACK_POS(0, 0, 0);

	*out_file_iter = file_iter;
	return error_code;

fail:
	*out_file_iter = NULL;
	return error_code;
}

void uni_file_iter_advance(uni_file_iter* file_iter) {
	file_iter->i++;

	if (file_iter->i >= file_iter->src_len) file_iter->done = true;
	else {
		file_iter->c = file_iter->src[file_iter->i];
		file_iter->pos.serial++;

		if (file_iter->c == '\n') {
			file_iter->pos.line++;
			file_iter->pos.col = -1;
		}
		else file_iter->pos.col++;
	}
}

void uni_file_iter_skip_whitespace(uni_file_iter* file_iter) {
	while (isspace(file_iter->c)) uni_file_iter_advance(file_iter);
}

char* uni_file_iter_collect(
	uni_file_iter* file_iter, size_t* out_size, uni_pos* out_start_pos,
	bool (*comp_func)(char)
) {
	uni_file_iter_skip_whitespace(file_iter);

	char* start = file_iter->src + file_iter->i;
	uni_pos start_pos = file_iter->pos;
	size_t size = 0;
	size_t line = file_iter->pos.line;
	while (!comp_func(file_iter->c) && !file_iter->done) {
		size++;
		if (line != file_iter->pos.line) {
			size++;
			line = file_iter->pos.line;
		}

		uni_file_iter_advance(file_iter);
	}

	*out_size = size;
	*out_start_pos = start_pos;
	return start;
}

bool uni_is_space_or_newline(char c) {
	return isspace(c) || c == '\n';
}

char* uni_file_iter_collect_til_whitespace(
	uni_file_iter* file_iter, size_t* out_size, uni_pos* out_start_pos
) {
	return uni_file_iter_collect(file_iter, out_size, out_start_pos, &uni_is_space_or_newline);
}

bool isquote(char c) {
	return c == '\"';
}

char* uni_file_iter_collect_til_quote(
	uni_file_iter* file_iter, size_t* out_size, uni_pos* out_start_pos
) {
	uni_file_iter_advance(file_iter);
	char* result = uni_file_iter_collect(file_iter, out_size, out_start_pos, &isquote);

	*out_size = *out_size + 1;
	return result;
}

void uni_file_iter_destroy(uni_file_iter* file_iter) {
	free(file_iter->src);
	free(file_iter);
}