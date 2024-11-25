#include "sem_anal.h"
#include "ast.h"
#include "error.h"

// TODO: implicit conversion and IDIV/DIV
void analyse (ASTNode* root) {
    checkForMain();
    root = root->right->left; // skip prolog
    symFuncDef(root);
}

void symFuncDef(ASTNode* node){
    if (node == NULL) return;
    // function Sym node 
    node = node->right; // ID
    symtable_node_ptr tree = symtable_search(SymFunctionTree, node->token->value.string_value);
    tree = *tree->entry->local_symtable;

    node = node->left->left; // params or RPAREN
    if (node->type == P_PARAM_LIST){

        symParamList(node, &tree);
        
        node = node->left; // RPAREN
    }

    node = node->left->left; // block

    symStatement(node->right, &tree); 

    symFuncDef(node->left);
}
void symStatement(ASTNode* node, symtable_tree_ptr tree) {
    if (node == NULL)
        return;
    
    if (node->type == ID) {
        symIdStatement(node, tree);
    }
    
    node = node->right;

    switch(node->type){

        case P_VAR_DECLARATION:
            symVarDec(node, tree);
            break;

        case ID:
            symIdStatement(node, tree);
            break;

        case P_IF_STATEMENT:
            symIfStatement(node, tree);
            break;

        case P_WHILE_LOOP:
            symWhileLoop(node, tree);
            break;
        
        case P_RETURN_STATEMENT:
            symReturnStatement(node, tree);
            break;

        case P_BREAK:
            symBreakStatement(node, tree);
            break;

        case P_CONTINUE:
            symContinueStatement(node, tree);
            break;

        case P_FOR_LOOP:
            symForLoop(node, tree);
            break;

        default:
            printf("unknown\n");
            break;
    }
}

void symParamList(ASTNode* node, symtable_tree_ptr tree) {
    if (node == NULL) return;

    ASTNode *id = node->right; // save ID
    symtable_insert(tree, id->token->value.string_value, T_VAR_SYM);
    node = id->left; // move to P_TYPE_COMPLETE

    symtable_node_ptr key = symtable_search(*tree, id->token->value.string_value);
    if (node->right->type == P_QUESTION_MARK) {
        key->entry->isChanged = false;
        key->entry->hasExplicitType = true;
        key->entry->isUsed = false;
        key->entry->isConst = true;
        key->entry->isNullable = true;
        key->entry->type = getRetType(convertToASTType(T_KW, node->right->left->right->token->value.keyword));
    }
    else if (node->right->type == P_TYPE) {
        key->entry->isChanged = false;
        key->entry->isUsed = false;
        key->entry->hasExplicitType = true;
        key->entry->isConst = true;
        key->entry->isNullable = false;
        key->entry->type = getRetType(convertToASTType(T_KW, node->right->right->token->value.keyword));
    }
    
    node = node->left; // comma or null
    if (node != NULL) {
        symParamList(node->right, tree);
    }
}

void symVarDec(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;
    
    node = node->right; // const or var
    bool isconst = node->token->value.keyword == KW_CONST ? true : false;

    symStatement(nextStatement, tree);
}

void symIdStatement(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;

    symStatement(nextStatement, tree);
}

void symIfStatement(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;

    symStatement(nextStatement, tree);
}

void symWhileLoop(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;

    symStatement(nextStatement, tree);
}

void symReturnStatement(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;

    symStatement(nextStatement, tree);
}

void symBreakStatement(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;

    symStatement(nextStatement, tree);
}

void symContinueStatement(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;

    symStatement(nextStatement, tree);
}

void symForLoop(ASTNode* node, symtable_tree_ptr tree){
    ASTNode* nextStatement = node->left;

    symStatement(nextStatement, tree);
}

void checkExpr(ASTNode* node) {
    if(node == NULL) {
        return;
    }
    switch(node->type) {
        case DIV:
            checkDiv(node);
            checkBinTypes(node);
            break;
        case EQ:
        case NEQ:
        case LESS:
        case LEQ:
        case MORE:
        case MEQ:
        case PLUS:
        case MINUS:
        case MUL:
            checkBinTypes(node);
            break;
        case BANG:
            checkUnTypes(node);
            break;
        default:
            break;
    }
    checkExpr(node->left);
    checkExpr(node->right);
}

void checkForMain() {
    symtable_node_ptr node = symtable_search(SymFunctionTree, "main");
    if (node == NULL) {
        symtable_dispose(&SymFunctionTree);
        freeAST(ASTRoot);
        error_exit(3, "ERROR: Missing main!\n");
    }
    if (node->entry->param_count != 0 || node->entry->returnsValue != false) {
        symtable_dispose(&SymFunctionTree);
        freeAST(ASTRoot);
        error_exit(4, "ERROR: Wrong main params or type!\n");
    }
}

void checkIfIdExits(symtable_node_ptr tree, char *key) {
    symtable_node_ptr node = symtable_search(tree, key);

    if (node == NULL) {
        symtable_dispose(&SymFunctionTree);
        freeAST(ASTRoot);
        error_exit(3, "ERROR: Missing definition!\n");
    }
}


