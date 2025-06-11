#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical_analyzer.h"
#include "parser.h"

// gcc ppp.c lexical_analyzer.c parser.c -o ppp.exe
// ./ppp myscript


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

    // Lexical analiz
    if (LexicalAnalyzer(full_filename) != 0) {
        printf("Lexical analizde hata oluştu.\n");
        return 1;
    }

    // Token listesini al
    TokenNode* tokens = getTokenList();

    // Parser (söz dizimi çözümleyici)
    if (Parser(tokens) != 0) {
        printf("Parser'da hata oluştu.\n");
        return 1;
    }

    return 0;
}