#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

void parse(Token* tokens);  

void generateHTMLTree(const char* htmlFilePath);
void finalizeHTMLTree();
void printTreeNode(const char* label, const char* id);
void closeTreeNode();


#endif  
