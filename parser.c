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
 
int moveCurrentToken(TokenNode** current) //currentToken'ı bir ileri almak için
{
    *current=(*current)->next;
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
 
int parseDeclaration(TokenNode* current)
{
    current=current->next;
    if (current == NULL || strcmp(current->token.type, "Identifier") != 0) { //currentTokenın type'ı identifier değilse
        printf("Error: Expected variable name after 'number'.\n");//hata veriyoruz
        return 1;//1 döndürüyoruz
    }
 
    TokenNode* previous=current;//anlık identifier token'ını kaydediyoruz
    current=current->next;
 
 
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) { //Currentoken endofline mı diye bakıyoruz
        printf("Error: Expected ';' after variable declaration.\n");//değilse hata veriyoruz
        return 1;
    }
    else//endofline ise declareVariable fonk çağırıyoruz ve previousTaki identifier'ın valuesini gönderiyoruz
    {
        declareVariable(previous->token.value);
    }
    return 0;
}

int executeDeclaration(TokenNode** current)
{
    moveCurrentToken(current);
    if ((*current) == NULL || strcmp((*current)->token.type, "Identifier") != 0) { //currentTokenın type'ı identifier değilse
        printf("Error: Expected variable name after 'number'.\n");//hata veriyoruz
        return 1;//1 döndürüyoruz
    }
    moveCurrentToken(current);//currentToken'ı ileri alıyoruz
    if ((*current) == NULL || strcmp((*current)->token.type, "EndOfLine") != 0) { //Currentoken endofline mı diye bakıyoruz
        printf("Error: Expected ';' after variable declaration.\n");//değilse hata veriyoruz
        return 1;
    }
    return 0;
}
 
//parse fonksiyonları buraya
int parseAssignment(TokenNode* current)// := operatörüyle değer atamak için
{
    TokenNode* firstVarToken=current;
    current=current->next;
 
    // := değilse, bu atama değildir
    if (current == NULL || strcmp(current->token.type, "Operator") != 0 || strcmp(current->token.value, ":=") != 0) {
        return 1;
    }
 
    current=current->next; // değere geç
   
    // Atanacak değer identifier veya int olmalı
    if (current == NULL ||
        (strcmp(current->token.type, "Identifier") != 0 && strcmp(current->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after ':=' operator.\n");
        return 1;
    }
    int isIdentifier=0;
    if(strcmp(current->token.type, "Identifier") == 0)
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=current;
    // test çıktısı
    //printf("Assignment: %s := %s\n", variableName, current->token.value);
    current=current->next;
 
    // satırın sonu kontrolü
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) {
        printf("Error: Expected ';' after assignment.\n");
        return 1;
    }
    VariableNode* variableCurrent=variableHead;
    VariableNode* firstvar=NULL;
    if(isIdentifier==1)
    {
        VariableNode* secondvar=NULL;
        while(variableCurrent!=NULL)
        {
            if(strcmp(variableCurrent->val.value_name,firstVarToken->token.value)==0)
            {
                firstvar=variableCurrent;
               
            }
            if(strcmp(variableCurrent->val.value_name,secondVarToken->token.value)==0)
            {
                secondvar=variableCurrent;
            }
            variableCurrent=variableCurrent->next;
        }
 
        if(firstvar!=NULL && secondvar!=NULL)
        {
            firstvar->val.value=secondvar->val.value;
        }
        else if(firstvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value);
            return 1;
        }    
        else if(secondvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",secondVarToken->token.value);
            return 1;
        }
        else{
            printf("ERROR:Some variables are not defined!!\n");
            return 1;
        }
    }
    else {
 
        while(variableCurrent!=NULL && strcmp(variableCurrent->val.value_name,firstVarToken->token.value)!=0)
        {
            variableCurrent=variableCurrent->next;
        }
 
        if(variableCurrent->val.value_name!=NULL)
        {
            const char* tempValue=secondVarToken->token.value;
            char *endptr=NULL;
            long long tempInt=strtoll(tempValue,&endptr,10);
            if (*endptr != '\0') {
                printf("ERROR: NOT AN INTEGER: %s\n", endptr);
            } else {
                variableCurrent->val.value=tempInt;
            }
        }
        else{
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value);
            return 1;
        }
    }
    return 0;
}

