# The path of the executable
OUTPUT_EXEC = build/main

# Use clang by default
CC = clang

# Flags for Clang
CFLAGS = -O0 -g -Weverything

# Disable annoying warnings
ANNOYING_WARNINGS = padded variadic-macros gnu-zero-variadic-macro-arguments
IGNORE_FLAGS = $(ANNOYING_WARNINGS:%=-Wno-%)

# Flags for GCC
GCC_FLAGS = -O0 -g -Wall -Wextra

# Default valgrind flags
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all -v

# Colors
RED := $(shell echo -ne "\e[0;31m")
LIGHT_RED := $(shell echo -ne "\e[0;91m")
GREEN := $(shell echo -ne "\e[0;32m")
LIGHT_GREEN := $(shell echo -ne "\e[0;92m")
YELLOW := $(shell echo -ne "\e[0;33m")
LIGHT_YELLOW := $(shell echo -ne "\e[0;93m")
BLUE := $(shell echo -ne "\e[0;34m")
LIGHT_BLUE := $(shell echo -ne "\e[0;94m")
MAGENTA := $(shell echo -ne "\e[0;35m")
LIGHT_MAGENTA := $(shell echo -ne "\e[0;95m")
CYAN := $(shell echo -ne "\e[0;36m")
LIGHT_CYAN := $(shell echo -ne "\e[0;96m")

WHITE := $(shell echo -ne "\e[0;97m")
LIGHT_GRAY := $(shell echo -ne "\e[0;37m")
DARK_GRAY := $(shell echo -ne "\e[0;90m")
BLACK := $(shell echo -ne "\e[0;30m")
RESET := $(shell echo -ne "\e[0m")

TAB := $(shell echo -ne "\t")

# All targets that are not files
.PHONY: help asan all shit bench bench_build valgrind valshit run clean remake cleanrun


# Show some help
help:
	@echo -e "make help      show this help"
	@echo -e "make all       build all files with ASAN on clang"
	@echo -e "make shit      build all files with ASAN on gcc"
	@echo -e "make bench     build all files with ASAN on clang with benchmarking on"
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


bench_build: CFLAGS = -O3 -DBENCH
bench_build: cleanrun

bench:
	@rm -f /tmp/bench
	@touch /tmp/bench
	@make --no-print-directory bench_build
	@rm -f /tmp/bench.tmp
	@sort -n /tmp/bench
	@rm -f /tmp/bench


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valgrind: clean $(OUTPUT_EXEC)
	valgrind $(VALGRIND_FLAGS) $(OUTPUT_EXEC)


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valshit: CC = gcc
valshit: CFLAGS = $(GCC_FLAGS)
valshit: valgrind


# Just run the executable
run:
	$(OUTPUT_EXEC)


# Clean all build files
clean:
	@echo "${RED}rm -f $(OUTPUT_EXEC) build/*${RESET}"
	@rm -f $(OUTPUT_EXEC) build/*


# Remove all files, then build again
remake: clean all


# Remove all files, build, then run
cleanrun: remake
	@echo -e "\n\n"
	$(OUTPUT_EXEC)


# Find all C files
DEPS = $(shell find src -name '*.c')

# Change src/%.c to build/%.o
DEPS_O = $(DEPS:src/%.c=build/%.o)


# Build dependencies
$(DEPS_O): build/%.o: src/%.c
	@echo "$(CC)${TAB}${YELLOW}$(CFLAGS) ${DARK_GRAY}$(IGNORE_FLAGS)"
	@echo "${TAB}${LIGHT_CYAN}-c" $< "${TAB}${LIGHT_GREEN}-o" $@
	@echo "${RESET}"
	@$(CC)  $(CFLAGS) $(IGNORE_FLAGS) -c $< -o $@


# Build main executable
$(OUTPUT_EXEC): $(DEPS_O)
	@echo "$(CC)${TAB}${YELLOW}$(CFLAGS) ${DARK_GRAY}$(IGNORE_FLAGS)"
	@echo "${TAB}${LIGHT_GREEN}$(DEPS_O)"
	@echo "${TAB}${LIGHT_MAGENTA}-o $(OUTPUT_EXEC)"
	@echo "${RESET}"
	@$(CC)  $(CFLAGS) $(IGNORE_FLAGS) $(DEPS_O) -o $(OUTPUT_EXEC)
