#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

Token* tokenize(const char* filename, int* tokenCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(1);
    }

    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    *tokenCount = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (isspace(ch)) {
            continue;
        } else if (ch == '#') {
            while ((ch = fgetc(file)) != '\n' && ch != EOF);
            continue;
        } else if (isalpha(ch) || ch == '_') {
            char buffer[100] = {0};
            int i = 0;
            buffer[i++] = ch;
            while (isalnum(ch = fgetc(file)) || ch == '_') {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, file);

            if (strcmp(buffer, "int") == 0) tokens[*tokenCount].type = TOKEN_INT;
            else if (strcmp(buffer, "if") == 0) tokens[*tokenCount].type = TOKEN_IF;
            else if (strcmp(buffer, "else") == 0) tokens[*tokenCount].type = TOKEN_ELSE;
            else if (strcmp(buffer, "for") == 0) tokens[*tokenCount].type = TOKEN_FOR;
            else if (strcmp(buffer, "while") == 0) tokens[*tokenCount].type = TOKEN_WHILE;
            else if (strcmp(buffer, "return") == 0) tokens[*tokenCount].type = TOKEN_RETURN;
            else tokens[*tokenCount].type = TOKEN_IDENTIFIER;

            strcpy(tokens[*tokenCount].lexeme, buffer);
            (*tokenCount)++;
        } else if (isdigit(ch)) {
            char buffer[100] = {0};
            int i = 0;
            buffer[i++] = ch;
            while (isdigit(ch = fgetc(file))) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, file);

            tokens[*tokenCount].type = TOKEN_NUMBER;
            strcpy(tokens[*tokenCount].lexeme, buffer);
            (*tokenCount)++;
        } else {
            switch (ch) {
                case '{':
                    tokens[*tokenCount].type = TOKEN_OPEN_BRACE;
                    tokens[*tokenCount].lexeme[0] = ch;
                    tokens[*tokenCount].lexeme[1] = '\0';
                    (*tokenCount)++;
                    break;
                case '}':
                    tokens[*tokenCount].type = TOKEN_CLOSE_BRACE;
                    tokens[*tokenCount].lexeme[0] = ch;
                    tokens[*tokenCount].lexeme[1] = '\0';
                    (*tokenCount)++;
                    break;
                case ';':
                    tokens[*tokenCount].type = TOKEN_SEMICOLON;
                    tokens[*tokenCount].lexeme[0] = ch;
                    tokens[*tokenCount].lexeme[1] = '\0';
                    (*tokenCount)++;
                    break;
                case '=':
                    tokens[*tokenCount].type = TOKEN_ASSIGN;
                    tokens[*tokenCount].lexeme[0] = ch;
                    tokens[*tokenCount].lexeme[1] = '\0';
                    (*tokenCount)++;
                    break;
                case '+': case '-': case '*': case '/':
                    tokens[*tokenCount].type = TOKEN_OPERATOR;
                    tokens[*tokenCount].lexeme[0] = ch;
                    tokens[*tokenCount].lexeme[1] = '\0';
                    (*tokenCount)++;
                    break;
                default:
                    tokens[*tokenCount].type = TOKEN_UNKNOWN;
                    tokens[*tokenCount].lexeme[0] = ch;
                    tokens[*tokenCount].lexeme[1] = '\0';
                    (*tokenCount)++;
                    break;
            }
        }
    }

    tokens[*tokenCount].type = TOKEN_EOF;
    strcpy(tokens[*tokenCount].lexeme, "EOF");
    (*tokenCount)++;

    fclose(file);
    return tokens;
}
