#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_TOKENS 1000

int isKeyword(const char* word) {
    return strcmp(word, "int") == 0 ||
           strcmp(word, "for") == 0 ||
           strcmp(word, "while") == 0 ||
           strcmp(word, "if") == 0 ||
           strcmp(word, "else") == 0 ||
           strcmp(word, "return") == 0 ||
           strcmp(word, "print") == 0 ||
           strcmp(word, "printf") == 0;
}

TokenType getKeywordType(const char* word) {
    if (strcmp(word, "int") == 0) return TOKEN_INT;
    if (strcmp(word, "for") == 0) return TOKEN_FOR;
    if (strcmp(word, "while") == 0) return TOKEN_WHILE;
    if (strcmp(word, "if") == 0) return TOKEN_IF;
    if (strcmp(word, "else") == 0) return TOKEN_ELSE;
    if (strcmp(word, "return") == 0) return TOKEN_RETURN;
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
