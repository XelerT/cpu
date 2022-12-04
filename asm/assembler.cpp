#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

#define $ fprintf(stderr, "I'm here. File %s Line %d\n", __FILE__, __LINE__);
// #define $

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
                printf("cmd -%s- %d ip=%d\n", cmd, i, ip);

#include "asm_instructions.en"

                if (strrchr(cmd, ':')) {
                        if (!second_cycle)
                                create_label(labels, cmd, ip, 0);
                        asm_code[ip++] = CMD_LABEL;

                }
                /* else {
                        if (get_jmp_line(labels, cmd) != NO_LABEL) {
                                if (!second_cycle)
                                        create_label(labels, cmd, ip, 1);
                                asm_code[ip++] = CMD_CALL_LABEL;
                        }
                } */
                free_buf(cmd);
        }
        for (int i = 0; i <= ip; i++) {
                printf("%d ", asm_code[i]);
        }
        printf("\n");
        if (second_cycle) {
                return ip;
        } else {
                if (convert_code(code, output_code, 1, labels, asm_code) == NO_LABEL)
                        return NO_LABEL;
        }

        for (int i = 0; i <= ip; i++) {
                printf("%d ", asm_code[i]);
                fprintf(output_code, "%d ", asm_code[i]);
                // if (asm_code[i] == CMD_HLT)
                        // fprintf(output_code, "\n");
        }
$
        // listing(code, asm_code, (char *) __PRETTY_FUNCTION__, __LINE__);
        return 0;
}

int asm_jmp_call (int second_cycle, code_t *code, int *asm_code,
             char *cmd, char *name, labels_t *labels, int i, int *ip)
{
        assert(code);
        assert(asm_code);
        assert(cmd);
        assert(name);
        assert(labels);
        assert(ip);


        if (second_cycle) {
                sscanf(code->lines[i].ptr + strlen(cmd), "%s", name);
                if ((asm_code[(*ip)++] = get_jmp_line(labels, name)) == NO_LABEL)
                        return NO_LABEL;
                printf("asm line %d\n", asm_code[*ip - 1]);
        } else {
        $
                char label_name[MAX_NAME_LENGTH] = {0};
                sscanf(code->lines[i].ptr + strlen(cmd), "%s", label_name);
                // if (get_jmp_line(labels, label_name) != NO_LABEL) {
                $
                        if (stricmp(cmd, "jmp") == 0)
                                asm_code[(*ip)++] = -1;
                        else if (stricmp(cmd, "call") == 0)
                                asm_code[(*ip)++] = -2;
                // }
        }

        return 0;
}

int get_pp_code (const char *val, int *asm_code, const char *cmd, int coeff)
{
        assert(val);
        assert(asm_code);
        assert(cmd);

        int num = 0;
        int reg = 0;
        char temp_val[MAX_NAME_LENGTH] = {};
        float dot_num = 0;

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
                } else if ((stricmp(cmd, "push") == 0) || (stricmp(cmd, "pushf") == 0)) {
                        if (sscanf(val, "%f", &dot_num)) {
                        // printf("dotnum %f\n", dot_num);
                                *asm_code = (int) (dot_num * coeff);
                                num |= ARG_IMMED;
                        } else if (sscanf(val, "%d", asm_code))
                                num = ARG_IMMED;
                }
        }

        if (stricmp(cmd, "pop") == 0  || (stricmp(cmd, "popf") == 0))
                return num | CMD_POP;
        else if (stricmp(cmd, "push") == 0 || (stricmp(cmd, "pushf") == 0))
                return num | CMD_PUSH;

        return NULL;
}

