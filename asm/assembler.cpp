#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "assembler.h"

int check_argv (int argc, char **argv)
{
        if (argc != 3)
                printf ("You need to enter 3 arguments.\n");
        if (argv[2] == 0)
                printf("Enter name of the file with source code.\n");
        else if (strstr(argv[2], ".txt") == nullptr) {
                printf("Cannot find .txt file with source code.\n");
                return NO_SOURCE;
        }
        if (argv[1] == 0) {
                printf("You need to enter name of the output file.\n");
                return NO_OUTPUT_NAME;
        }

        return 0;
}

int convert_code (code_t *code, FILE *output_code, int second_cycle, labels_t *labels, int *asm_code)
{
        assert(code);
        assert(output_code);

        char cmd[MAX_NAME_LENGTH]  = {};
        char name[MAX_NAME_LENGTH] = {};
        int ip = 0;

        for (int i = 0; i < code->n_lines; i++) {
                sscanf(code->lines[i].ptr, "%s", cmd);
                if (stricmp(cmd, "push") == 0) {
                        sscanf(code->lines[i].ptr + strlen(cmd), "%s", name);
                        asm_code[--ip] = get_push_code(name, &asm_code[++ip]);
                        ip += 2;
                } else if (stricmp(cmd, "pop") == 0) {
                        sscanf(code->lines[i].ptr + strlen(cmd), "%s", name);
                        asm_code[--ip] = get_pop_code(name, &asm_code[++ip]/*, code->lines[i].length*/);
                        ip += 2;
                } else if (stricmp(cmd, "add") == 0) {
                        asm_code[ip++] = CMD_ADD;
                } else if (stricmp(cmd, "sub") == 0) {
                        asm_code[ip++] = CMD_SUB;
                } else if (stricmp(cmd, "mul") == 0) {
                        asm_code[ip++] = CMD_MUL;
                } else if (stricmp(cmd, "div") == 0) {
                        asm_code[ip++] = CMD_DIV;
                } else if (stricmp(cmd, "out") == 0) {
                        asm_code[ip++] = CMD_OUT;
                } else if (stricmp(cmd, "in") == 0) {
                        asm_code[ip++] = CMD_IN;
                } else if (stricmp(cmd, "hlt") == 0) {
                        asm_code[ip++] = CMD_HLT;
                } else if (stricmp(cmd, "dump") == 0) {
                        asm_code[ip++] = CMD_DUMP;
                } else if (stricmp(cmd, "jmp") == 0) {
                        asm_code[ip++] = CMD_JMP;
                        if (second_cycle) {
                                sscanf(code->lines[i].ptr + strlen(cmd), "%s", name);
                                if ((asm_code[ip++] = get_jmp_line(labels, name)) == NO_LABEL)
                                        return NO_LABEL;
                        } else {
                                sscanf(code->lines[i].ptr + strlen(cmd), "%s", get_label_name(labels));
                                asm_code[ip++] = -1;
                        }
                } else if (strrchr(cmd, ':')) {
                        if (!second_cycle)
                                create_label(labels, cmd, ip);
                        asm_code[ip++] = CMD_LABEL;
                } /*else if (stricmp(cmd, "call") == 0) {
                        asm_code[ip++] = CMD_CALL;
                        if (second_cycle) {
                                sscanf(code->lines[i].ptr + strlen(cmd), "%s", name);
                                if ((asm_code[ip++] = get_jmp_line(call_labels, name)) == NO_LABEL)
                                        return NO_LABEL;
                        } else {
                                sscanf(code->lines[i].ptr + strlen(cmd), "%s", labels->name);
                                asm_code[ip++] = -1;
                        }
                }  else if (stricmp(cmd, "ret")) {
                        asm_code[ip++] = CMD_RETURN;
                } else {
                        create_label(labels, cmd, ip);
                        asm_code[ip++] = CMD_CALL_LABEL;
                } */
        }
        if (second_cycle) {
                return 0;
        } else {
                convert_code(code, output_code, 1, labels, asm_code);
        }

        for (int i = 0; i < ip; i++) {
                fprintf(output_code, "%d ", asm_code[i]);
                if (asm_code[i] == CMD_HLT)
                        fprintf(output_code, "\n");
        }

        return 0;
}

int get_push_code (const char *val, int *asm_code)
{
        int num = 0;
        int reg = 0;
        char temp_val[MAX_NAME_LENGTH] = {};

        if (sscanf(val, "[%s]", temp_val)) {
                num = ARG_RAM;
                if ((reg = find_reg(temp_val)) > 0) {
                        num |= ARG_REG;
                        *asm_code = reg;
                } else if (sscanf(temp_val, "%d", asm_code)) {
                        num |= ARG_IMMED;
                }
        } else if (sscanf(val, "%s", temp_val)) {
                if ((reg = find_reg(temp_val)) > 0) {
                        num = ARG_REG;
                        *asm_code = reg;
                } else if (sscanf(val, "%d", asm_code)) {
                        num = ARG_IMMED;
                }
        }

        return num + CMD_PUSH;
}

int get_pop_code (const char *val, int *asm_code)
{
        int num = 0;
        int reg = 0;
        char temp_val[MAX_NAME_LENGTH] = {};

        if (sscanf(val, "[%s]", temp_val)) {
                num = ARG_RAM;
                if ((reg = find_reg(temp_val)) > 0) {
                        num |= ARG_REG;
                        *asm_code = reg;
                } else if (sscanf(temp_val, "%d", asm_code)) {
                        num |= ARG_IMMED;
                }
        } else if (sscanf(val, "%s", temp_val))
                if ((reg = find_reg(temp_val)) > 0) {
                        num = ARG_REG;
                        *asm_code = reg;
                }

        return num | CMD_POP;
}

int find_reg (const char *val)
{
        const char *regs_names[N_REGS] = {
                "rax",
                "rbx",
                "rcx",
                "rdx",
                "rex",
                "rfx",
                "rgx",
                "rhx"
        };

        for (int i = 0; i < N_REGS; i++)
                if (strstr(val, regs_names[i]))
                        return *(regs_names[i] + 1) - 'a' + 1;

        return NO_REGISTER;
}

int get_jmp_line (labels_t *labels, char *name)
{
        assert(labels);

        for (int i = 0; i < MAX_N_LABELS; i++)
                if (!strcmp(labels[i].name, name))
                        return labels[i].line;

        return NO_LABEL;
}

char* get_label_name (labels_t *labels)
{
        for (int i = 0; i < MAX_N_LABELS; i++)
                if (labels[i].name[0] == '\0')
                        return labels[i].name;

        return nullptr;
}

int create_label (labels_t *labels, const char *cmd, int n_line)
{
                // printf(" qwerty2 %s %d\n", cmd, n_line);
        int i = 0;
        char name[MAX_NAME_LENGTH] = {};
        strcpy(name, cmd);
                // printf(" qwerty3 %s %d\n", name, n_line);
        while (name[i] != ':')
                i++;
        name[i] = '\0';
        for (i = 0; i < MAX_N_LABELS; i++) {
                if (labels[i].name[0] == '\0') {
                        strcpy(labels[i].name, name);
                        labels[i].line = n_line;
                        // printf("N_LINE %d %s\n", labels[i].line, labels[i].name);
                        break;
                }
        }

        return 0;
}

void append_txt (char *output_file_name)
{
        if (strstr(output_file_name, ".txt") == nullptr)
                for (int i = 0; i < MAX_NAME_LENGTH; i++)
                        if (output_file_name[i] == '\0') {
                                strcpy(output_file_name + i, ".txt");
                                break;
                        }
}
