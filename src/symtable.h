#ifndef SYMTABLE_H
#define SYMTABLE_H

/*
 * symtable.h - Symbol Table with Scope Management
 * Supports global and local scopes, arrays, and type info.
 */

#define MAX_NAME      64
#define MAX_SYMBOLS   512
#define MAX_SCOPES    64

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_UNKNOWN
} DataType;

typedef enum {
    SYM_VAR,
    SYM_FUNC,
    SYM_ARRAY,
    SYM_PARAM
} SymKind;

typedef struct Symbol {
    char     name[MAX_NAME];
    DataType type;
    SymKind  kind;
    int      scope_level;
    int      offset;        /* stack/memory offset */
    int      array_size;    /* >0 if array */
    int      param_count;   /* for functions */
    DataType param_types[16];
    int      is_initialized;
    int      line_declared;
} Symbol;

typedef struct Scope {
    Symbol  symbols[MAX_SYMBOLS];
    int     count;
    int     level;
} Scope;

/* Symbol table API */
void     sym_init(void);
void     sym_enter_scope(void);
void     sym_exit_scope(void);
Symbol  *sym_declare(const char *name, DataType type, SymKind kind, int line);
Symbol  *sym_declare_array(const char *name, DataType type, int size, int line);
Symbol  *sym_declare_func(const char *name, DataType ret_type, int param_count,
                          DataType *param_types, int line);
Symbol  *sym_lookup(const char *name);
Symbol  *sym_lookup_current(const char *name);
int      sym_current_scope(void);
void     sym_print(void);

const char *type_to_str(DataType t);
DataType    str_to_type(const char *s);

extern int semantic_errors;

#endif /* SYMTABLE_H */
