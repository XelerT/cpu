#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "assembler.h"

#define $ fprintf(stderr, "I'm here. File %s Line %d\n", __FILE__, __LINE__);
// #define $

int main (int argc, char *argv[])
{
        if (int i = check_argv(argc, argv))
                return i;

        FILE *src_code    = nullptr;
        FILE *output_code = nullptr;
        char output_file_name[MAX_NAME_LENGTH] = {};
        src_code    = fopen(argv[2], "r");
        if (!src_code) {
                printf("Source file pointer is null.\n");
                return NO_SOURCE;
        }

        strcpy(output_file_name, argv[1]);
        append_txt(output_file_name);
        output_code = fopen(output_file_name, "w");
        $
        fprintf(output_code, " ");
        $
        code_t code = {};

        labels_t labels[MAX_N_LABELS] = {};
        int *asm_code = (int*) calloc(code.n_lines * 2, sizeof(int));
        if (!asm_code) {
                printf("Calloc return null pointer.\n");
                return NULL_CALLOC;
        }
        get_code(src_code, &code, argv[2]);
        pre_asm(&code);
        $
        if (convert_code(&code, output_code, 0, labels, asm_code) == NO_LABEL) {
                fprintf(stderr, "No label\n");
                return NO_LABEL;
        }

        fclose(output_code);
        fclose(src_code);
        return 0;
}
