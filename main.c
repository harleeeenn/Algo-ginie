#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main() {
    int tokenCount = 0;
    Token* tokens = tokenize("input.c", &tokenCount);  

    for (int i = 0; i < tokenCount; i++) {
    printf("Token %d: Type = %d, Lexeme = %s\n", i, tokens[i].type, tokens[i].lexeme);
}

    printf("Pseudocode:\n");
    parse(tokens);  
    free(tokens); 
    return 0;
}
