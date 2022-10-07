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

int convert_code (code_t *code, FILE *output_code)
{
        assert(code);
        assert(output_code);

        char cmd[MAX_NAME_LENGTH] = {};
        for (int i = 0; i < code->n_lines; i++) {
                sscanf(code->lines[i].ptr, "%s", cmd);
                if (stricmp(cmd, "push") == 0) {
                        int val = 0;
                        sscanf(code->lines[i].ptr + strlen(cmd), "%d", &val);
                        fprintf(output_code, "%d %d ", CMD_PUSH, val);
                } else if (stricmp(cmd, "pop") == 0) {
                        fprintf(output_code, "%d ", CMD_POP);
                } else if (stricmp(cmd, "add") == 0) {
                        fprintf(output_code, "%d ", CMD_ADD);
                } else if (stricmp(cmd, "sub") == 0) {
                        fprintf(output_code, "%d ", CMD_SUB);
                } else if (stricmp(cmd, "mul") == 0) {
                        fprintf(output_code, "%d ", CMD_MUL);
                } else if (stricmp(cmd, "div") == 0) {
                        fprintf(output_code, "%d ", CMD_DIV);
                } else if (stricmp(cmd, "out") == 0) {
                        fprintf(output_code, "%d ", CMD_OUT);
                } else if (stricmp(cmd, "in") == 0) {
                        fprintf(output_code, "%d ", CMD_IN);
                } else if (stricmp(cmd, "hlt") == 0) {
                        fprintf(output_code, "%d\n", CMD_HLT);
                } else if (stricmp(cmd, "dump") == 0) {
                        fprintf(output_code, "%d ", CMD_DUMP);
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
