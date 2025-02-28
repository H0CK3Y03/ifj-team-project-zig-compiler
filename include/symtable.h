/**
 * file: symtable.h
 * author: Roman Andraščík xandrar00
 * Header file for symtable.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"

#ifndef SYMTABLE_H
#define SYMTABLE_H


#define DEFAULT_FUNCTION_PARAM_SIZE 4
#define DEFAULT_INDEX_SIZE 0

typedef enum {
    T_VAR_SYM,
    T_FUN_SYM,
} symtable_entry_type;

typedef enum {
    T_INT_RET,
    T_FLOAT_RET,
    T_BOOL_RET,
    T_STR_RET,
    T_NULL_RET,
    T_UNREACHABLE_RET,
    T_ERROR_RET,
    T_WHILE_RET,
    T_ANY
} ret_type;

struct symtable_node_s;
typedef struct symtable_node_s *symtable_node_ptr;
typedef struct symtable_node_s **symtable_tree_ptr;

extern symtable_node_ptr SymFunctionTree;

typedef struct symtable_entry_s {
    symtable_entry_type entry_type;
    
    // common
    ret_type type;
    bool isUsed;
    bool isNullable;

    // variables
    bool isConst;
    bool isConvertable;
    bool hasExplicitType;
    bool isChanged;
    bool isNull;
    int scopeLevel;

    // functions
    int param_count;
    bool *param_nullable;
    ret_type *param_types;
    bool returnsValue;
    bool hasReturn;
    symtable_tree_ptr local_symtable;
} symtable_entry_t, *symtable_entry_ptr;

typedef struct symtable_node_s{
    char *key;
    struct symtable_node_s *left, *right;
    symtable_entry_ptr entry;
    int balance_factor;
} symtable_node_t;

// Creates new node
symtable_node_ptr symtable_node_create(char *key, symtable_entry_type type);
// Frees the tree in param
void symtable_dispose(symtable_tree_ptr tree);
// Create new string, copy old string(param) into new and return it
char* my_str_dup(char* key);
// Returns higher int
int max(int a, int b);
// Returns height of node
int height(symtable_node_ptr node);
// Searches for key in tree
symtable_node_ptr symtable_search(symtable_node_ptr tree, char *key);
// Returns balance
int get_balance (symtable_node_ptr node);
// Frees entry struct
void symtable_free_entry(symtable_entry_ptr entry);
// Initializes symtable to null
void symtable_init(symtable_tree_ptr tree);
// Inserts new node into tree
void symtable_insert(symtable_tree_ptr tree, char* key, symtable_entry_type type);

// Help functions used in rebalancing of tree
void simple_right_rot(symtable_tree_ptr root);
void simple_left_rot(symtable_tree_ptr root);
void left_right_rot(symtable_tree_ptr tree);
void right_left_rot(symtable_tree_ptr tree);

// Updates balances of all nodes and returns a bool (true->rebalance, false->all ok)
bool update_balances(symtable_tree_ptr tree);
// Rebalances tree based on balance factor
void rebalance(symtable_tree_ptr root);
// Initializes new entry variable
symtable_entry_ptr symtable_entry_init();

#endif // SYMTABLE_H
