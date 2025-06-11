#include <stdio.h>
#include "token.h"

typedef struct {
    char *value_name;
    char *value;
} Values;

typedef struct ValuesNode {
    Values value;
    struct ValuesNode* next;
} ValuesNode;

int Parser(TokenNode* tokens) {
    /*TokenNode* current = tokenHead;
    
    while (tokenCurrent != NULL) {
        current = current->next;
    }
    switch (current->token.type)
    {
    case "Keyword":
        switch (current->token.value)
        {
        case "number":
            while(current!=tail)
            {
                if()
            }
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
    // Gerçek parser mantığını buraya ekle
    return 0;*/
    return 0;
}