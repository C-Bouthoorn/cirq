.PHONY: all clean server


all: index.html


clean:
	rm -f *.js *.wasm *.html


server:
	emrun --no_browser --port 8080 .


# Find all C files
DEPS = $(shell find . -name '*.c')
DEPS_HTML = $(DEPS:%.c=%.html)

$(DEPS_HTML): %.html: %.c
	emcc -s WASM=1 $< -o $@
	./remove_svg $@
