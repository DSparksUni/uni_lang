#include "uni_io.h"

uni_error uni_read_file(
	const char* file_path, char** out_file_buffer,
	size_t* out_file_size, size_t* out_bytes_read
) {
	uni_error error_flag = UNI_SUCCESS;

	FILE* fptr = fopen(file_path, "r");
	if (!fptr) {
		error_flag = UNI_FILE_OPEN_ERROR;
		goto fail;
	}

	fseek(fptr, 0, SEEK_END);
	size_t file_size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	char* file_buffer = (char*)malloc(file_size + 1);
	if (!file_buffer) {
		error_flag = UNI_ALLOC_ERROR;
		goto fail;
	}

	size_t bytes_read = fread(file_buffer, 1, file_size, fptr);
	if (bytes_read == 0 && file_size != 0) {
		error_flag = UNI_FILE_READ_ERROR;
		free(file_buffer);
		goto fail;
	}
	if (bytes_read < file_size) file_buffer[bytes_read] = '\0';
	else file_buffer[file_size] = '\0';

	*out_file_size = file_size;
	*out_file_buffer = file_buffer;
	*out_bytes_read = bytes_read;
	return UNI_SUCCESS;

fail:
	*out_file_buffer = "";
	*out_file_size = 0;
	*out_bytes_read = 0;
	return error_flag;
}