int parseIncrement(TokenNode* current) // += operatörüyle arttırma işlemi için
{
    TokenNode* firstVarToken=current;  
    current=current->next;
 
    // += değilse bu fonksiyona ait değil
    if (current == NULL || strcmp(current->token.type, "Operator") != 0 || strcmp(current->token.value, "+=") != 0) {
        return 1;
    }
 
    current=current->next;
 
    // Sağdaki değer uygun değilse hata ver
    if (current == NULL ||
        (strcmp(current->token.type, "Identifier") != 0 && strcmp(current->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after '+= operator.\n");
        return 1;
    }
    int isIdentifier=0;
    if(strcmp(current->token.type, "Identifier") == 0)
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=current;
    // test çıktısı
    current=current->next;
 
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) {
        printf("Error: Expected ';' after increment.\n");
        return 1;
    }
 
    VariableNode* variableCurrent=variableHead;
    VariableNode* firstvar=NULL;
    if(isIdentifier==1)
    {
        VariableNode* secondvar=NULL;
        while(variableCurrent!=NULL)
        {
            if(strcmp(variableCurrent->val.value_name,firstVarToken->token.value)==0)
            {
                firstvar=variableCurrent;
               
            }
            if(strcmp(variableCurrent->val.value_name,secondVarToken->token.value)==0)
            {
                secondvar=variableCurrent;
            }
            variableCurrent=variableCurrent->next;
        }
 
        if(firstvar!=NULL && secondvar!=NULL)
        {
            firstvar->val.value=firstvar->val.value+secondvar->val.value;
        }
        else if(firstvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value);
            return 1;
        }    
        else if(secondvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",secondVarToken->token.value);
            return 1;
        }
    }
    else {
 
        while(variableCurrent!=NULL && strcmp(variableCurrent->val.value_name,firstVarToken->token.value)!=0)
        {
            variableCurrent=variableCurrent->next;
        }
 
        if(variableCurrent!=NULL)
        {
            const char* tempValue=secondVarToken->token.value;
            char *endptr=NULL;
            long long tempInt=strtoll(tempValue,&endptr,10);
            if (*endptr != '\0') {
                printf("ERROR: NOT AN INTEGER: %s\n", endptr);
            } else {
                variableCurrent->val.value=variableCurrent->val.value+tempInt;
            }
           
           
           
        }
        else{
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value);
            return 1;
        }
    }
    return 0;
}
 
int parseDecrement(TokenNode* current) // -= operatörüyle azaltma işlemi için
{
    TokenNode* firstVarToken=current;  
    current=current->next;
    // -= değilse bu fonksiyona ait değil
    if (current == NULL || strcmp(current->token.type, "Operator") != 0 || strcmp(current->token.value, "-=") != 0) {
        return 1;
    }
 
    current=current->next;
 
    // Sağdaki değer uygun değilse hata ver
    if (current == NULL ||
        (strcmp(current->token.type, "Identifier") != 0 && strcmp(current->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after '-=' operator.\n");
        return 1;
    }
    int isIdentifier=0;
    if(strcmp(current->token.type, "Identifier") == 0)
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=current;
    // test çıktısı
    current=current->next;
 
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) {
        printf("Error: Expected ';' after decrement.\n");
        return 1;
    }
 
    VariableNode* variableCurrent=variableHead;
    VariableNode* firstvar=NULL;
    if(isIdentifier==1)
    {
        VariableNode* secondvar=NULL;
        while(variableCurrent!=NULL)
        {
            if(strcmp(variableCurrent->val.value_name,firstVarToken->token.value)==0)
            {
                firstvar=variableCurrent;
               
            }
            if(strcmp(variableCurrent->val.value_name,secondVarToken->token.value)==0)
            {
                secondvar=variableCurrent;
            }
            variableCurrent=variableCurrent->next;
        }
 
        if(firstvar!=NULL && secondvar!=NULL)
        {
            firstvar->val.value=firstvar->val.value-secondvar->val.value;
        }
        else if(firstvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value);
            return 1;
        }    
        else if(secondvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",secondVarToken->token.value);
            return 1;
        }
    }
    else {
 
        while(variableCurrent!=NULL && strcmp(variableCurrent->val.value_name,firstVarToken->token.value)!=0)
        {
            variableCurrent=variableCurrent->next;
        }
 
        if(variableCurrent!=NULL)
        {
            const char* tempValue=secondVarToken->token.value;
            char *endptr=NULL;
            long long tempInt=strtoll(tempValue,&endptr,10);
            if (*endptr != '\0') {
                printf("ERROR: NOT AN INTEGER: %s\n", endptr);
            } else {
                variableCurrent->val.value=variableCurrent->val.value-tempInt;
            }
           
           
           
        }
        else{
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value);
            return 1;
        }
    }
    return 0;
}

