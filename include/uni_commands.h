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

int uni_comp(const char*);
int uni_run(const char*);
int uni_help();

#endif  //UNI_COMMANDS_H_INCLUDED_