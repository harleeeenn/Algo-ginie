#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>  

#include "parser.h"

void printTreeNode(const char* label, const char* id);
void closeTreeNode();


FILE* htmlTreeFile;
int nodeCount = 0;

void startHTMLTree() {
    htmlTreeFile = fopen("tree.html", "w");
    if (!htmlTreeFile) return;

    fprintf(htmlTreeFile,
        "<!DOCTYPE html><html><head><meta charset='UTF-8'><style>"
        ".tree ul { padding-top: 20px; position: relative; }"
        ".tree li { float: left; text-align: center; list-style-type: none; position: relative; padding: 20px 5px 0 5px; }"
        ".tree li::before, .tree li::after { content: ''; position: absolute; top: 0; right: 50%%; border-top: 2px solid #ccc; width: 50%%; height: 20px; }"
        ".tree li::after { right: auto; left: 50%%; border-left: 2px solid #ccc; }"
        ".tree li:only-child::before, .tree li:only-child::after { display: none; }"
        ".tree li:only-child { padding-top: 0; }"
        ".tree li:first-child::before, .tree li:last-child::after { border: 0 none; }"
        ".tree li:last-child::before { border-right: 2px solid #ccc; border-radius: 0 5px 0 0; }"
        ".tree li:first-child::after { border-radius: 5px 0 0 0; }"
        ".tree .node { display: inline-block; border: 2px solid #4CAF50; padding: 10px 20px; border-radius: 20px; background: #eaffea; font-weight: bold; }"
        "</style></head><body><div class='tree'><ul>\n"
    );
}

void endHTMLTree() {
    fprintf(htmlTreeFile, "</ul></div></body></html>");
    fclose(htmlTreeFile);
}

void openNode(const char* label) {
    fprintf(htmlTreeFile, "<li><div class='node'>%s</div><ul>\n", label);
}

void closeNode() {
    fprintf(htmlTreeFile, "</ul></li>\n");
}

void leafNode(const char* label) {
    fprintf(htmlTreeFile, "<li><div class='node'>%s</div></li>\n", label);
}

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
            i++;
            while (tokens[i].type == TOKEN_IDENTIFIER) {
                openNode("Declaration (int)");
                leafNode(tokens[i].lexeme);
                printPseudo("Declare int %s\n", tokens[i].lexeme);
                i++;

                if (tokens[i].type == TOKEN_ASSIGN) {
                    i++;
                    if (tokens[i].type == TOKEN_NUMBER || tokens[i].type == TOKEN_IDENTIFIER) {
                        openNode("Assignment");
                        leafNode(tokens[i - 2].lexeme); // variable name
                        leafNode(tokens[i].lexeme);    // value
                        printPseudo("Assign %s = %s\n", tokens[i - 2].lexeme, tokens[i].lexeme);
                        closeNode(); // Assignment
                        i++;
                    }
                }
                if (tokens[i].type == TOKEN_COMMA || tokens[i].type == TOKEN_SEMICOLON) i++;
                closeNode(); // Declaration
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

       


        // For loop
        else if (tokens[i].type == TOKEN_FOR) {
            openNode("For Loop");
            printPseudo("For Loop Detected:\n");
            i++; // for

            if (tokens[i].type == TOKEN_ROUND_BRACE_OPEN) i++;

            // Init
            openNode("Init");
            printPseudo("  Init: ");
            while (tokens[i].type != TOKEN_SEMICOLON) {
                printPseudo("%s ", tokens[i].lexeme);
                leafNode(tokens[i].lexeme);
                i++;
            }
            printPseudo("\n");
            i++; closeNode();

            // Condition
            openNode("Condition");
            printPseudo("  Condition: ");
            while (tokens[i].type != TOKEN_SEMICOLON) {
                printPseudo("%s ", tokens[i].lexeme);
                leafNode(tokens[i].lexeme);
                i++;
            }
            printPseudo("\n");
            i++; closeNode();

            // Increment
            openNode("Increment");
            printPseudo("  Increment: ");
            while (tokens[i].type != TOKEN_ROUND_BRACE_CLOSE) {
                printPseudo("%s ", tokens[i].lexeme);
                leafNode(tokens[i].lexeme);
                i++;
            }
            printPseudo("\n");
            i++; closeNode();

            // Body
            if (tokens[i].type == TOKEN_OPEN_BRACE) {
                i++; openNode("Body");
                printPseudo("  Body:\n");
                while (tokens[i].type != TOKEN_CLOSE_BRACE && tokens[i].type != TOKEN_EOF) {
                    printPseudo("  %s", tokens[i].lexeme);
                    leafNode(tokens[i].lexeme);
                    i++;
                }
                i++; closeNode(); // Body
            }

            closeNode(); // For Loop
        }

        else {
            i++;
        }
    }

    closeNode(); // Program
    endHTMLTree();
}