int executeAssignIncrDecre(TokenNode** current)// := operatörüyle değer atamak için
{
    moveCurrentToken(current);
    moveCurrentToken(current); // değere geç
    moveCurrentToken(current);
    return 0;
}

int parseWrite(TokenNode* current) {
    current=current->next;
 
    while (current != NULL && strcmp(current->token.type, "EndOfLine") != 0) {
        if (strcmp(current->token.type, "StringConst") == 0 ||
            strcmp(current->token.type, "Identifier") == 0 ||
            strcmp(current->token.value, "newline") == 0 ||
            strcmp(current->token.type, "IntConstant") == 0) {
 
            // newline yazmak için özel kontrol
            if (strcmp(current->token.value, "newline") == 0) {
                printf("\n");
            }
            else if(strcmp(current->token.type, "Identifier") == 0)
            {
                VariableNode* variableCurrent=variableHead;
                VariableNode* variable1=NULL;
                while(variableCurrent!=NULL)
                {
                    if(strcmp(variableCurrent->val.value_name,current->token.value)==0)
                    {
                        variable1=variableCurrent;
                        break;
                    }
                    variableCurrent=variableCurrent->next;
                   
                }
 
                if(variable1==NULL){
                    printf("\nERROR: %s not declareted.\n",current->token.value);
                    return 1;
                }
                else if(strcmp(variable1->val.value_name,current->token.value)==0)
                {
                    printf("%d",variableCurrent->val.value);
                }
               
            }else if(strcmp(current->token.type, "StringConst") == 0) 
            {
                char* tempValue= current->token.value;
                size_t len = strlen(tempValue);
                // Yeni string için bellek ayır (sonda \0 için +1)
                char* trimmed = (char*)malloc(len - 1);  // len - 2 karakter + \0
                if (trimmed == NULL) return 0;
                // input[1] ile input[len - 2] arasını kopyala
                strncpy(trimmed, tempValue + 1, len - 2);
                trimmed[len - 2] = '\0';  // null-terminator
                printf("%s", trimmed);
            }else if(strcmp(current->token.type, "IntConstant") == 0)
            {
                printf("%s", current->token.value);
            }
            else {
                printf("%s", current->token.value);
            }
        } else {
            // Geçersiz bir ifade varsa hata ver
            printf("\nError: Invalid expression in write: %s (%s)\n", current->token.type, current->token.value);
            return 1;
        }
        if(current->next!=NULL)
        {
            current=current->next;
            if(strcmp(current->token.value, ";") == 0)
            {
                break;
            }
        }
        if(strcmp(current->token.value, ";") != 0 && strcmp(current->token.value, "and") != 0) 
        {
            printf("\nERROR: EXCEPTED AND.");
            return 1;
        }
        if(current->next!=NULL)
        {
            current=current->next;
        }
        
    }
 
    // EndOfLine kontrolü
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) {
        printf("\nError: Missing ';' at the end of write.\n");
        return 1;
    }
 
    printf("\n");
   
    return 0;
}

