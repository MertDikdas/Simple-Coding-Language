#include <stdio.h>
#include "parser.h"

int Parser(TokenNode* tokens) {
    TokenNode* current = head;
    while (current != NULL) {
        printf("%s(%s)\n", current->token.type, current->token.value);
        current = current->next;
    }
    // Gerçek parser mantığını buraya ekle
    return 0;
}