#include <stdlib.h>
#include <stdbool.h>

#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef enum {
    T_VAR,
    T_FUN,
} symtable_entry_type;

typedef enum {
    T_INT,
    T_FLOAT,
    T_BOOL,
    T_STR,
    T_NULL
} ret_type;

typedef struct symtable_node_s *symtable_node_ptr;

typedef struct symtable_entry_s{
    symtable_entry_type entry_type;
    
    // common
    ret_type type;
    bool isNullable;
    bool isUsed;
    bool isChanged;
    bool isDefined;

    //variables
    bool isConst;
    bool hasExplicitType;

    // functions
    int param_count;
    bool *param_nullable;
    ret_type *param_types;
    bool returnsValue;
    symtable_node_ptr local_symtable;
} symtable_entry_t, *symtable_entry_ptr;

typedef struct symtable_node_s{
    char *key;
    struct symtable_node_s *left, *right;
    symtable_entry_ptr entry;
    int balance_factor;
} symtable_node_t, **symtable_tree_ptr;

#endif // SYMTABLE_H