int executeWrite(TokenNode** current)
{
    moveCurrentToken(current); // write'tan sonraki tokena geç
 
    while ((*current) != NULL && strcmp((*current)->token.type, "EndOfLine") != 0) {
        moveCurrentToken(current);
    }
 
    // EndOfLine kontrolü
    if ((*current) == NULL || strcmp((*current)->token.type, "EndOfLine") != 0) {
        printf("\nError: Missing ';' at the end of write.\n");
        return 1;
    }  
    return 0;
}




int parseLoop(TokenNode** current)
{
    moveCurrentToken(current);
    if((*current)==NULL)
    {
        printf("ERROR.INVALID EXPRESSİON\n");
        return 1;
    }
    if(strcmp((*current)->token.type,"Identifier")!=0 && strcmp((*current)->token.type,"IntConstant")!=0)
    {
        printf("ERROR : HAVE TO BE AN IDENTIFIER AFTER REPEAT.\n");
        return 1;
    }

    const char* tempValue=(*current)->token.value;
    char *endptr=NULL;
    long long size=strtoll(tempValue,&endptr,10);

    if (*endptr != '\0') {
        printf("ERROR: NOT AN INTEGER: %s\n", endptr);
    }
    
    moveCurrentToken(current);
    if(strcmp((*current)->token.value,"times")!=0)
    {
        printf("ERROR : HAVE TO BE AN times KEYWORD.\n");
        return 1;
    }
    moveCurrentToken(current);
    int returnValue=0;
    if(strcmp((*current)->token.value, "{")==0)
    {
        for(int i=size;i>=1;i--)
        {
            TokenNode* localCurrent=*current;
            moveCurrentToken(&localCurrent);
            while(strcmp(localCurrent->token.value, "}")!=0)
            { 
                if (strcmp(localCurrent->token.type, "Keyword") == 0 && strcmp(localCurrent->token.value, "number") == 0) {//keyword ve number'sa
                    returnValue = parseDeclaration(localCurrent);
                    if(returnValue==0)
                    {
                        returnValue=executeDeclaration(&localCurrent);
                    }
                    
                    
                }
        
                if (strcmp(localCurrent->token.type, "Identifier") == 0) {
                    if(strcmp(localCurrent->next->token.type,"Operator")!=0)
                    {
                        printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                        return 1;
                    }
                
                    if(strcmp(localCurrent->next->token.value,":=")==0)
                    {
                        returnValue = parseAssignment(localCurrent);
                        if(returnValue==0)
                        {
                            returnValue=executeAssignIncrDecre(&localCurrent);
                        }
                    }
                    else if(strcmp(localCurrent->next->token.value,"+=")==0)
                    {
                        returnValue = parseIncrement(localCurrent);
                        if(returnValue==0)
                        {
                            returnValue=executeAssignIncrDecre(&localCurrent);
                        }
                    }
                    else if(strcmp(localCurrent->next->token.value,"-=")==0)
                    {
                        returnValue = parseDecrement(localCurrent);
                        if(returnValue==0)
                        {
                            returnValue=executeAssignIncrDecre(&localCurrent);
                        }  
                    }
                    else{
                        printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                        return 1;
                    }
                
                }
                
                if (strcmp(localCurrent->token.type, "Keyword") == 0 && strcmp(localCurrent->token.value, "write") == 0) {
                    returnValue = parseWrite(localCurrent);
                    if(returnValue==0)
                    {
                        returnValue=executeWrite(&localCurrent);
                    }  

                }
                
                if (strcmp(localCurrent->token.type, "Keyword") == 0 && strcmp(localCurrent->token.value, "repeat") == 0) {  // repeat kelimesi gelmiş mi diye kontrol ediyoruz
                    
                    returnValue = parseLoop(&localCurrent);
                }
            
        
                if (returnValue != 0)
                {
                    return 1;
                }
                
                moveCurrentToken(&localCurrent); //currenToken'ı bir ileri alıyoruz
            }
            
        }
        while((*current)!=NULL && strcmp((*current)->token.value, "}")!=0)
        {
            *current=(*current)->next;
        } 
        if(*current==NULL)
        {
            return 1;
        } 
        return 0;

    }
    else
    {
        for(int i=size; i>=1;i--)
        {
            TokenNode* localCurrent=*current;
            if(strcmp((*current)->token.value, "repeat")==0)
            {
                parseLoop(&localCurrent);
            }
            else if (strcmp((*current)->token.type, "Keyword") == 0 && strcmp((*current)->token.value, "number") == 0) {//keyword ve number'sa
                returnValue = parseDeclaration(*current);
                if(returnValue)
                {
                    return 1;
                }
            }
            else if (strcmp((*current)->token.type, "Identifier") == 0) {
                if(strcmp((*current)->next->token.type,"Operator")!=0)
                {
                    printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                    return 1;
                }
            
                if(strcmp((*current)->next->token.value,":=")==0)
                {
                    returnValue = parseAssignment(*current);
                    if(returnValue)
                    {
                        return 1;
                    }
                }
                else if(strcmp((*current)->next->token.value,"+=")==0)
                {
                    returnValue = parseIncrement(*current);
                    if(returnValue)
                    {
                        return 1;
                    }
                }
                else if(strcmp((*current)->next->token.value,"-=")==0)
                {
                    returnValue = parseDecrement(*current);
                    if(returnValue)
                    {
                        return 1;
                    }
                }
                else{
                    printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                    return 1;
                }
            
            }
            else if (strcmp((*current)->token.type, "Keyword") == 0 && strcmp((*current)->token.value, "write") == 0) {
                returnValue = parseWrite(*current);
                
                if(returnValue)
                {
                    return 1;
                }
            }
            else{
                printf("ERROR:%s\n",(*current)->token.value);
                return 1;
            }
        }

        if(strcmp((*current)->token.value, "number")==0)
        {
            executeDeclaration(current);
            return 0;
        }
        else if(strcmp((*current)->token.type, "Identifier")==0)
        {
            executeAssignIncrDecre(current);
            return 0;
        }
        else if(strcmp((*current)->token.value, "write")==0)
        {
            executeWrite(current);
            return 0;
        }
        else if(strcmp((*current)->token.value, "repeat")==0)
        {
            while ((*current) != NULL && strcmp((*current)->token.value, "}") != 0) {
                moveCurrentToken(current);
            }
            if (*current != NULL)               /* '}' üzerinde duruyorsak, onu da geç */
                moveCurrentToken(current);      /* { 3. satırı: bloğun dışına çık    */
            return 0;
        }
    }

}
 
