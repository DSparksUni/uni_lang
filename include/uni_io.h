#ifndef UNI_IO_H_INCLUDED_
#define UNI_IO_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>
#include <stdint.h>

#include "uni_util.h"

typedef enum uni_command_t {
    UNI_COMMAND_NULL,
    UNI_COMMMAND_COMP,
    UNI_COMMAND_RUN,
    UNI_COMMAND_HELP,
    UNI_COMMAND_COUNT
} uni_command;

static const char* uni_command_strs[] = {
    "null",
    "comp",
    "run",
    "help"
};

uni_error uni_read_file(const char*, char**, size_t*, size_t*);
uni_command uni_get_command(const char*);

#endif	//UNI_IO_H_INCLUDED_