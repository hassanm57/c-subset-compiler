/*
 * main.c - C-Subset Compiler Driver
 *
 * Usage:
 *   compiler [options] <input.c>
 *
 * Options:
 *   -ast        Dump AST after parsing
 *   -sym        Dump symbol table after semantic analysis
 *   -tac        Dump TAC before optimization
 *   -opt        Dump TAC after optimization
 *   -no-opt     Skip optimization pass
 *   -o <file>   Write TAC output to file (default: output/out.tac)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtable.h"
#include "semantic.h"
#include "codegen.h"
#include "optimizer.h"

extern FILE *yyin;
extern int   yyparse(void);
extern ASTNode *ast_root;
extern int   line_num;

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s [options] <input.c>\n"
        "Options:\n"
        "  -ast      Dump AST\n"
        "  -sym      Dump symbol table\n"
        "  -tac      Dump TAC (before opt)\n"
        "  -opt      Dump TAC (after opt)\n"
        "  -no-opt   Skip optimization\n"
        "  -o <f>    Output file (default: output/out.tac)\n",
        prog);
}

int main(int argc, char *argv[]) {
    const char *input_file  = NULL;
    const char *output_file = "output/out.tac";
    int show_ast  = 0;
    int show_sym  = 0;
    int show_tac  = 0;
    int show_opt  = 0;
    int do_opt    = 1;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-ast"))    show_ast = 1;
        else if (!strcmp(argv[i], "-sym"))    show_sym = 1;
        else if (!strcmp(argv[i], "-tac"))    show_tac = 1;
        else if (!strcmp(argv[i], "-opt"))    show_opt = 1;
        else if (!strcmp(argv[i], "-no-opt")) do_opt   = 0;
        else if (!strcmp(argv[i], "-o") && i+1 < argc)
            output_file = argv[++i];
        else if (argv[i][0] != '-')
            input_file = argv[i];
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (!input_file) {
        usage(argv[0]);
        return 1;
    }

    /* ---- Phase 1: Lexing + Parsing ---- */
    yyin = fopen(input_file, "r");
    if (!yyin) {
        perror(input_file);
        return 1;
    }
    printf("[Parser] Parsing '%s'...\n", input_file);
    int parse_result = yyparse();
    fclose(yyin);

    if (parse_result != 0 || !ast_root) {
        fprintf(stderr, "[Compiler] Parsing failed.\n");
        return 1;
    }
    printf("[Parser] Parsing successful.\n");

    if (show_ast) {
        printf("\n===== Abstract Syntax Tree =====\n");
        ast_print(ast_root, 0);
        printf("================================\n\n");
    }

    /* ---- Phase 2: Semantic Analysis ---- */
    printf("[Semantic] Running semantic analysis...\n");
    semantic_analyze(ast_root);

    if (show_sym) {
        sym_print();
    }

    if (semantic_errors > 0) {
        fprintf(stderr, "[Compiler] Semantic errors — aborting code generation.\n");
        ast_free(ast_root);
        return 1;
    }

    /* ---- Phase 3: TAC Code Generation ---- */
    printf("[Codegen] Generating Three-Address Code...\n");
    TACList tac;
    tac_init(&tac);
    tac_generate(&tac, ast_root);
    printf("[Codegen] Generated %d TAC instructions.\n", tac.count);

    if (show_tac) {
        tac_print(&tac, stdout);
    }

    /* ---- Phase 4: Optimization ---- */
    if (do_opt) {
        optimize(&tac);
        if (show_opt) {
            printf("\n===== Optimized TAC =====\n");
            tac_print(&tac, stdout);
        }
    }

    /* ---- Output ---- */
    tac_print_file(&tac, output_file);
    printf("[Compiler] Output written to '%s'\n", output_file);
    printf("[Compiler] Compilation complete.\n");

    ast_free(ast_root);
    return 0;
}
