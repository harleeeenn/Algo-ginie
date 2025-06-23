#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_ASSIGN, TOKEN_NUMBER, TOKEN_SEMICOLON,
    TOKEN_COMMA, TOKEN_OPERATOR, TOKEN_OPEN_BRACE, TOKEN_CLOSE_BRACE,
    TOKEN_ROUND_BRACE_OPEN, TOKEN_ROUND_BRACE_CLOSE,
    TOKEN_IF, TOKEN_ELSE, TOKEN_FOR, TOKEN_WHILE, TOKEN_RETURN, TOKEN_PRINT,
    TOKEN_STRING_LITERAL,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
} Token;

Token* tokenize(const char* filename, int* tokenCount);

#endif  // LEXER_H
