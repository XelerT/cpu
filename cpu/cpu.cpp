#include <stdio.h>
#include "cpu.h"

int execute_code (code_t *code, stack *stk)
{
        int ip = 0;
        int temp = 0;
        while (code->cmds[ip] != CMD_HLT)
                switch (code->cmds[ip]) {
                case CMD_PUSH:
                        stack_push(stk, code->cmds[++ip]);
                        ip++;
                        break;
                case CMD_ADD:
                        stack_push(stk, stack_pop(stk) + stack_pop(stk));
                        ip++;
                        break;
                case CMD_SUB:
                        temp = stack_pop(stk);
                        stack_push(stk, stack_pop(stk) - temp);
                        ip++;
                        break;
                case CMD_MUL:
                        stack_push(stk, stack_pop(stk) * stack_pop(stk));
                        ip++;
                        break;
                case CMD_DIV:
                        temp = stack_pop(stk);
                        stack_push(stk, stack_pop(stk) / temp);
                        ip++;
                        break;
                case CMD_OUT:
                        printf("%d\n", stack_pop(stk), ip);
                        ip++;
                        break;
                case CMD_DUMP:
                        stack_dump(stk, (char*) __PRETTY_FUNCTION__, (char*) __FILE__, __LINE__);
                        ip++;
                        break;
                case CMD_HLT:
                        return 0;
                case CMD_JMP:
                        ip = code->cmds[++ip];
                        break;
                default:
                        ip++;
                }
        // if ((value = stack_pop(&stk1)) == POP_ERROR)
        //         return POP_ERROR;

        return 1;
}

int divide_cmds (code_t *code)
{
        int *cmd_list = (int*) calloc(code->n_chars / 2 + 1, sizeof(int));
        if (!code) {
                printf("Calloc returned NULL.\n");
                return NULL_CALLOC;
        }

        int cmd = 0;
        int val = 0;
        int ip = 0;
        int n_chars = 0;
        int i = 0;

        while (cmd != CMD_HLT) {
                sscanf(code->buf + i, "%d %n", &cmd, &n_chars);
                cmd_list[ip++] = cmd;
                switch (cmd) {
                case CMD_PUSH:
                        sscanf(code->buf + i + n_chars, "%d %n", &val, &n_chars);
                        cmd_list[ip++] = val;
                        i += n_chars;
                        break;
                }
                i += n_chars;
        }
        code->cmds = cmd_list;

        return 1;
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