void getFunctionParamInfo(symtable_node_ptr tree, char *key, ASTNode *ParamList, int i, int capacity) {
    symtable_node_ptr node = symtable_search(tree, key);

    if (node->entry->param_nullable == NULL && node->entry->param_types == NULL) {
        node->entry->param_nullable = malloc(capacity*sizeof(bool));
        node->entry->param_types = malloc(capacity*sizeof(ret_type));
        if (node->entry->param_nullable == NULL || node->entry->param_types == NULL) {
            symtable_dispose(&SymFunctionTree);
            freeAST(ASTRoot);
            error_exit(99, "ERROR: Unable to allocate memory\n");
        }
    }

    if (i == capacity - 1) {
        capacity = 2 * capacity;
        node->entry->param_nullable = realloc(node->entry->param_nullable, capacity * sizeof(bool));
        node->entry->param_types = realloc(node->entry->param_types, capacity * sizeof(ret_type));
        if (node->entry->param_nullable == NULL || node->entry->param_types == NULL) {
            symtable_dispose(&SymFunctionTree);
            freeAST(ASTRoot);
            error_exit(99, "ERROR: Unable to allocate memory\n");
        }
    }
    // move to ID
    ParamList = ParamList->right;
    if (ParamList != NULL) {
        (node->entry->param_count)++;
    }
    // move to P_TYPE_COMPLETE
    ParamList = ParamList->left;

    if (ParamList->right->type == P_QUESTION_MARK) {
        node->entry->param_nullable[i] = true;
        node->entry->param_types[i] = getRetType(convertToASTType(T_KW, ParamList->right->left->right->token->value.keyword));
        i++;
    } else if (ParamList->right->type == P_TYPE) {
        node->entry->param_nullable[i] = false;
        node->entry->param_types[i] = getRetType(convertToASTType(T_KW, ParamList->right->right->token->value.keyword));
        i++;
    }
    if (ParamList->left != NULL) {
        if (ParamList->left->right != NULL) {
            getFunctionParamInfo(tree, key, ParamList->left->right, i, capacity);
        }
    }
}

void getFunctionType(symtable_node_ptr tree, char *key, ASTNode *FunctionType) {
    symtable_node_ptr node = symtable_search(tree, key);
    node->entry->isUsed = false;

    if (FunctionType->right->type == T_VOID) {
        node->entry->type = getRetType(T_VOID);
        node->entry->returnsValue = false;

    } else {
        // Move to P_TYPE_COMPLETE
        FunctionType = FunctionType->right;

        if (FunctionType->right->type == P_QUESTION_MARK) {
            node->entry->isNullable = true;
            node->entry->returnsValue = true;
            node->entry->type = getRetType(convertToASTType(T_KW, FunctionType->right->left->right->token->value.keyword));

        } else if (FunctionType->right->type == P_TYPE) {
            node->entry->isNullable = false;
            node->entry->returnsValue = true;
            node->entry->type = getRetType(convertToASTType(T_KW, FunctionType->right->right->token->value.keyword));
        }
    }
}

ret_type getRetType(ASTNodeType type) {
    switch(type) {
        case T__KW_I32: return T_INT_RET;
        case T_KW_F64: return T_FLOAT_RET;
        case T_KW_BOOL: return T_BOOL_RET;
        case T_U8: return T_STR_RET;
        case T_VOID: return T_NULL_RET;
        default: return 99;
    }
}

void checkDiv(ASTNode* node) {    
    if(node->left->type == TYPE_INT && node->right->type == TYPE_INT) {
        node->token->type = T_IDIV;
        node->type = IDIV;
    }
}

void checkBinTypes(ASTNode* node) {
    if(node->left == NULL || node->right == NULL) {
        symtable_dispose(&SymFunctionTree);
        freeAST(ASTRoot);
        error_exit(7, "ERROR: Missing operand in binary operation!\n");
        return;
    }
    if(node->left->type == TYPE_INT && node->right->type == TYPE_F64) {
        ASTNode* temp = node->left;
        Token* new_token = (Token*)malloc(sizeof(Token));
        if (new_token == NULL){ 
            symtable_dispose(&SymFunctionTree);
            freeAST(ASTRoot);
            error_exit(99, "Memory allocation failed"); 
        }
        new_token->type = T_I2F;
        // create node for orelse
        ASTNode* new_node = nodeCreate(I2F, new_token);
        new_node->right = temp;
        node->left = new_node;
    }
    if(node->right->type == TYPE_INT && node->left->type == TYPE_F64) {
        ASTNode* temp = node->right;
        Token* new_token = (Token*)malloc(sizeof(Token));
        if (new_token == NULL){ 
            symtable_dispose(&SymFunctionTree);
            freeAST(ASTRoot);
            error_exit(99, "Memory allocation failed"); 
        }
        new_token->type = T_I2F;
        // create node for orelse
        ASTNode* new_node = nodeCreate(I2F, new_token);
        new_node->right = temp;
        node->right = new_node;
    }
    if(node->left->type != node->right->type) {
        symtable_dispose(&SymFunctionTree);
        freeAST(ASTRoot);
        error_exit(7, "ERROR: Wrong types in binary operation!\n");
    }
}

void checkUnTypes(ASTNode* node) {
    switch(node->right->type) {
        case TYPE_INT:
        case TYPE_F64:
        case ID:
        case BANG:
            return;
        default:
            symtable_dispose(&SymFunctionTree);
            freeAST(ASTRoot);
            error_exit(7, "ERROR: Wrong type in unary operation!\n");
    }
}