# ============================================================
# Makefile - C-Subset Compiler
# Supports: Linux/macOS (flex/bison) and Windows (win_flex/win_bison)
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -g -I$(SRC) -std=gnu99
LDFLAGS = -lm

SRC     = src
OUT     = output
BIN     = compiler

# Detect platform
ifeq ($(OS),Windows_NT)
    FLEX    = win_flex
    BISON   = win_bison
    EXE     = $(BIN).exe
    MKDIR   = if not exist $(OUT) mkdir $(OUT)
    RM      = del /Q
else
    FLEX    = flex
    BISON   = bison
    EXE     = $(BIN)
    MKDIR   = mkdir -p $(OUT)
    RM      = rm -f
endif

# Source files
GEN_C   = $(SRC)/lex.yy.c $(SRC)/parser.tab.c
GEN_H   = $(SRC)/parser.tab.h
SRCS    = $(GEN_C) \
          $(SRC)/ast.c \
          $(SRC)/symtable.c \
          $(SRC)/semantic.c \
          $(SRC)/codegen.c \
          $(SRC)/optimizer.c \
          $(SRC)/main.c

OBJS    = $(SRCS:.c=.o)

# ---- Targets ----

.PHONY: all clean test help

all: $(OUT) $(EXE)

$(OUT):
	$(MKDIR)

# Flex → C
$(SRC)/lex.yy.c: $(SRC)/lexer.l $(GEN_H)
ifeq ($(OS),Windows_NT)
	$(FLEX) --wincompat -o $@ $<
else
	$(FLEX) -o $@ $<
endif

# Bison → C + header
$(SRC)/parser.tab.c $(SRC)/parser.tab.h: $(SRC)/parser.y
	$(BISON) -d -o $(SRC)/parser.tab.c $<

# Compile each object
%.o: %.c $(GEN_H)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build successful: $(EXE)"

# ---- Tests ----

test: all
	@echo "=== Test 1: Basic arithmetic ==="
	./$(EXE) -ast -sym -opt tests/test_basic.c
	@echo ""
	@echo "=== Test 2: Control flow ==="
	./$(EXE) -opt tests/test_control.c
	@echo ""
	@echo "=== Test 3: Functions ==="
	./$(EXE) -opt tests/test_functions.c
	@echo ""
	@echo "=== Test 4: Arrays ==="
	./$(EXE) -opt tests/test_arrays.c
	@echo ""
	@echo "=== Test 5: Optimizer ==="
	./$(EXE) -tac -opt tests/test_optimizer.c
	@echo ""
	@echo "=== All tests complete ==="

clean:
	$(RM) $(SRC)/lex.yy.c $(SRC)/parser.tab.c $(SRC)/parser.tab.h
	$(RM) $(SRC)/*.o
	$(RM) $(EXE)
	$(RM) $(OUT)/*.tac

help:
	@echo "Targets:"
	@echo "  all       Build the compiler"
	@echo "  test      Run all test cases"
	@echo "  clean     Remove generated files"
	@echo ""
	@echo "Compiler options:"
	@echo "  -ast      Dump AST"
	@echo "  -sym      Dump symbol table"
	@echo "  -tac      Dump TAC before optimization"
	@echo "  -opt      Dump TAC after optimization"
	@echo "  -no-opt   Skip optimization"
	@echo "  -o <f>    Output file (default: output/out.tac)"
