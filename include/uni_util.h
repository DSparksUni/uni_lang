#ifndef UNI_UTIL_H_INCLUDED_
#define UNI_UTIL_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wunused-variable"

#define UNI_ALLOC(T) (T*)malloc(sizeof(T))

#define UNI_PACK_POS(l, c, s) (uni_pos){l, c, s}
#define UNI_UNPACK_POS(p) p.line, p.col, p.serial
#define UNI_POS_NULL (uni_pos) {0, 0, 0}

typedef enum uni_error_t {
	UNI_SUCCESS,
	UNI_FILE_OPEN_ERROR,
	UNI_FILE_LENGTH_ERROR,
	UNI_ALLOC_ERROR,
	UNI_FILE_READ_ERROR,
	UNI_INVALID_WRITE_HANDLE_ERROR,
	UNI_ERROR_COUNT
} uni_error;

static const char* uni_error_display_msg[] = {
	"Success",
	"Failed to open file",
	"Failed to get file length",
	"Allocation failure",
	"Failed to read file",
	"Invalid file write handle"
};

typedef struct uni_pos_t {
	size_t line, col, serial;
} uni_pos;

#endif	//UNI_UTIL_H_INCLUDED_
