#ifndef STACK_U
#define STACK_U

#include "symtable.h"
#include "stack.h"

symtable_node_ptr stackUtilCopy(symtable_node_ptr tree);

symtable_node_ptr stackUtilPop(stack_t* stack);

void updateTableBySameKey(symtable_node_ptr oldTable, symtable_tree_ptr localTable);

#endif