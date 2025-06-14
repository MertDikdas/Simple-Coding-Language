#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
    char *type;
    char *value;
} Token;

typedef struct TokenNode {
    Token token;
    struct TokenNode* next;
} TokenNode;

extern TokenNode* tokenTail;
extern TokenNode* tokenHead;
extern TokenNode* tokenCurrent;
extern Token* tokens;
// Token listesine erişim için
TokenNode* getTokenList(void);

// Token eklemek için (opsiyonel)
void addToken(const char* type, const char* value);


#endif
