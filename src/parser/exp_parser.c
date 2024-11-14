/*
file: exp_parser.h
author: Adam Vesely
Implementation of the expression parser.
*/
// TODO ERROR HANDLING AND EXTENSIONS
#include <stdbool.h>

#include "exp_parser.h"
#include "scanner.h"
#include "stack.h"

#define P_TABLE_SIZE 12

// we compare the operator on the stack vs the current operator from the token
ASOC precedence_table[P_TABLE_SIZE][P_TABLE_SIZE] = 
{ //  .? ! */ +- orelse  r and or (  )  i  $
    { L, U, U, U,   U,   U, U, U, L, H, L, H }, // UNREACHABLE ".?"
    { U, L, H, H,   H,   H, H, H, L, H, H, H }, // NEGATE "!"
    { U, L, H, H,   H,   H, H, H, L, H, L, H }, // MUL_DIV "* /"
    { U, L, L, H,   H,   H, H, H, L, H, L, H }, // ADD_SUB "+ -"
    { U, L, L, L,   H,   H, H, H, L, H, L, H }, // ORELSE "orelse"
    { U, L, L, L,   L,   U, H, H, L, H, L, H }, // RELATION "r"
    { U, L, L, L,   L,   L, H, H, L, H, L, H }, // AND "and"
    { U, L, L, L,   L,   L, L, H, L, H, L, H }, // OR "or"
    { L, L, L, L,   L,   L, L, L, L, E, L, U }, // L_PAR "("
    { H, H, H, H,   H,   H, H, H, U, H, U, H }, // R_PAR ")"
    { H, U, H, H,   H,   H, H, H, U, H, U, H }, // LIT_ID "i"
    { L, L, L, L,   L,   L, L, L, L, U, L, U }  // END "$"
};

// Function to parse an expression
ASTNode *parseExpression(Token* token, circ_buff_ptr buff) {

    if (token == NULL) {
        return NULL; // Error: unexpected end of input
    }
    stack_t* operand_stack = stackInit();
    stack_t* operator_stack = stackInit();
    // number of left parenthesis on the stack
    int* paren_depth = malloc(sizeof(int));
    if(paren_depth == NULL) {
        error_exit(99, "ERROR: Unable to allocate memory for paren_depth!\n");
        return NULL;
    }
    *paren_depth = 0;

    while(token != NULL && !expressionEnd(isEnd(token), stackIsEmpty(operator_stack))) {
        ASTNode* node = nodeCreate(convertToASTType(token->type, NO_KW), token);
        if(isOperand(token)) {
            stackPush(operand_stack, (long)node);
        }
        else if(isOperator(token)) {
            if(token->type == T_LPAREN) {
                (*paren_depth)++;
            }
            if(token->type == T_RPAREN) {
                if(*paren_depth != 0) {
                    reduceParen(operand_stack, operator_stack);
                    (*paren_depth)--;
                }
                else {
                    break;
                }
                continue;
            }
            if(stackIsEmpty(operator_stack)) {
                stackPush(operator_stack, (long)node);
                continue;
            }
            ASTNode* top = (ASTNode*)stackGetTop(operator_stack);
            switch(precedence_table[getIndex(top)][getIndex(node)]) {
                case L:
                    stackPush(operator_stack, (long)node);
                    break;
                case H:
                case E:
                    reduce(operand_stack, operator_stack);
                    stackPush(operator_stack, (long)node);
                    break;
                case U:
                default:
                    return NULL;
            }
        }
        else {
            return NULL;
        }
        token = get_token(buff);
    }

    ASTNode* root = reduceAll(operand_stack, operator_stack);
    stackClear(operand_stack);
    stackClear(operator_stack);
    free(paren_depth);

    return root;
}

void reduce(stack_t* operand_stack, stack_t* operator_stack) {
    ASTNode* root = (ASTNode*)stackGetTop(operator_stack);
    stackPop(operator_stack);

    ASTNode* child = (ASTNode*)stackGetTop(operand_stack);
    stackPop(operator_stack);
    insertRight(root, child);

    if(root->type != BANG) {
        child = (ASTNode*)stackGetTop(operand_stack);
        stackPop(operator_stack);
        insertLeft(root, child);
    }

    stackPush(operand_stack, (long)root);

}

void reduceParen(stack_t* operand_stack, stack_t* operator_stack) {
    ASTNode* curr = (ASTNode*)stackGetTop(operator_stack);
    while(curr->type != LPAREN && !stackIsEmpty(operator_stack)) {
        reduce(operand_stack, operator_stack);
        curr = (ASTNode*)stackGetTop(operator_stack);
    }
    stackPop(operator_stack);
}

ASTNode* reduceAll(stack_t* operand_stack, stack_t* operator_stack) {
    // while (), a = ();
    if(stackIsEmpty(operator_stack) && stackIsEmpty(operand_stack)) {
        return NULL;
    }
    while(!stackIsEmpty(operator_stack)) {
        reduce(operand_stack, operator_stack);
    }
    ASTNode* root = (ASTNode*)stackGetTop(operand_stack);
    stackPop(operand_stack);
    return root;
}

PREC_TABLE_INDEX getIndex(ASTNode* node) {
    switch (node->type) {
        case BANG:
            return NEGATE;
        case MUL:
        case DIV:
            return MUL_DIV;
        case PLUS:
        case MINUS:
            return ADD_SUB;
        case EQ:
        case NEQ:
        case LESS:
        case LEQ:
        case MORE:
        case MEQ:
            return RELATION;
        case LPAREN:
            return L_PAR;
        case RPAREN:
            return R_PAR;
        case ID:
        case TYPE_INT:
        case TYPE_F64:
        case TYPE_STR:
            return LIT_ID;
    }   
}

bool isOperator(Token* token) {
    switch(token->type) {
        case T_EQ:
        case T_BANG:
        case T_NEQ:
        case T_LESS:
        case T_LEQ:
        case T_MORE:
        case T_MEQ:
        case T_PLUS:
        case T_MINUS:
        case T_MUL:
        case T_DIV:
            return true;
        default:
            return false;
    }
}

bool isOperand(Token* token) {
    switch(token->type) {
        case T_ID:
        case T_INT:
        case T_FLOAT:
        case T_STR:
            return true;
        default:
            return false;
    }
}

bool isEnd(Token* token) {
    switch(token->type) {
        case T_RPAREN:
        case T_SEMICOL:
        case T_COMMA:
            return true;
        default:
            return false;
    }
}

bool expressionEnd(bool end, bool operator_stack_isEmpty) {
    return end && operator_stack_isEmpty;
}
