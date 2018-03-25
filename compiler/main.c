#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>


#define BITCODE(name) const uint8_t name

BITCODE(BYTE) = 0044;
BITCODE(REG) = 0100;

BITCODE(INC) = 0151;
BITCODE(MOV) = 0075;
BITCODE(ADD) = 0053;
BITCODE(SUB) = 0055;
BITCODE(MUL) = 0052;
BITCODE(SQ) = 0062;

BITCODE(REGA) = 0101;
BITCODE(REGB) = 0102;
BITCODE(REGC) = 0103;
BITCODE(REGD) = 0104;

BITCODE(DEBUG) = 0140;



bool isnumber(char *str) {
    assert(str != NULL);

    size_t len = strlen(str), i = 0;
    uint8_t base = 10;

    if (len == 0) return false;

    // binary, octal, decimal or hexadecimal
    if (str[i] == 'b' || str[i] == 'o' || str[i] == 'd' || str[i] == 'h') {
        if (str[i] == 'b') base = 2;
        else if (str[i] == 'o') base = 8;
        else if (str[i] == 'd') base = 10;
        else if (str[i] == 'h') base = 16;

        i++;
    }

    // sign
    if (str[i] == '-' || str[i] == '+') {
        i++;
    }

    for (; i < len; i++) {
        char digit = str[i];

        if (base == 2 && !(digit >= '0' && digit <= '1')) {
            return false;
        }
        else if (base == 8 && !(digit >= '0' && digit <= '7')) {
            return false;
        }
        else if (base == 10 && !(digit >= '0' && digit <= '9')) {
            return false;
        }
        else if (base == 16 && !((digit >= '0' && digit <= '9') || (digit >= 'a' && digit <= 'f') || (digit >= 'A' && digit <= 'F'))) {
            return false;
        }
    }

    return true;
}


