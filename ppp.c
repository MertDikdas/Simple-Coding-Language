#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"


// gcc ppp.c lexical_analyzer.c parser.c -o ppp.exe
// ./ppp myscript

int LexicalAnalyzer(const char* filename); // Lexical Analyzer function 
int Parser(TokenNode* tokens); // Parser function 

int main(int argc, char* argv[]) {
    if (argc != 2) {  // If the number of arguments is not 2 (program name + file name)
        printf("Kullanım: ppp <dosya_adı>\n"); // Show the user the correct usage
        return 1;
    }

    const char* filename = argv[1];   // Get the file name from the command line

    // Automatically add the .ppp extension (if it doesn't exist)
    char full_filename[256];  // Create an array large enough for the file name
    if (strstr(filename, ".ppp") == NULL) { // If the file name does not have the ‘.ppp’ extension
        snprintf(full_filename, sizeof(full_filename), "%s.ppp", filename); // Add ‘.ppp’ to the file name
    } else {
        strncpy(full_filename, filename, sizeof(full_filename));  // Copy the file name as is
    }

    // Lexical analysis
    if (LexicalAnalyzer(full_filename) != 0) { // If there is an error in lexical analysis
        printf("There is an error in LEXICAL ANALYZER.\n");  // Display error message
        return 1;
    }


    // Parser (syntax analyser)
    if (Parser(tokenHead) != 0) { // If an error was returned from the parser function
        printf("Parser'da hata oluştu.\n"); // Print error message
        return 1;
    }

    return 0;
}