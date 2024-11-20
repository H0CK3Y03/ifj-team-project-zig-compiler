#include <stdlib.h>
#include <stdbool.h>

#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef enum {
    T_VAR_SYM,
    T_FUN_SYM,
} symtable_entry_type;

typedef enum {
    T_INT_RET,
    T_FLOAT_RET,
    T_BOOL_RET,
    T_STR_RET,
    T_NULL_RET
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

symtable_node_ptr symNodeCreate(char *key);

void symtable_dispose(symtable_node_ptr root);

char* my_str_dup(char* key);

int max(int a, int b);

int height(symtable_node_ptr node);

int get_balance (symtable_node_ptr node);

void symtable_free_entry(symtable_entry_ptr entry);

void symtable_init(symtable_tree_ptr tree);

void symtable_insert(symtable_node_ptr node);

void symtable_remove(symtable_node_ptr node);

void simple_right_rot(symtable_tree_ptr root);

void simple_left_rot(symtable_tree_ptr root);

void right_left_rot(symtable_tree_ptr root);

void left_right_rot(symtable_tree_ptr root);

void rebalance(symtable_tree_ptr root);

#endif // SYMTABLE_H
