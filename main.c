#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main() {
    int tokenCount = 0;
    Token* tokens = tokenize("input.c", &tokenCount);  // Tokenize input file

    printf("Pseudocode:\n");
    parse(tokens);  // Call parser for gen the token
    free(tokens); 
    return 0;
}
