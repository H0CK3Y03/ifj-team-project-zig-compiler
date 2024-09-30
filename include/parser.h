/**
 * @file parser.h
 * 
 * @author Adam Veselý
 * 
 * @brief Header file for the parser.
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdbool.h>

#include "scanner.h"
#include "token.h"

enum return_values {
    SUCCESS,
    LEXICAL_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR_DEFINITION,
    SEMANTIC_ERROR_PARAMETER,
    SEMANTIC_ERROR_ASSIGNMENT,
    SEMANTIC_ERROR_EXPRESSION,
    SEMANTIC_ERROR_COMPATIBILITY,
    SEMANTIC_ERROR_TYPE,
    SEMANTIC_ERROR_VARIABLE,
    SEMANTIC_ERROR_OTHER,
    INTERNAL_ERROR = 99
};


#endif

/*** End of file parser.h ***/