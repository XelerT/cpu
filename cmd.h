#ifndef CMD_H
#define CMD_H

// DEFINE_REG(DICK, 0x0, 'big_dick', HIDDEN, )

enum registers {
        N_REGS  = 8,
        REG_RAX = 1,
        REG_RBX = 2,
        REG_RCX = 3,
        REG_RDX = 4,
        REG_REX = 5,
        REG_RFX = 6,
        REG_RGX = 7,
        REG_RHX = 8
};

enum args {
        ARG_IMMED = 1 << 4,
        ARG_REG   = 1 << 5,
        ARG_RAM   = 1 << 6
};

// /* Instruction encoding */
// DEFINE_CMD(PUSH, 0xF, "push", ...)

enum cmd {
        CMD_PUSH  =  1,
        CMD_PUSH_REG   = ARG_REG   + CMD_PUSH,
        CMD_PUSH_RAM   = ARG_RAM   + CMD_PUSH,
        CMD_PUSH_IMMED = ARG_IMMED + CMD_PUSH,
        CMD_POP   =  2,
        CMD_POP_REG   = ARG_REG   + CMD_POP,
        CMD_POP_RAM   = ARG_RAM   + CMD_POP,
        CMD_ADD   =  3,
        CMD_SUB   =  4,
        CMD_MUL   =  5,
        CMD_DIV   =  6,
        CMD_OUT   =  7,
        CMD_IN    =  8,
        CMD_HLT   =  9,
        CMD_DUMP  = 10,
        CMD_JMP   = 11,
        CMD_LABEL = 12
};

enum errors {
        NO_REGISTER    = -7,
        DIVIDE_ERR     = -6, //
        NO_LABEL       = -5, // 0x Hex speak
        NULL_CALLOC    = -4, // 0xBADALL0C
        EXECUTE_ERR    = -3,
        NO_SOURCE      = -2,
        NO_OUTPUT_NAME = -1
};

static const int MAX_NAME_LENGTH = 20;

#endif /*CMD_H*/
