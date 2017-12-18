# The path of the executable
OUTPUT_EXEC = build/main

# Use clang by default
CC = clang

# Flags for Clang
CFLAGS = -O0 -g -Weverything

# Disable annoying warnings
ANNOYING_WARNINGS = padded gnu-zero-variadic-macro-arguments variadic-macros
CFLAGS += $(ANNOYING_WARNINGS:%=-Wno-%)

# Flags for GCC
GCC_FLAGS = -O0 -g -Wall -Wextra

# Default valgrind flags
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all -v


# All targets that are not files
.PHONY: all gcc shit asan help run valgrind clean remake cleanrun


# Show some help
help:
	@echo -e "make help      show this help"
	@echo -e "make all       build all files with ASAN on clang"
	@echo -e "make shit      build all files with ASAN on gcc"
	@echo -e "make valgrind  build all files (clang), then run with valgrind"
	@echo -e "make valshit   build all files (gcc), then run with valgrind"
	@echo -e "make run       just run the executable"
	@echo -e "make clean     remove all build files"
	@echo -e "make remake    same as make clean; make all"
	@echo -e "make cleanrun  same as make remake, but runs formed executable directly after"


# ASAN
asan: CFLAGS += -fsanitize=address
asan: $(OUTPUT_EXEC)


# Clang + ASAN
all: asan


# GCC + ASAN
shit: CC = gcc
shit: CFLAGS = $(GCC_FLAGS)
shit: asan


# Just run the executable
run:
	$(OUTPUT_EXEC)


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valgrind: clean $(OUTPUT_EXEC)
	valgrind $(VALGRIND_FLAGS) $(OUTPUT_EXEC)


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valshit: CC = gcc
valshit: CFLAGS = $(GCC_FLAGS)
valshit: valgrind

# Clean all build files
clean:
	rm -f $(OUTPUT_EXEC) build/*


# Remove all files, then build again
remake: clean all


# Remove all files, build, then run
cleanrun: remake
	@echo -e "\n\n"
	$(OUTPUT_EXEC)


# Build all dependencies

# Find all C files
DEPS = $(shell find src -name '*.c')
# Change src/ to build/ and .c to .o
DEPS_O = $(DEPS:src/%.c=build/%.o)

$(OUTPUT_EXEC): $(DEPS_O)
	$(CC)  $(CFLAGS)	$(DEPS_O)	-o $(OUTPUT_EXEC)

$(DEPS_O): build/%.o: src/%.c
	$(CC)  $(CFLAGS)	-c $<	-o $@
