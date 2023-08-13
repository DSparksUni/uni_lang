#ifndef UNI_IO_H_INCLUDED_
#define UNI_IO_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <stdint.h>

#include "uni_util.h"

uni_error uni_read_file(const char*, char**, size_t*, size_t*);

#endif	//UNI_IO_H_INCLUDED_