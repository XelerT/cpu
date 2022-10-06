#ifndef ASM_H
#define ASM_H
#include "buffer.h"

enum cmd {
        CMD_PUSH =  1,
        CMD_POP  =  2,
        CMD_ADD  =  3,
        CMD_SUB  =  4,
        CMD_MUL  =  5,
        CMD_DIV  =  6,
        CMD_OUT  =  7,
        CMD_IN   =  8,
        CMD_HLT  =  9,
        CMD_DUMP = 10
};

enum errors {
        NO_SOURCE = -2,
        NO_OUTPUT_NAME = -1
};

static const int MAX_NAME_LENGTH = 20;

int convert_code (code_t *code, FILE *output_code);
int check_argv (int argc, char *argv[]);
void append_txt (char *output_file_name);

#endif
