#ifndef ASM_H
#define ASM_H
#include "..\buffer.h"
#include "..\cmd.h"

const int MAX_N_LABELS = 50;

struct labels_t
{
        int line = 0;
        char name[MAX_NAME_LENGTH] = {};
};

int convert_code (code_t *code, FILE *output_code, int second, labels_t *labels, int *asm_code);
int check_argv (int argc, char *argv[]);
void append_txt (char *output_file_name);
int create_label (labels_t *labels, const char *cmd, int n_line);
int get_jmp_line (labels_t *labels, char *name);
int get_push_code (const char *val, int *asm_code);
int find_reg (const char *val);
int get_pop_code (const char *val, int *asm_code);

#endif
