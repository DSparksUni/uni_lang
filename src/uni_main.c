#include <stdio.h>
#include <stdlib.h>

#include "uni_commands.h"

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wmain"

int main(size_t argc, char** argv) {
	if(argc == 1) {
		fputs("[ERROR] No command supplied...", stderr);
		return -1;
	}
	uni_command command = uni_get_command(argv[1]);
	switch(command) {
	case UNI_COMMAND_NULL: {
		fputs("[ERROR] Unknown command supplied...", stderr);
		return -1;
	} break;
	case UNI_COMMMAND_COMP: {
		if(argc < 3) {
			fputs("[ERROR] No file supplied...", stderr);
			return -1;
		}
		return uni_comp(argv[2]);
	} break;
	case UNI_COMMAND_RUN: {
		if(argc < 3) {
			fputs("[ERROR] No file supplied...", stderr);
			return -1;
		}
		return uni_run(argv[2]);
	} break;
	case UNI_COMMAND_HELP:
		return uni_help(); break;
	default:
		fputs("[ERROR] Unreachable...", stderr);
		return -2;
	}
}
