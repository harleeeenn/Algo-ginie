#include <stdio.h>
#include <string.h>
#include "parser.h"

void parse(Token* tokens) {
    int i = 0;
    while (tokens[i].type != TOKEN_EOF) {
        if (tokens[i].type == TOKEN_INT) {
            i++;  // Move to identifier
            if (tokens[i].type == TOKEN_IDENTIFIER) {
                char varName[100];
                strcpy(varName, tokens[i].lexeme);
                i++;
                if (tokens[i].type == TOKEN_ASSIGN) {
                    i++;
                    if (tokens[i].type == TOKEN_NUMBER || tokens[i].type == TOKEN_IDENTIFIER) {
                        printf("Declare int %s and assign %s\n", varName, tokens[i].lexeme);
                        i++;
                        if (tokens[i].type == TOKEN_SEMICOLON) {
                            i++;
                        }
                    }
                } else if (tokens[i].type == TOKEN_SEMICOLON) {
                    printf("Declare int %s\n", varName);
                    i++;
                }
            }
        } else if (tokens[i].type == TOKEN_IDENTIFIER) {
            char varName[100];
            strcpy(varName, tokens[i].lexeme);
            i++;
            if (tokens[i].type == TOKEN_ASSIGN) {
                i++;
                if (tokens[i].type == TOKEN_NUMBER || tokens[i].type == TOKEN_IDENTIFIER) {
                    char exprLeft[100];
                    strcpy(exprLeft, tokens[i].lexeme);
                    i++;
                    if (tokens[i].type == TOKEN_OPERATOR) {
                        char op = tokens[i].lexeme[0];
                        i++;
                        if (tokens[i].type == TOKEN_NUMBER || tokens[i].type == TOKEN_IDENTIFIER) {
                            printf("Set %s = %s %c %s\n", varName, exprLeft, op, tokens[i].lexeme);
                            i++;
                        }
                    } else {
                        printf("Set %s = %s\n", varName, exprLeft);
                    }
                    if (tokens[i].type == TOKEN_SEMICOLON) {
                        i++;
                    }
                }
            }
        } else if (tokens[i].type == TOKEN_RETURN) {
            i++;
            if (tokens[i].type == TOKEN_NUMBER || tokens[i].type == TOKEN_IDENTIFIER) {
                printf("Return %s\n", tokens[i].lexeme);
                i++;
                if (tokens[i].type == TOKEN_SEMICOLON) {
                    i++;
                }
            }
        } else if (tokens[i].type == TOKEN_OPEN_BRACE) {
            printf("Begin Block\n");
            i++;
        } else if (tokens[i].type == TOKEN_CLOSE_BRACE) {
            printf("End Block\n");
            i++;
        } else {
            i++;
        }
    }
}
