/**
 * @file scanner.h
 * 
 * @author Marek Hric
 * 
 * @brief Header file for the scanner.
 */

enum State {
    S_START,
    S_END,
    S_ERROR,
    
    S_ID,
    S_KW,

    //OPERATORS
    S_ASGN, S_EQ, 
    S_BANG, S_NEQ,
    S_LESS, S_LEQ,
    S_MORE, S_MEQ,
    
    S_PLUS, S_PASGN,
    S_MINUS, S_MASGN,
    S_MUL, S_MULASGN,
    S_SLASH, S_DASGN,

    //NUMBERS
    S_INT,
    S_F64, S_F64_DOT, S_F64_E, S_F64_E1, S_F64_E2,

    //DELIMITERS
    S_DOT,
    S_COMMA,
    S_SEMICOL,
    S_LPAREN, S_RPAREN,
    S_LBRACE, S_RBRACE,
    S_LBRACKET, S_RBRACKET,

    //LITERALS
    S_STR, S_STR_ESC,

    //COMMENTS
    S_COMM   
};

typedef struct Token;

// Function prototypes
Token* get_token();
void free_token(Token* token);

/*** End of file scanner.h ***/