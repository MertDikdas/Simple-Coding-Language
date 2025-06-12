#include <stdio.h>
#include "token.h"
#include <string.h>
#include <stdlib.h>

typedef struct 
{
    char value_name[21];     // Değişken ismi (en fazla 20 karakter + \0)
    long long value;         // 100 basamaklı sayılar için büyük tamsayı (istersen char[101] da yaparız)
} 
Values;

typedef struct VariableNode 
{
    Values val;
    struct VariableNode* next;
} 
VariableNode;

VariableNode* variableHead=NULL;

int moveCurrentToken() //currentToken'ı bir ileri almak için
{
    tokenCurrent=tokenCurrent->next;
}

int moveCurrentTokenToHead() //currentToken'ı geri head'e eşitlemek için
{
    tokenCurrent=tokenHead;
}

void declareVariable(char*name){//Variable Declare etmek için

    VariableNode* current; //VariableNodeları dolaşmak için bir current node'u oluşturuyoz
    current=variableHead; //bunu variableHead'e eşitliyoz

    if (current==NULL){ //current NULL'sa yani head NULL demek oluyor
        current = (VariableNode*)malloc(sizeof(VariableNode)); //bellekte burası için yer açıyoruz
        strcpy(current->val.value_name, name); //ismi kaydediyoruz
        current->val.value=0; //bütün declarationların sonucunda 0 kaydediyoruz
        current->next = NULL; //next'i null yapıyoruz
        variableHead = current;//sonra variableHead'i current'a eşitliyoruz
        return; //fonksiyon daha devam etmesin diye
    }
    //Composite
    //Abstract Fact.
    //builder
    if (strcmp(current->val.value_name, name)==0){ //burada current'taki name ile yeni name eşitse eğer return ediyoruz
        return;
    }

    while (current->next!=NULL)   
    {
        if (strcmp(current->next->val.value_name, name)==0){ //burada nextlere bakıyoruz eğer herhangi biri olan bir değere eşitse sadece return ediyoruz(belki eğer değeri atanmışsa tekrar 0 yapabiliriz veya hata verebiliriz)
            return;
        }
        current=current->next; //bir sonraki node'a geçiyoruz

                
    }
    current->next = (VariableNode*)malloc(sizeof(VariableNode)); //En son while'da return olmadan çıktıysa demekki yeni değere gelmişiz ve next'e bellekte yer atıyoruz
    current=current->next; // current'ımız next'e eşitliyoruz
    strcpy(current->val.value_name, name); //current'in valuesinin name'sini name değerine eşitliyoruz
    current->val.value=0; //current'ın valuesiini 0'a eşitliyoruz
    current->next = NULL;//current'ın nextini null yapıyoruz
    return;
}

int parseDeclaration()
{
    if (strcmp(tokenCurrent->token.type, "Keyword") == 0 && strcmp(tokenCurrent->token.value, "number") == 0) {//keyword ve number'sa
        moveCurrentToken(); //currentToken'ı bir ileri alıyoruz
        if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "Identifier") != 0) { //currentTokenın type'ı identifier değilse
            printf("Error: Expected variable name after 'number'.\n");//hata veriyoruz
            return 1;//1 döndürüyoruz
        }

        printf("Variable declaration: %s\n", tokenCurrent->token.value); //Test amaçlı yazdırma
        TokenNode* previous=tokenCurrent;//anlık identifier token'ını kaydediyoruz 
        moveCurrentToken();//currentToken'ı ileri alıyoruz


        if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "EndOfLine") != 0) { //Currentoken endofline mı diye bakıyoruz
            printf("Error: Expected ';' after variable declaration.\n");//değilse hata veriyoruz
            return 1;
        }
        else//endofline ise declareVariable fonk çağırıyoruz ve previousTaki identifier'ın valuesini gönderiyoruz
        {
            declareVariable(previous->token.value);
        }
    
    }
}

//parse fonksiyonları buraya
int parseAssignment();
int parseIncrement();
int parseDecrement();
int parseWrite();
int parseLoop();
//

int testDeclarations()
{
    VariableNode* while2current= variableHead;
    while (while2current!=NULL)
    {
        printf("Variable Name: %s \n",while2current->val.value_name);
        printf("Variable Value: %d \n",while2current->val.value);
        while2current=while2current->next;
    }
}

int Parser(TokenNode* tokens) {
    moveCurrentTokenToHead();//ilk başta currentTokenı head'e eşitliyoruz
    int returnValue=0; //return value'miz eğer returnvalue 1 olursa return ettirmemiz gerekiyor if ile check edicez
    while(tokenCurrent!=NULL)
    {
        returnValue=parseDeclaration();//parseDeclaration yapıyoruz
        //parse'lar

        if(returnValue==1)//return valuemize bakıp(eğer .ppp uzantılı dosyada syntax hatası varsa 1 oluyo) hata veriyoruz
        {
            return 1;
        }
        moveCurrentToken(); //currenToken'ı bir ileri alıyoruz
    }

    //TEST DECLARATİON
    testDeclarations();
    //Yazdırmak için
    
    

        /*
        // --- ASSIGNMENT / INCREMENT / DECREMENT ---
        if (strcmp(current->token.type, "Identifier") == 0) {
            char* variableName = current->token.value;
            current = current->next;

            if (current == NULL || strcmp(current->token.type, "Operator") != 0) {
                printf("Error: Expected operator after variable '%s'.\n", variableName);
                return 1;
            }
            /*
            char* operatorStr = current->token.value;
            int opIndex = getOperatorIndex(operatorStr);  // listeden indeks alıyoruz
            if (opIndex == -1) {
                printf("Error: Unknown operator '%s'.\n", operatorStr);
                return 1;
            }
            

            current = current->next;

            if (current == NULL || (strcmp(current->token.type, "Identifier") != 0 && strcmp(current->token.type, "IntConstant") != 0)) {
                printf("Error: Expected value after operator '%s'.\n", operatorStr);
                return 1;
            }

            char* rightSide = current->token.value;

            // İşlemi indeks ile ayırt ediyoruz
            switch (opIndex) {
                case 0:
                    printf("Assignment: %s := %s\n", variableName, rightSide);
                    break;
                case 1:
                    printf("Increment: %s += %s\n", variableName, rightSide);
                    break;
                case 2:
                    printf("Decrement: %s -= %s\n", variableName, rightSide);
                    break;
                default:
                    printf("Error: Unexpected operator index.\n");
                    return 1;
            }

            current = current->next;
            if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) {
                printf("Error: Expected ';' after operation.\n");
                return 1;
            }

            current = current->next;
            continue;
        }


    }
    */

    return 0;
}