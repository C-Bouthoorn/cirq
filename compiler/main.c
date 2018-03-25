#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>


#define BITCODE(x) const uint8_t x

BITCODE(MOV) = 0010;
BITCODE(ADD) = 0011;
BITCODE(MUL) = 0012;

BITCODE(REGA) = 0100;
BITCODE(REGB) = 0101;
BITCODE(REGC) = 0102;
BITCODE(REGD) = 0103;

BITCODE(DEBUG) = 0111;



bool isnumber(char *str) {
    assert(str != NULL);

    size_t len = strlen(str), i = 0;

    if (len == 0) return false;

    // binary, octal, decimal or hexadecimal
    if (str[i] == 'b' || str[i] == 'o' || str[i] == 'd' || str[i] == 'h') {
        i++;
    }

    // sign
    if (str[i] == '-' || str[i] == '+') {
        i++;
    }

    for (; i < len; i++) {
        if (! isdigit(str[i])) {
            return false;
        }
    }

    return true;
}


int16_t tonumber(char *str) {
    assert(str != NULL);
    assert(isnumber(str));

    int16_t num = 0;
    uint8_t base = 10;
    bool is_positive = true;

    size_t len = strlen(str), i = 0;

    // binary, octal, decimal or hexadecimal
    if (str[i] == 'b' || str[i] == 'o' || str[i] == 'd' || str[i] == 'h') {
        i++;
    }

    // sign
    if (str[i] == '-' || str[i] == '+') {
        if (str[i] == '-') {
            is_positive = false;
        }

        i++;
    }

    for (; i < len; i++) {
        char c = str[i];

        int value;
        if (c >= '0' && c <= '9') {
            value = c - '0';
        }
        else if (c >= 'a' && c <= 'f') {
            value = c - 'a';
        }
        else if (c >= 'A' && c <= 'F') {
            value = c - 'A';
        }
        else {
            fprintf(stderr, "Failed to convert number: invalid base %u for char %c\n", base, c);
            exit(1);
        }

        num *= base;
        num += value;
    }


    if (! is_positive) {
        num *= -1;
    }

    return num;
}


int compile(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: compiler <inputfile> <outputfile>\n");
        return 1;
    }


    char *input_filename = argv[1];
    FILE *input_file = fopen(input_filename, "r");

    if (input_file == NULL) {
        fprintf(stderr, "Failed to open input file %s\n", input_filename);
        return 2;
    }


    char *output_filename = argv[2];
    FILE *output_file;

    if (strcmp(output_filename, "-") == 0) {
        output_file = stdout;
    }
    else {
        output_file = fopen(output_filename, "wb");

        if (output_file == NULL) {
            fprintf(stderr, "Failed to open output file %s\n", output_filename);
            return 2;
        }
    }


    char *line;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, input_file)) >= 0) {
        // Remove newline
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // Ignore empty lines and comments
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        printf("\n%s\n", line);

        // Split line in parts
        char *str, *tofree;
        str = tofree = strdup(line);

        char *tokens[10];
        size_t amount_tokens = 0;

        char *token;

        while ((token = strsep(&str, " ")) != NULL) {
            // printf("%s\n", token);
            tokens[amount_tokens++] = token;
        }

        char *cmd = tokens[0];
        // printf("cmd: %s\n", cmd);

        uint8_t bitcode = 0;

        #define cmp(stra, strb, bc)  else if (strcmp(stra, strb) == 0) bitcode = bc

        if (0) {}
        cmp(cmd, "mov", MOV);
        cmp(cmd, "add", ADD);
        cmp(cmd, "mul", MUL);
        cmp(cmd, "debug", DEBUG);

        else {
            fprintf(stderr, "Failed to convert token `%s`\n", cmd);
        }

        printf("%02x ", bitcode);
        fwrite(&bitcode, 1, sizeof(bitcode), output_file);


        for (size_t i=1; i < amount_tokens; i++) {
            token = tokens[i];
            bitcode = 0;

            if (0) {}
            cmp(token, "rega", REGA);
            cmp(token, "regb", REGB);
            cmp(token, "regc", REGC);
            cmp(token, "regd", REGD);

            else if (isnumber(token)) {
                bitcode = (uint8_t) tonumber(token);
            }
            else if (strlen(token) == 0) {
                // ignore
                continue;
            }
            else {
                fprintf(stderr, "Failed to convert token `%s`\n", token);
            }


            printf("%02x ", bitcode);
            fwrite(&bitcode, 1, sizeof(bitcode), output_file);
        }

        printf("\b\n");
        free(tofree);
    }

    free(line);

    fclose(input_file);
    fclose(output_file);
    return 0;
}



int run(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: compiler_run <inputfile>");
        return 1;
    }

    char *input_filename = argv[1];
    FILE *input_file = fopen(input_filename, "rb");

    // 0-512: instructions
    char stack[1024];
    size_t stack_amount=0;

    char regs[4];

    char c;
    while ((c = (char) fgetc(input_file)) != EOF) {
        stack[stack_amount++] = c;
    }

    fclose(input_file);


    size_t ip = 0;

    while (ip < stack_amount) {
        char cmd = stack[ip];

        #define get_dest(dest, loc) \
            switch (loc) { \
                case REGA: \
                    dest = &regs[0]; \
                    break; \
                case REGB: \
                    dest = &regs[1]; \
                    break; \
                case REGC: \
                    dest = &regs[2]; \
                    break; \
                case REGD: \
                    dest = &regs[3]; \
                    break; \
                default: \
                    fprintf(stderr, "No idea which variable to add"); \
                    continue; \
            }


        if (cmd == MOV) {
            char args[] = { stack[ip+1], stack[ip+2] };
            ip += 2;

            char *dest;
            get_dest(dest, args[0]);

            *dest = args[1];
        }

        else if (cmd == ADD) {
            char args[] = { stack[ip+1], stack[ip+2] };
            ip += 2;

            char *dest;
            get_dest(dest, args[0]);

            *dest += args[1];
        }

        else if (cmd == MUL) {
            char args[] = { stack[ip+1], stack[ip+2] };
            ip += 2;

            char *dest;
            get_dest(dest, args[0]);

            *dest *= args[1];
        }

        else if (cmd == DEBUG) {
            char args[] = { stack[ip+1] };
            ip += 1;

            char *dest;
            get_dest(dest, args[0]);

            if (args[0] >= REGA && args[0] <= REGD) {
                char regc = args[0] - REGA + 'A';

                printf("REG%c %i\n", regc, *dest);
            }
            else {
                printf("%x %i\n", args[0], *dest);
            }
        }

        else {
            fprintf(stderr, "Failed to exec cmd %x\n", cmd);
        }

        ip++;
    }


    return 0;
}




int main(int argc, char *argv[]) {
    #ifdef RUN
        return run(argc, argv);
    #else
        return compile(argc, argv);
    #endif
}