void listing (code_t *code, int *asm_code, char *function, int line)
{
        assert(code);
        assert(asm_code);
        assert(function);

        printf("\n\nListing for %s on the %d line.\n\n", function, line);
        int ip = 0;
        for (int i = 0; i < code->n_lines; i++, ip++) {
                if (*code->lines[i].ptr != '\0' && *code->lines[i].ptr != '\n')
                        switch (asm_code[ip] & MASK_CMD) {
                        case CMD_PUSH:
                                printf("%s\t | %d %d\n", code->lines[i].ptr, asm_code[ip], asm_code[ip + 1]);
                                ip++;
                                break;
                        case CMD_PUSHF:
                                printf("%s\t | %d %d\n", code->lines[i].ptr, asm_code[ip], asm_code[ip + 1]);
                                ip++;
                                break;
                        case CMD_POPF:
                                if ((asm_code[ip] & ARG_REG) || (asm_code[ip] & ARG_RAM))
                                        printf("%s\t | %d %d\n", code->lines[i].ptr, asm_code[ip], asm_code[ip + 1]);
                                if (asm_code[ip] & ARG_IMMED)
                                        printf("%s\t | %d\n", code->lines[i].ptr, asm_code[ip]);
                                        ip++;
                                break;
                        case CMD_POP:
                                if ((asm_code[ip] & ARG_REG) || (asm_code[ip] & ARG_RAM))
                                        printf("%s\t | %d %d\n", code->lines[i].ptr, asm_code[ip], asm_code[ip + 1]);
                                if (asm_code[ip] & ARG_IMMED)
                                        printf("%s\t | %d\n", code->lines[i].ptr, asm_code[ip]);
                                ip++;
                                break;
                        case CMD_JMP:
                                printf("%s\t | %d %d\n", code->lines[i].ptr, asm_code[ip], asm_code[ip + 1]);
                                ip++;
                                break;
                        case CMD_CALL:
                                printf("%s\t | %d %d\n", code->lines[i].ptr, asm_code[ip], asm_code[ip + 1]);
                                ip++;
                                break;
                        default:
                                printf("*%s\t | %d\n", code->lines[i].ptr, asm_code[ip]);
                                break;
                        }
                else
                        ip--;
        }
}

#define DEF_REG(name,...) #name,
int find_reg (const char *val)
{
        assert(val);

        char upper_val[MAX_NAME_LENGTH] = {};
        for (int i = 0; *(val + i) != '\0'; i++)
                upper_val[i] = toupper(*(val + i));
        static const char *regs_names[N_REGS] = {
#include "..\registers.en"
                "R0X"
        };

        for (int i = 0; i < N_REGS; i++)
                if (strstr(upper_val, regs_names[i]))
                        return *(regs_names[i] + 1) - 'A' + 1;

        return NO_REGISTER;
}
#undef DEF_REG

int get_jmp_line (labels_t *labels, char *name)
{
        assert(labels);
        assert(name);

        for (int i = 0; i < MAX_N_LABELS; i++) {
                // printf("+line %s: %d %d\n", labels[i].name, labels[i].line, __LINE__);
                if (!strcmp(labels[i].name, name)) {
                        // printf("+-line %s: %d %d\n", labels[i].name, labels[i].line, __LINE__);
                        return labels[i].line;
                }
        }

        return NO_LABEL;
}

char* get_free_label_name (labels_t *labels)
{
        assert(labels);
        for (int i = 0; i < MAX_N_LABELS; i++)
                if (labels[i].name[0] == '\0')
                        return labels[i].name;

        return nullptr;
}

int create_label (labels_t *labels, const char *cmd, int n_line, int is_call)
{
        assert(labels);
        assert(cmd);
                // printf(" qwerty2 %s %d\n", cmd, n_line);
        int i = 0;
        char name[MAX_NAME_LENGTH] = {};
        strcpy(name, cmd);
        int label_exist = get_jmp_line(labels, name);
                // printf(" qwerty3 %s %d\n", name, n_line);
        while (name[i] != ':' && !is_call)
                i++;
        if (!is_call)
                name[i] = '\0';

        if (label_exist == NO_LABEL) {
                for (i = 0; i < MAX_N_LABELS; i++) {
                        if (labels[i].name[0] == '\0') {
                                strcpy(labels[i].name, name);
                                labels[i].line = n_line;
                                printf("created label line %s: %d\n", labels[i].name, labels[i].line);
                                // printf("N_LINE %d %s\n", labels[i].line, labels[i].name);
                                break;
                        }
                }
        } else {
                for (i = 0; i < MAX_N_LABELS; i++) {
                        if (strcmp(labels[i].name, name) == 0) {
                                labels[i].line = n_line;
                                // printf("N_LINE %d %s\n", labels[i].line, labels[i].name);
                                break;
                        }
                }
        }

        return 0;
}

void append_txt (char *output_file_name)
{
        assert(output_file_name);

        if (strstr(output_file_name, ".txt") == nullptr)
                for (int i = 0; i < MAX_NAME_LENGTH; i++)
                        if (output_file_name[i] == '\0') {
                                strcpy(output_file_name + i, ".txt");
                                break;
                        }
}

void free_buf (char *buf)
{
        assert(buf);

        for (int i = 0; buf[i] != '\0'; i++) {
                buf[i] = '\0';
        }
}
