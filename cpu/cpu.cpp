#include <stdio.h>
#include "cpu.h"

int execute_code (code_t *code, stack *stk)
{
        int val = 0;
        int cmd = 0;
        // while (code->buf[i]) //skip version
        //         i++;
        for (int i = 0; i < code->n_chars - 4; i++) {
                sscanf(code->buf + i, "%d", &cmd);
        // printf("%d %d\n", i, cmd);
                switch (cmd) {
                case CMD_PUSH:
                        sscanf(code->buf + i + tens_count(cmd), "%d", &val);
                        stack_push(stk, val);
                        i += tens_count(cmd) + 2;
                        break;
                case CMD_ADD:
                        stack_push(stk, stack_pop(stk) + stack_pop(stk));
                        break;
                case CMD_SUB:
                        stack_push(stk, stack_pop(stk) - stack_pop(stk));
                        break;
                case CMD_MUL:
                        stack_push(stk, stack_pop(stk) * stack_pop(stk));
                        break;
                case CMD_DIV:
                        stack_push(stk, stack_pop(stk) / stack_pop(stk));
                        break;
                case CMD_OUT:
                        printf("%d\n", stack_pop(stk));
                        break;
                case CMD_DUMP:
                        stack_dump(stk, (char*) __PRETTY_FUNCTION__, (char*) __FILE__, __LINE__);
                        break;
                case CMD_HLT:
                        return 0;
                }
        }
        // if ((value = stack_pop(&stk1)) == POP_ERROR)
        //         return POP_ERROR;

        return 1;
}

int tens_count (int num)
{
        int counter = 1;
        while (num / 10 != 0) {
                num /= 10;
                counter++;
        }

        return counter;
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