int8_t tonumber(char *str) {
    assert(str != NULL);
    if (! isnumber(str)) {
        fprintf(stderr, "%s is not a number\n", str);
        assert(false);
    }

    int8_t num = 0;
    uint8_t base = 10;
    bool is_positive = true;

    size_t len = strlen(str), i = 0;

    // binary, octal, decimal or hexadecimal
    if (str[i] == 'b' || str[i] == 'o' || str[i] == 'd' || str[i] == 'h') {
        if (str[i] == 'b') base = 2;
        else if (str[i] == 'o') base = 8;
        else if (str[i] == 'd') base = 10;
        else if (str[i] == 'h') base = 16;

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
            value = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F') {
            value = c - 'A' + 10;
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
        cmp(cmd, "sub", SUB);
        cmp(cmd, "mul", MUL);
        cmp(cmd, "sq", SQ);
        cmp(cmd, "inc", INC);
        cmp(cmd, "debug", DEBUG);

        else {
            fprintf(stderr, "Failed to convert token `%s`\n", cmd);
            exit(1);
        }

        printf("%02x ", bitcode);
        fwrite(&bitcode, 1, sizeof(bitcode), output_file);


        for (size_t i=1; i < amount_tokens; i++) {
            uint8_t type = REG;

            token = tokens[i];
            bitcode = 0;

            if (0) {}
            cmp(token, "rega", REGA);
            cmp(token, "regb", REGB);
            cmp(token, "regc", REGC);
            cmp(token, "regd", REGD);

            else if (isnumber(token)) {
                type = BYTE;
                bitcode = (uint8_t) tonumber(token);
            }
            else if (strlen(token) == 0) {
                // ignore
                continue;
            }
            else {
                fprintf(stderr, "Failed to convert token `%s`\n", token);
                exit(1);
            }


            printf("%04x ", type << 8 | bitcode);
            fwrite(&type, 1, sizeof(type), output_file);
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

    char stack[1024];
    size_t stack_amount = 0;

    char regs[4] = {0, 0, 0, 0};

    char c;
    while ((c = (char) fgetc(input_file)) != EOF) {
        stack[stack_amount++] = c;
    }

    fclose(input_file);

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
                fprintf(stderr, "Unable to find destination"); \
                exit(1); \
        }

    size_t ip = 0;

    while (ip < stack_amount) {
        char cmd = stack[ip];

        if (cmd == MOV) {
            char args[] = { stack[ip+1], stack[ip+2], stack[ip+3], stack[ip+4] };
            ip += 4;

            char *dest;
            assert(args[0] == REG && "First argument to MOV has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                char *from;
                get_dest(from, args[3]);

                *dest = *from;
            }
            else if (args[2] == BYTE) {
                *dest = args[3];
            }
            else {
                fprintf(stderr, "Unknown type");
                exit(1);
            }
        }

        else if (cmd == ADD) {
            char args[] = { stack[ip+1], stack[ip+2], stack[ip+3], stack[ip+4] };
            ip += 4;

            char *dest;
            assert(args[0] == REG && "First argument to ADD has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                char *from;
                get_dest(from, args[3]);

                *dest += *from;
            }
            else if (args[2] == BYTE) {
                *dest += args[3];
            }
            else {
                fprintf(stderr, "Unknown type");
                exit(1);
            }
        }

        else if (cmd == SUB) {
            char args[] = { stack[ip+1], stack[ip+2], stack[ip+3], stack[ip+4] };
            ip += 4;

            char *dest;
            assert(args[0] == REG && "First argument to SUB has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                char *from;
                get_dest(from, args[3]);

                *dest -= *from;
            }
            else if (args[2] == BYTE) {
                *dest -= args[3];
            }
            else {
                fprintf(stderr, "Unknown type");
                exit(1);
            }
        }

        else if (cmd == MUL) {
            char args[] = { stack[ip+1], stack[ip+2], stack[ip+3], stack[ip+4] };
            ip += 4;

            char *dest;
            assert(args[0] == REG && "First argument to MUL has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                char *from;
                get_dest(from, args[3]);

                *dest *= *from;
            }
            else if (args[2] == BYTE) {
                *dest *= args[3];
            }
            else {
                fprintf(stderr, "Unknown type");
                exit(1);
            }
        }

        else if (cmd == SQ) {
            char args[] = { stack[ip+1], stack[ip+2] };
            ip += 2;

            char *dest;
            assert(args[0] == REG && "First argument to SQ has to be register");
            get_dest(dest, args[1]);

            *dest *= *dest;
        }

        else if (cmd == INC) {
            char args[] = { stack[ip+1], stack[ip+2] };
            ip += 2;

            char *dest;
            assert(args[0] == REG && "First argument to INC has to be register");
            get_dest(dest, args[1]);

            *dest += 1;
        }

        else if (cmd == DEBUG) {
            char args[] = { stack[ip+1], stack[ip+2] };
            ip += 2;

            if (args[0] == REG) {
                char *dest;
                get_dest(dest, args[1]);

                char regc = args[1] - REGA + 'A';

                printf("REG%c %i\n", regc, *dest);
            }
            else if (args[1] == BYTE) {
                printf("0x%04x\n", args[1]);
            }
            else {
                fprintf(stderr, "Unknown type while debugging");
                exit(1);
            }
        }

        else {
            fprintf(stderr, "Failed to exec cmd %x\n", cmd);
            exit(1);
        }

        ip++;
    }


    return 0;
}


void test() {
    assert(tonumber("b10") == 0b10);
    assert(tonumber("o10") == 010);
    assert(tonumber("d-10") == -10);
    assert(tonumber("h10") == 0x10);

    assert(tonumber("h09") == 0x09);
    assert(tonumber("h7f") == 0x7f);
    assert(tonumber("b101010") == 0b101010);
    assert(tonumber("o76") == 076);
}




int main(int argc, char *argv[]) {
    test();

    #ifdef RUN
        return run(argc, argv);
    #else
        return compile(argc, argv);
    #endif
}
