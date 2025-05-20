#ifndef LEXER_H
#define LEXER_H
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_INT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGN,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN,
    TOKEN_EOF,
    TOKEN_PRINT,
    TOKEN_STRING,
    TOKEN_COMMA,
    TOKEN_SLIT,
    TOKEN_ROUND_BRACE_OPEN,
    TOKEN_ROUND_BRACE_CLOSE,
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
} Token;

Token* tokenize(const char* filename, int* tokenCount);
#endif