/*int parseLoop(TokenNode** current)
{
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
   
     if (*current == NULL ||
        (strcmp((*current)->token.type, "IntConstant") != 0 && strcmp((*current)->token.type, "Identifier") != 0)) { //repeatten sonra IntConstant veya Identifier gelmiş mi diye kontrol ediyoruz
        printf("Error: 'IntConstant' or 'Identifier' expected after 'repeat' statement.\n");
        return 1;
    }
 
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
 
    if (*current == NULL || strcmp((*current)->token.value, "times") != 0) { // times kelimesi gelmiş mi diye bakıyoruz
    printf("Error: ‘times’ is expected after the ‘IntConstant’ or ‘Identifier’ statement.\n");
    return 1;
    }
 
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
 
    if (*current == NULL) { //timestan sonra loop döngüsü boş gelmişse
        printf("Error: Loop body missing.\n");
        return 1;
    }
 
    if (strcmp((*current)->token.value, "{") == 0) { // Blok varsa parseCodeBlock metodunu çağırıyoruz
        return parseCodeBlock(current);
    }
 
    else {
        return parseLine(current);         // Tek satır kod varsa parseLine metodunu çağırıyoruz
    }  
}
 
int parseLine(TokenNode** current) { // Döngüde Tek satır kod  metodu
    if (*current == NULL) {
        printf("Error: Unexpected end of input in parseLine.\n");
        return 1;
    }
 
    if (strcmp((*current)->token.type, "Keyword") == 0) {
        if (strcmp((*current)->token.value, "write") == 0) {
            return parseWrite(current);
        }
        else if (strcmp((*current)->token.value, "repeat") == 0) {
            return parseLoop(current);
        }
        else {
            printf("Error: Unknown keyword in parseLine: %s\n", (*current)->token.value);
            return 1;
        }
    }
    else if (strcmp((*current)->token.type, "Identifier") == 0) {
        // parseAssignment, parseIncrement ve parseDecrement'ı sırayla deniyoruz
        if (parseAssignment(current) == 0) {
            return 0;
        }
        else if (parseIncrement(current) == 0) {
            return 0;
        }
        else if (parseDecrement(current) == 0) {
            return 0;
        }
        else {
            printf("Error: Invalid statement starting with identifier %s\n", (*current)->token.value);
            return 1;
        }
    }
    else {
        printf("Error: Unexpected token type in parseLine: %s\n", (*current)->token.type);
        return 1;
    }
}
 
int parseCodeBlock(TokenNode** current) {
   
    if (*current == NULL || strcmp((*current)->token.value, "{") != 0) { // current şu an '{' tokeninde olmalı
        printf("Error: Expected '{' at the beginning of code block.\n");
        return 1;
    }
 
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
 
   
    while (*current != NULL && strcmp((*current)->token.value, "}") != 0) { // '}' gelene kadar döngüyü devam ettirip her satırda parseLine'ı çağır
        int x = parseLine(current);
        if (x != 0) {
            return 1;
        }
    }
 
   
    if (*current == NULL || strcmp((*current)->token.value, "}") != 0) { // Döngüden çıkınca '}' token'ını kontrol et
        printf("Error: Expected '}' at the end of code block.\n");
        return 1;
    }
 
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
 
    return 0;
}
 */
