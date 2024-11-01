#ifndef PARSER__H_
#define PARSER__H_

#include "ast.h"



void Parse();

void Prog(Token *token, circ_buff_ptr buffer);

void Prolog(Token *token, ASTNode *ptr, circ_buff_ptr buffer);

void FunctionDef(Token *token, ASTNode *ptr, circ_buff_ptr buffer);

void End(Token *token, ASTNode *ptr, circ_buff_ptr buffer); 

void Expression(Token *token, ASTNode *ptr, circ_buff_ptr buffer);


#endif // PARSER__H_