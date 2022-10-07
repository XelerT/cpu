#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "assembler.h"

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

        code_t code = {};

        get_code(src_code, &code, argv[2]);
        convert_code(&code, output_code);

        fclose(src_code);
        fclose(output_code);
        return 0;
}