// devamını yazıcam
 
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
    moveCurrentTokenToHead();
    int returnValue = 0;
 
    while (tokenCurrent != NULL) {
        if (strcmp(tokenCurrent->token.type, "Keyword") == 0 && strcmp(tokenCurrent->token.value, "number") == 0) {//keyword ve number'sa
            returnValue = parseDeclaration(tokenCurrent);
            if(returnValue==0)
            {
                returnValue=executeDeclaration(&tokenCurrent);
            }
            
            
        }
 
        if (strcmp(tokenCurrent->token.type, "Identifier") == 0) {
            if(strcmp(tokenCurrent->next->token.type,"Operator")!=0)
            {
                printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                return 1;
            }
           
            if(strcmp(tokenCurrent->next->token.value,":=")==0)
            {
                
                returnValue = parseAssignment(tokenCurrent);
                if(returnValue==0)
                {
                    returnValue=executeAssignIncrDecre(&tokenCurrent);
                }
            }
            else if(strcmp(tokenCurrent->next->token.value,"+=")==0)
            {
                returnValue = parseIncrement(tokenCurrent);
                if(returnValue==0)
                {
                    returnValue=executeAssignIncrDecre(&tokenCurrent);
                }
            }
            else if(strcmp(tokenCurrent->next->token.value,"-=")==0)
            {
                returnValue = parseDecrement(tokenCurrent);
                if(returnValue==0)
                {
                    returnValue=executeAssignIncrDecre(&tokenCurrent);
                }  
            }
            else{
                printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                return 1;
            }
           
        }
 
        if (strcmp(tokenCurrent->token.type, "Keyword") == 0 && strcmp(tokenCurrent->token.value, "write") == 0) {
            returnValue = parseWrite(tokenCurrent);
            if(returnValue==0)
            {
                returnValue=executeWrite(&tokenCurrent);
            }  

        }
 
        if (strcmp(tokenCurrent->token.type, "Keyword") == 0 && strcmp(tokenCurrent->token.value, "repeat") == 0) {  // repeat kelimesi gelmiş mi diye kontrol ediyoruz
            returnValue = parseLoop(&tokenCurrent);
        }
       
 
        if (returnValue != 0)
        {
            return 1;
        }
        moveCurrentToken(&tokenCurrent); //currenToken'ı bir ileri alıyoruz
    }
 
 
    //TEST DECLARATİON
    testDeclarations();
    return 0;
}