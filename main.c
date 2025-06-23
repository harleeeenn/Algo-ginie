#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

extern char parseTreeOutput[10000];  

int main() {
    int tokenCount = 0;
    Token* tokens = tokenize("input.c", &tokenCount);

    
    printf("Pseudocode:\n");
    parse(tokens); 
    printf("===END===\n");
    
    printf("Tokens:\n");
    for (int i = 0; i < tokenCount; i++) {
        printf("Token %d: Type = %d, Lexeme = %s\n", i, tokens[i].type, tokens[i].lexeme);
    }
    printf("===END===\n");

    free(tokens);
    return 0;
}
