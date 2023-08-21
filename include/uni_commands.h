#ifndef UNI_COMMANDS_H_INCLUDED_
#define UNI_COMMANDS_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "uni_util.h"
#include "uni_io.h"
#include "uni_file_iter.h"
#include "uni_token.h"
#include "uni_op.h"

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

uni_command uni_get_command(const char*);

int uni_comp(const char*);
int uni_run(const char*);
int uni_help();

#endif  //UNI_COMMANDS_H_INCLUDED_