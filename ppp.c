#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"


// gcc ppp.c lexical_analyzer.c parser.c -o ppp.exe
// ./ppp myscript

int LexicalAnalyzer(const char* filename);
int Parser(TokenNode* tokens);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Kullanım: ppp <dosya_adı>\n");
        return 1;
    }

    const char* filename = argv[1];

    // .ppp uzantısını otomatik ekle (eğer yoksa)
    char full_filename[256];
    if (strstr(filename, ".ppp") == NULL) {
        snprintf(full_filename, sizeof(full_filename), "%s.ppp", filename);
    } else {
        strncpy(full_filename, filename, sizeof(full_filename));
    }

    printf("İşlenen dosya: %s\n", full_filename);
    // Lexical analiz
    if (LexicalAnalyzer(full_filename) != 0) {
        printf("There is an error in LEXICAL ANALYZER.\n");
        return 1;
    }


    // Parser (söz dizimi çözümleyici)
    if (Parser(tokenHead) != 0) {
        printf("Parser'da hata oluştu.\n");
        return 1;
    }

    return 0;
}