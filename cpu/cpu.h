#ifndef CPU_H
#define CPU_H

#include <string.h>
#include <stdlib.h>
#include "..\buffer.h"
#include "stack\stack.h"
#include "..\cmd.h"

static const int  DEFAULT_CAPACITY =   50;
static const int REGISTER_CAPACITY =    8;
static const int      RAM_CAPACITY = 8192;

struct cpu {
        int registers[REGISTER_CAPACITY] = {};
        int RAM[RAM_CAPACITY] = {};
};

int divide_cmds (code_t *code);
int execute_code (code_t *code, stack *stk);
int tens_count (int num);
void append_txt (char *output_file_name);

#endif /*CPU_H*/