#ifndef UNI_FILE_ITER_H_INCLUDED_
#define UNI_FILE_ITER_H_INCLUDED_

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "uni_util.h"

typedef struct uni_file_iter_t {
	char* src;
	size_t src_len;

	size_t i;
	char c;
	bool done;

	uni_pos pos;
} uni_file_iter;

uni_error uni_file_iter_init(char*, size_t, uni_file_iter**);

void uni_file_iter_advance(uni_file_iter*);
void uni_file_iter_skip_whitespace(uni_file_iter*);
char* uni_file_iter_collect_til_whitespace(uni_file_iter*, size_t*, uni_pos*);
char* uni_file_iter_collect_til_quote(uni_file_iter*, size_t*, uni_pos*);

void uni_file_iter_destroy(uni_file_iter*);

#endif	//UNI_FILE_ITER_H_INCLUDED