#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_TOKENS 1000

int isKeyword(const char* word) {
    const char* keywords[] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long",
        "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while"
    };

    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; ++i) {
        if (strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}


TokenType getKeywordType(const char* word) {
    if (strcmp(word, "auto") == 0) return TOKEN_AUTO;
    if (strcmp(word, "break") == 0) return TOKEN_BREAK;
    if (strcmp(word, "case") == 0) return TOKEN_CASE;
    if (strcmp(word, "char") == 0) return TOKEN_CHAR;
    if (strcmp(word, "const") == 0) return TOKEN_CONST;
    if (strcmp(word, "continue") == 0) return TOKEN_CONTINUE;
    if (strcmp(word, "default") == 0) return TOKEN_DEFAULT;
    if (strcmp(word, "do") == 0) return TOKEN_DO;
    if (strcmp(word, "double") == 0) return TOKEN_DOUBLE;
    if (strcmp(word, "else") == 0) return TOKEN_ELSE;
    if (strcmp(word, "enum") == 0) return TOKEN_ENUM;
    if (strcmp(word, "extern") == 0) return TOKEN_EXTERN;
    if (strcmp(word, "float") == 0) return TOKEN_FLOAT;
    if (strcmp(word, "for") == 0) return TOKEN_FOR;
    if (strcmp(word, "goto") == 0) return TOKEN_GOTO;
    if (strcmp(word, "if") == 0) return TOKEN_IF;
    if (strcmp(word, "int") == 0) return TOKEN_INT;
    if (strcmp(word, "long") == 0) return TOKEN_LONG;
    if (strcmp(word, "register") == 0) return TOKEN_REGISTER;
    if (strcmp(word, "return") == 0) return TOKEN_RETURN;
    if (strcmp(word, "short") == 0) return TOKEN_SHORT;
    if (strcmp(word, "signed") == 0) return TOKEN_SIGNED;
    if (strcmp(word, "sizeof") == 0) return TOKEN_SIZEOF;
    if (strcmp(word, "static") == 0) return TOKEN_STATIC;
    if (strcmp(word, "struct") == 0) return TOKEN_STRUCT;
    if (strcmp(word, "switch") == 0) return TOKEN_SWITCH;
    if (strcmp(word, "typedef") == 0) return TOKEN_TYPEDEF;
    if (strcmp(word, "union") == 0) return TOKEN_UNION;
    if (strcmp(word, "unsigned") == 0) return TOKEN_UNSIGNED;
    if (strcmp(word, "void") == 0) return TOKEN_VOID;
    if (strcmp(word, "volatile") == 0) return TOKEN_VOLATILE;
    if (strcmp(word, "while") == 0) return TOKEN_WHILE;
    if (strcmp(word, "print") == 0 || strcmp(word, "printf") == 0) return TOKEN_PRINT;
    return TOKEN_IDENTIFIER;
}

Token* tokenize(const char* filename, int* tokenCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    *tokenCount = 0;

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (isspace(ch)) continue;

        // Identifiers / keywords
        if (isalpha(ch)) {
            char buffer[100] = {0};
            int len = 0;
            buffer[len++] = ch;
            while (isalnum(ch = fgetc(file))) {
                buffer[len++] = ch;
            }
            buffer[len] = '\0';
            ungetc(ch, file);

            Token t;
            if (isKeyword(buffer)) {
                t.type = getKeywordType(buffer);
            } else {
                t.type = TOKEN_IDENTIFIER;
            }
            strcpy(t.lexeme, buffer);
            tokens[(*tokenCount)++] = t;
        }

        // Numbers
        else if (isdigit(ch)) {
            char buffer[100] = {0};
            int len = 0;
            buffer[len++] = ch;
            while (isdigit(ch = fgetc(file))) {
                buffer[len++] = ch;
            }
            buffer[len] = '\0';
            ungetc(ch, file);

            Token t = {TOKEN_NUMBER};
            strcpy(t.lexeme, buffer);
            tokens[(*tokenCount)++] = t;
        }

        // String literal
        else if (ch == '"') {
            char buffer[100] = {0};
            int len = 0;
            while ((ch = fgetc(file)) != '"' && ch != EOF) {
                buffer[len++] = ch;
            }
            buffer[len] = '\0';

            Token t = {TOKEN_STRING_LITERAL};
            strcpy(t.lexeme, buffer);
            tokens[(*tokenCount)++] = t;
        }

        // Two-character operators
        else if (ch == '=' || ch == '!' || ch == '<' || ch == '>') {
            char next = fgetc(file);
            if (next == '=') {
                Token t = {TOKEN_OPERATOR};
                sprintf(t.lexeme, "%c=", ch);
                tokens[(*tokenCount)++] = t;
            } else {
                ungetc(next, file);
                Token t = {TOKEN_OPERATOR};
                sprintf(t.lexeme, "%c", ch);
                tokens[(*tokenCount)++] = t;
            }
        }

        // Single-character operators and punctuation
        else {
            Token t;
            switch (ch) {
                case '+': case '-': case '*': case '/': case '%':
                    t.type = TOKEN_OPERATOR;
                    break;
                case '=': t.type = TOKEN_ASSIGN; break;
                case ';': t.type = TOKEN_SEMICOLON; break;
                case ',': t.type = TOKEN_COMMA; break;
                case '{': t.type = TOKEN_OPEN_BRACE; break;
                case '}': t.type = TOKEN_CLOSE_BRACE; break;
                case '(': t.type = TOKEN_ROUND_BRACE_OPEN; break;
                case ')': t.type = TOKEN_ROUND_BRACE_CLOSE; break;
                default:
                    printf("Unknown character: %c\n", ch);
                    continue;
            }
            t.lexeme[0] = ch;
            t.lexeme[1] = '\0';
            tokens[(*tokenCount)++] = t;
        }
    }

    Token eof = {TOKEN_EOF, "EOF"};
    tokens[(*tokenCount)++] = eof;

    fclose(file);
    return tokens;
}
