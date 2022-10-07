#ifndef CPU_H
#define CPU_H

#include <string.h>
#include "buffer.h"
#include "stack\stack.h"

enum cmd {
        CMD_PUSH =  1,
        CMD_POP  =  2,
        CMD_ADD  =  3,
        CMD_SUB  =  4,
        CMD_MUL  =  5,
        CMD_DIV  =  6,
        CMD_OUT  =  7,
        CMD_IN   =  8, //?
        CMD_HLT  =  9,
        CMD_DUMP = 10
};

enum errors {
        EXECUTE_ERR = -3,
        NO_SOURCE = -2,
        NO_OUTPUT_NAME = -1
};


static const int MAX_NAME_LENGTH = 20;
static const int DEFAULT_CAPACITY = 50;

int execute_code (code_t *code, stack *stk);
int tens_count (int num);
void append_txt (char *output_file_name);

#endif /*CPU_H*/
