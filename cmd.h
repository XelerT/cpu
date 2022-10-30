#ifndef CMD_H
#define CMD_H

// DEFINE_REG(DICK, 0x0, 'big_dick', HIDDEN, )
#define DEF_REG(name,num) REG_##name = num,

enum registers {
        #include "registers.en"
        N_REGS  = 8
};
#undef DEF_REG

enum args {
        ARG_IMMED = 1 << 5, //0010 0000
        ARG_REG   = 1 << 6, //0100 0000
        ARG_RAM   = 1 << 7  //1000 0000
};

enum masks {
        MASK_CMD  = 0x1F
};

// /* Instruction encoding */
// DEFINE_CMD(PUSH, 0xF, "push", ...)

#define DEF_CMD(name,num,...) CMD_##name = num,
#define DEF_STND_CMD(name,num,...) CMD_##name = num,
#define DEF_J_CMD(name,num,...) CMD_##name = num,
enum cmd {
        #include "instructions.en"
        // CMD_PUSH       =  1,
        // CMD_POP        =  2,
        // //CMD_ADD        =  3,
        // //CMD_SUB        =  4,
        // //CMD_MUL        =  5,
        // //CMD_DIV        =  6,
        // //CMD_OUT        =  7,
        // //CMD_IN         =  8,
        // CMD_HLT        =  9,
        // //CMD_DUMP       = 10,
        // CMD_JMP        = 11,
        CMD_LABEL      = 12,
        // CMD_CALL       = 13,
        CMD_CALL_LABEL = 14
        // CMD_RET        = 15
        // // CMD_NULL = 0
};
#undef DEF_CMD
#undef DEF_STND_CMD
#undef DEF_J_CMD

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
