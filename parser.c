#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.h"
#include "tree.h"

int nodeCount = 0;

void printPseudo(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void parse(Token* tokens) {
    int i = 0;
    startHTMLTree();

    openNode("Program");
    printPseudo("Pseudocode:\n");

    while (tokens[i].type != TOKEN_EOF) {
        // Variable declaration
   if (tokens[i].type == TOKEN_INT) {
    i++;  // skip 'int'

    while (1) {
        if (tokens[i].type == TOKEN_IDENTIFIER) {
            openNode("Declaration (int)");
            char varName[100];
            strcpy(varName, tokens[i].lexeme);
            leafNode(varName);
            printPseudo("Declare int %s\n", varName);
            i++;

            if (tokens[i].type == TOKEN_ASSIGN) {
                i++;
                if (tokens[i].type == TOKEN_NUMBER || tokens[i].type == TOKEN_IDENTIFIER) {
                    openNode("Assignment");
                    leafNode(varName);
                    leafNode(tokens[i].lexeme);
                    printPseudo("Assign %s = %s\n", varName, tokens[i].lexeme);
                    i++;
                    closeNode(); // Assignment
                }
            }

            closeNode(); // Declaration
        } else {
            break;
        }

        if (tokens[i].type == TOKEN_COMMA) {
            i++;
        } else if (tokens[i].type == TOKEN_SEMICOLON) {
            i++;
            break;
        } else {
            break;  // to be safe
        }
    }
}



        // Assignment
        else if (tokens[i].type == TOKEN_IDENTIFIER) {
            char var[100];
            strcpy(var, tokens[i].lexeme);
            i++;

            if (tokens[i].type == TOKEN_ASSIGN) {
                i++;
                char left[100];
                strcpy(left, tokens[i].lexeme);
                i++;

                if (tokens[i].type == TOKEN_OPERATOR) {
                    char op = tokens[i].lexeme[0];
                    i++;
                    char right[100];
                    strcpy(right, tokens[i].lexeme);
                    i++;

                    openNode("Assignment");
                    leafNode(var);
                    char expr[100];
                    sprintf(expr, "%s %c %s", left, op, right);
                    leafNode(expr);
                    closeNode();

                    printPseudo("Set %s = %s\n", var, expr);
                } else {
                    openNode("Assignment");
                    leafNode(var);
                    leafNode(left);
                    closeNode();

                    printPseudo("Set %s = %s\n", var, left);
                }
                if (tokens[i].type == TOKEN_SEMICOLON) i++;
            }
        }

        // If Statement
        else if (tokens[i].type == TOKEN_IF) {
            openNode("If Statement");
            i++; // skip 'if'

            if (tokens[i].type == TOKEN_ROUND_BRACE_OPEN) i++;

            char left[50], op[5], right[50];
            strcpy(left, tokens[i++].lexeme);
            strcpy(op, tokens[i++].lexeme);
            strcpy(right, tokens[i++].lexeme);

            leafNode("Condition:");
            leafNode(left);
            leafNode(op);
            leafNode(right);
            printPseudo("If (%s %s %s)\n", left, op, right);

            if (tokens[i].type == TOKEN_ROUND_BRACE_CLOSE) i++;

            if (tokens[i].type == TOKEN_OPEN_BRACE) {
                i++; openNode("Body"); printPseudo("Begin Block\n");

                while (tokens[i].type != TOKEN_CLOSE_BRACE && tokens[i].type != TOKEN_EOF) {
                    if (tokens[i].type == TOKEN_IDENTIFIER) {
                        char varName[50]; strcpy(varName, tokens[i++].lexeme);

                        if (tokens[i].type == TOKEN_ASSIGN) {
                            i++;
                            char left[50]; strcpy(left, tokens[i++].lexeme);

                            if (tokens[i].type == TOKEN_OPERATOR) {
                                char op = tokens[i++].lexeme[0];
                                char right[50]; strcpy(right, tokens[i++].lexeme);

                                openNode("Assignment");
                                leafNode(varName);
                                char expr[100]; sprintf(expr, "%s %c %s", left, op, right);
                                leafNode(expr);
                                closeNode();
                                printPseudo("Set %s = %s\n", varName, expr);
                            }
                        }
                        if (tokens[i].type == TOKEN_SEMICOLON) i++;
                    }
                    else if (tokens[i].type == TOKEN_PRINT) {
                        i++;
                        while (tokens[i].type != TOKEN_STRING_LITERAL && tokens[i].type != TOKEN_EOF) i++;
                        if (tokens[i].type == TOKEN_STRING_LITERAL) i++;

                        while (tokens[i].type == TOKEN_COMMA) {
                            i++;
                            if (tokens[i].type == TOKEN_IDENTIFIER) {
                                leafNode(tokens[i].lexeme);
                                printPseudo("Print %s", tokens[i].lexeme);
                                i++;
                            }
                        }
                        if (tokens[i].type == TOKEN_SEMICOLON) i++;
                    }
                    else i++;
                }

                closeNode(); // Body
                printPseudo("End Block\n");
                i++;
            }
            closeNode(); // If
        }

        // Print Statement
        else if (tokens[i].type == TOKEN_PRINT) {
            i++; openNode("Print Statement");

            if (tokens[i].type == TOKEN_STRING_LITERAL) {
                i++;
                while (tokens[i].type != TOKEN_STRING_LITERAL && tokens[i].type != TOKEN_EOF) i++;
                if (tokens[i].type == TOKEN_STRING_LITERAL) i++;
            }

            while (tokens[i].type == TOKEN_COMMA) {
                i++;
                if (tokens[i].type == TOKEN_IDENTIFIER) {
                    leafNode(tokens[i].lexeme);
                    printPseudo("Print %s\n", tokens[i].lexeme);
                    i++;
                }
            }

            if (tokens[i].type == TOKEN_SEMICOLON) i++;
            closeNode();
        }

        // For Loop
        else if (tokens[i].type == TOKEN_FOR) {
            openNode("For Loop");
            printPseudo("For Loop Detected:\n");
            i++;

            if (tokens[i].type == TOKEN_ROUND_BRACE_OPEN) i++;

            openNode("Init");
            printPseudo("  Init: ");
            while (tokens[i].type != TOKEN_SEMICOLON) {
                printPseudo("%s ", tokens[i].lexeme);
                leafNode(tokens[i].lexeme);
                i++;
            }
            i++; closeNode();

            openNode("Condition");
            printPseudo("  Condition: ");
            while (tokens[i].type != TOKEN_SEMICOLON) {
                printPseudo("%s ", tokens[i].lexeme);
                leafNode(tokens[i].lexeme);
                i++;
            }
            i++; closeNode();

            openNode("Increment");
            printPseudo("  Increment: ");
            while (tokens[i].type != TOKEN_ROUND_BRACE_CLOSE) {
                printPseudo("%s ", tokens[i].lexeme);
                leafNode(tokens[i].lexeme);
                i++;
            }
            i++; closeNode();

            if (tokens[i].type == TOKEN_OPEN_BRACE) {
                i++; openNode("Body"); printPseudo("  Body:\n");
                while (tokens[i].type != TOKEN_CLOSE_BRACE && tokens[i].type != TOKEN_EOF) {
                    printPseudo("  %s", tokens[i].lexeme);
                    leafNode(tokens[i].lexeme);
                    i++;
                }
                i++; closeNode();
            }
            closeNode();
        }

        else {
            i++;
        }
    }

    closeNode(); // Program
    endHTMLTree();
}
