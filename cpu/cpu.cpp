#include <stdio.h>
#include "cpu.h"

#define DEF_STND_CMD(name,num,arg,...) DEF_CMD(name,num,arg,__VA_ARGS__)
#define DEF_J_CMD(name,num,arg,...)    DEF_CMD(name,num,arg,__VA_ARGS__)
#define DEF_CMD(name,num,arg,...)                                      \
                                case num:                              \
                                        if (arg) __VA_ARGS__           \
                                        ip++;                          \
                                        break;
#define PP_CASE         if ((code->cmds[ip] & MASK_CMD) == CMD_PUSH) {                                                      \
                                if (code->cmds[ip] & ARG_IMMED) {                                               \
                                        stack_push(stk, code->cmds[++ip]);                                      \
                                } else if (code->cmds[ip] & ARG_RAM) {                                          \
                                if (code->cmds[ip] & ARG_REG)                                                   \
                                        stack_push(stk, cpu->RAM[cpu->registers[code->cmds[++ip]]]);            \
                                if (code->cmds[ip] & ARG_IMMED)                                                 \
                                        stack_push(stk, cpu->RAM[code->cmds[++ip]]);                            \
                                } else if (code->cmds[ip] & ARG_REG) {                                          \
                                stack_push(stk, cpu->registers[code->cmds[++ip]]);                              \
                                }                                                                               \
                        } else if ((code->cmds[ip] & MASK_CMD) == CMD_POP) {                                               \
                                if (code->cmds[ip] & ARG_RAM) {                                                 \
                                        if (code->cmds[ip] & ARG_REG)                                           \
                                                cpu->RAM[cpu->registers[code->cmds[++ip]]] = stack_pop(stk);    \
                                        if (code->cmds[ip] & ARG_IMMED)                                         \
                                                cpu->RAM[code->cmds[++ip]] = stack_pop(stk);                    \
                                }                                                                               \
                                if (code->cmds[ip] & ARG_REG) {                                                 \
                                        cpu->registers[code->cmds[++ip]] = stack_pop(stk);                      \
                                }                                                                               \
                        }

#define CALL_ stack_push(&call_stack, ip + 2);          \
                        ip = code->cmds[++ip];

#define PUSH(arg) stack_push(stk, arg)
#define POP stack_pop(stk)
#define JMP ip=code->cmds[++ip]; ip--;

int execute_code (code_t *code, stack *stk, cpu_t *cpu)
{
        assert(code);
        assert(stk);
        assert(cpu);

        int ip = 0;
        int temp = 0;
        stack call_stack = {};

        if (oper_stack_ctor(&call_stack, DEFAULT_CAPACITY, (char*) "call_stack",
                            (char*) __PRETTY_FUNCTION__, (char*) __FILE__, __LINE__))
                return CTOR_ERROR;

        while (code->cmds[ip] != CMD_HLT) {
        // printf("WHILE %d %d\n", code->cmds[ip], ip);
                switch (code->cmds[ip] & MASK_CMD) {
                // case CMD_PUSH:
                //         // printf("PUSH1\n");
                //         if (code->cmds[ip] & ARG_IMMED) {
                //                 // printf("PUSH2\n");
                //                 stack_push(stk, code->cmds[++ip]);
                //         } else if (code->cmds[ip] & ARG_RAM) {
                //                 if (code->cmds[ip] & ARG_REG)
                //                         stack_push(stk, cpu->RAM[cpu->registers[code->cmds[++ip]]]);
                //                 if (code->cmds[ip] & ARG_IMMED)
                //                         stack_push(stk, cpu->RAM[code->cmds[++ip]]);
                //         } else if (code->cmds[ip] & ARG_REG) {
                //                 stack_push(stk, cpu->registers[code->cmds[++ip]]);
                //         }
                //         ip++;
                //         break;
//                 case CMD_POP:
//                         // printf("POP\n");
//                         if (code->cmds[ip] & ARG_RAM) {
//                                 if (code->cmds[ip] & ARG_REG)
//                                         cpu->RAM[cpu->registers[code->cmds[++ip]]] = stack_pop(stk);
//                                 if (code->cmds[ip] & ARG_IMMED)
//                                         cpu->RAM[code->cmds[++ip]] = stack_pop(stk);
//
//                         }
//                         if (code->cmds[ip] & ARG_REG) {
//                                 cpu->registers[code->cmds[++ip]] = stack_pop(stk);
//                         }
//                         // if (!(code->cmds[ip] & ARG_RAM) && !(code->cmds[ip] & ARG_REG))
//                         ip++;
//                         break;
#include "..\instructions.en"
                // case CMD_HLT:
                //         // printf("HLT\n");
                //         return 0;
                // case CMD_JMP:
                // // printf("JMP\n");
                //         ip = code->cmds[++ip];
                //         break;
                // case CMD_CALL:
                // // printf("CALL\n");
                //         stack_push(&call_stack, ip + 2);
                //         ip = code->cmds[++ip];
                //         ip++;
                //         break;
                // case CMD_RET:
                // // printf("RET\n");
                //         ip = stack_pop(&call_stack);
                //         break;
                case CMD_CALL_LABEL:
                // printf("CALL_LABEl\n");
                        while (code->cmds[ip] != CMD_RET)
                                ip++;
                        ip++;
                        break;
                default:
                // printf("DEF\n");
                        ip++;
                        // printf("def\n");
                }
        }

        return 0;
}

#undef DEF_STND_CMD
#undef DEF_J_CMD
#undef DEF_CMD
#undef CALL_
#undef PP_CASE
#undef PUSH
#undef POP
#undef JMP

int divide_cmds (code_t *code)
{
        assert(code);

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
                // switch (cmd) {
                // case CMD_PUSH:
                //         sscanf(code->buf + i + n_chars, "%d %n", &val, &n_chars);
                //         cmd_list[ip++] = val;
                //         i += n_chars;
                //         break;
                // }
                i += n_chars;
        }
        code->cmds = cmd_list;

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
