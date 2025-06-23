#include <stdio.h>
#include "tree.h"

FILE* htmlTreeFile;

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
    if (htmlTreeFile) {
        fprintf(htmlTreeFile, "</ul></div></body></html>");
        fclose(htmlTreeFile);
    }
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
