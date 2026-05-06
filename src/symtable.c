/*
 * symtable.c - Symbol Table Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

static Scope scope_stack[MAX_SCOPES];
static int   scope_top = -1;
static int   next_offset = 0;

int semantic_errors = 0;

void sym_init(void) {
    scope_top  = -1;
    next_offset = 0;
    sym_enter_scope(); /* global scope (level 0) */
}

void sym_enter_scope(void) {
    if (scope_top >= MAX_SCOPES - 1) {
        fprintf(stderr, "[Sym] Scope stack overflow\n");
        exit(1);
    }
    scope_top++;
    scope_stack[scope_top].count = 0;
    scope_stack[scope_top].level = scope_top;
}

void sym_exit_scope(void) {
    if (scope_top <= 0) {
        fprintf(stderr, "[Sym] Cannot exit global scope\n");
        return;
    }
    scope_top--;
}

int sym_current_scope(void) {
    return scope_top;
}

static Symbol *alloc_in_current(void) {
    Scope *cur = &scope_stack[scope_top];
    if (cur->count >= MAX_SYMBOLS) {
        fprintf(stderr, "[Sym] Symbol table overflow in scope %d\n", scope_top);
        exit(1);
    }
    return &cur->symbols[cur->count++];
}

Symbol *sym_declare(const char *name, DataType type, SymKind kind, int line) {
    /* Check for re-declaration in same scope */
    Scope *cur = &scope_stack[scope_top];
    for (int i = 0; i < cur->count; i++) {
        if (strcmp(cur->symbols[i].name, name) == 0) {
            fprintf(stderr,
                "[Semantic Error] Line %d: Re-declaration of '%s' in same scope "
                "(first declared at line %d)\n",
                line, name, cur->symbols[i].line_declared);
            semantic_errors++;
            return &cur->symbols[i];
        }
    }
    Symbol *s = alloc_in_current();
    strncpy(s->name, name, MAX_NAME - 1);
    s->name[MAX_NAME - 1] = '\0';
    s->type         = type;
    s->kind         = kind;
    s->scope_level  = scope_top;
    s->offset       = next_offset;
    s->array_size   = 0;
    s->param_count  = 0;
    s->is_initialized = 0;
    s->line_declared  = line;
    if (kind == SYM_VAR || kind == SYM_PARAM) {
        next_offset += (type == TYPE_FLOAT) ? 8 : 4;
    }
    return s;
}

Symbol *sym_declare_array(const char *name, DataType type, int size, int line) {
    Symbol *s = sym_declare(name, type, SYM_ARRAY, line);
    if (s) {
        s->array_size = size;
        next_offset += (type == TYPE_FLOAT ? 8 : 4) * (size - 1); /* already added 1 unit */
    }
    return s;
}

Symbol *sym_declare_func(const char *name, DataType ret_type,
                          int param_count, DataType *param_types, int line) {
    Symbol *s = sym_declare(name, ret_type, SYM_FUNC, line);
    if (s) {
        s->param_count = param_count;
        for (int i = 0; i < param_count && i < 16; i++)
            s->param_types[i] = param_types[i];
    }
    return s;
}

Symbol *sym_lookup(const char *name) {
    for (int lvl = scope_top; lvl >= 0; lvl--) {
        Scope *sc = &scope_stack[lvl];
        for (int i = 0; i < sc->count; i++) {
            if (strcmp(sc->symbols[i].name, name) == 0)
                return &sc->symbols[i];
        }
    }
    return NULL;
}

Symbol *sym_lookup_current(const char *name) {
    Scope *cur = &scope_stack[scope_top];
    for (int i = 0; i < cur->count; i++) {
        if (strcmp(cur->symbols[i].name, name) == 0)
            return &cur->symbols[i];
    }
    return NULL;
}

const char *type_to_str(DataType t) {
    switch (t) {
        case TYPE_INT:   return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_CHAR:  return "char";
        case TYPE_VOID:  return "void";
        default:         return "unknown";
    }
}

DataType str_to_type(const char *s) {
    if (strcmp(s, "int")   == 0) return TYPE_INT;
    if (strcmp(s, "float") == 0) return TYPE_FLOAT;
    if (strcmp(s, "char")  == 0) return TYPE_CHAR;
    if (strcmp(s, "void")  == 0) return TYPE_VOID;
    return TYPE_UNKNOWN;
}

void sym_print(void) {
    printf("\n===== Symbol Table =====\n");
    printf("%-20s %-8s %-8s %-6s %-8s\n",
           "Name", "Type", "Kind", "Scope", "Offset");
    printf("%-20s %-8s %-8s %-6s %-8s\n",
           "----", "----", "----", "-----", "------");
    for (int lvl = 0; lvl <= scope_top; lvl++) {
        Scope *sc = &scope_stack[lvl];
        for (int i = 0; i < sc->count; i++) {
            Symbol *s = &sc->symbols[i];
            const char *kind_str = "var";
            if (s->kind == SYM_FUNC)  kind_str = "func";
            if (s->kind == SYM_ARRAY) kind_str = "array";
            if (s->kind == SYM_PARAM) kind_str = "param";
            printf("%-20s %-8s %-8s %-6d %-8d",
                   s->name, type_to_str(s->type), kind_str,
                   s->scope_level, s->offset);
            if (s->kind == SYM_ARRAY)
                printf("  [size=%d]", s->array_size);
            if (s->kind == SYM_FUNC)
                printf("  params=%d", s->param_count);
            printf("\n");
        }
    }
    printf("========================\n\n");
}
