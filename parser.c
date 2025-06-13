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
    return 0;

    
}

//parse fonksiyonları buraya
int parseAssignment()
// := operatörüyle değer atamak için
{
    TokenNode* firstVarToken=tokenCurrent;   
    moveCurrentToken(); // operatöre geçiyoruz

    // := değilse, bu atama değildir
    if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "Operator") != 0 || strcmp(tokenCurrent->token.value, ":=") != 0) {
        return 1;
    }

    moveCurrentToken(); // değere geç
    
    // Atanacak değer identifier veya int olmalı
    if (tokenCurrent == NULL || 
        (strcmp(tokenCurrent->token.type, "Identifier") != 0 && strcmp(tokenCurrent->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after ':=' operator.\n");
        return 1;
    }
    int isIdentifier=0;
    if(strcmp(tokenCurrent->token.type, "Identifier") == 0)
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=tokenCurrent;
    // test çıktısı
    //printf("Assignment: %s := %s\n", variableName, tokenCurrent->token.value);
    moveCurrentToken();

    // satırın sonu kontrolü
    if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "EndOfLine") != 0) {
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

int parseIncrement()
// += operatörüyle arttırma işlemi için
{
    TokenNode* firstVarToken=tokenCurrent;   
    moveCurrentToken();

    // += değilse bu fonksiyona ait değil
    if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "Operator") != 0 || strcmp(tokenCurrent->token.value, "+=") != 0) {
        return 1;
    }

    moveCurrentToken();

    // Sağdaki değer uygun değilse hata ver
    if (tokenCurrent == NULL || 
        (strcmp(tokenCurrent->token.type, "Identifier") != 0 && strcmp(tokenCurrent->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after '+= operator.\n");
        return 1;
    }
    int isIdentifier=0;
    if(strcmp(tokenCurrent->token.type, "Identifier") == 0)
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=tokenCurrent;
    // test çıktısı
    moveCurrentToken();

    if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "EndOfLine") != 0) {
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


int parseDecrement()
// -= operatörüyle azaltma işlemi için
{
    TokenNode* firstVarToken=tokenCurrent;   
    moveCurrentToken();
    // -= değilse bu fonksiyona ait değil
    if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "Operator") != 0 || strcmp(tokenCurrent->token.value, "-=") != 0) {
        return 1;
    }

    moveCurrentToken();

    // Sağdaki değer uygun değilse hata ver
    if (tokenCurrent == NULL || 
        (strcmp(tokenCurrent->token.type, "Identifier") != 0 && strcmp(tokenCurrent->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after '-=' operator.\n");
        return 1;
    }
    int isIdentifier=0;
    if(strcmp(tokenCurrent->token.type, "Identifier") == 0)
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=tokenCurrent;
    // test çıktısı
    moveCurrentToken();

    if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "EndOfLine") != 0) {
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

int parseWrite() {
    
    moveCurrentToken(); // write'tan sonraki tokena geç

    printf("WRITE: ");

    while (tokenCurrent != NULL && strcmp(tokenCurrent->token.type, "EndOfLine") != 0) {
        if (strcmp(tokenCurrent->token.type, "StringConst") == 0 ||
            strcmp(tokenCurrent->token.type, "Identifier") == 0 ||
            strcmp(tokenCurrent->token.value, "newline") == 0) {

            // newline yazmak için özel kontrol
            if (strcmp(tokenCurrent->token.value, "newline") == 0) {
                printf("\n");
            }
            else if(strcmp(tokenCurrent->token.type, "Identifier") == 0)
            {
                VariableNode* variableCurrent=variableHead;
                VariableNode* variable1=NULL;
                while(variableCurrent!=NULL)
                {
                    if(strcmp(variableCurrent->val.value_name,tokenCurrent->token.value)==0)
                    {
                        variable1=variableCurrent;
                        break;
                    }
                    variableCurrent=variableCurrent->next;
                    
                }

                if(variable1==NULL){
                    printf("\nERROR: %s not declareted.\n",tokenCurrent->token.value);
                    return 1;
                }
                else if(strcmp(variable1->val.value_name,tokenCurrent->token.value)==0)
                {
                    printf("%d",variableCurrent->val.value);
                }
                
            } else {
                printf("%s ", tokenCurrent->token.value);
            }

        } else if (strcmp(tokenCurrent->token.value, "and") == 0) {
            // and kelimesi geçilebilir
        } else {
            // Geçersiz bir ifade varsa hata ver
            printf("\nError: Invalid expression in write: %s (%s)\n", tokenCurrent->token.type, tokenCurrent->token.value);
            return 1;
        }

        moveCurrentToken();
    }

    // EndOfLine kontrolü
    if (tokenCurrent == NULL || strcmp(tokenCurrent->token.type, "EndOfLine") != 0) {
        printf("\nError: Missing ';' at the end of write.\n");
        return 1;
    }

    printf("\n");
    
    return 0;
}

int parseLoop(TokenNode** current)
{
   
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
   
     if (*current == NULL ||
        (strcmp((*current)->token.type, "IntConstant") != 0 && strcmp((*current)->token.type, "Identifier") != 0)) { //repeatten sonra IntConstant veya Identifier gelmiş mi diye kontrol ediyoruz
        printf("Error: 'IntConstant' or 'Identifier' expected after 'repeat' statement.\n"); // hata mesajı yazdırıyoruz
        return 1;
    }
 
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
 
    if (*current == NULL || strcmp((*current)->token.value, "times") != 0) { // times kelimesi gelmiş mi diye bakıyoruz
    printf("Error: ‘times’ is expected after the ‘IntConstant’ or ‘Identifier’ statement.\n"); // hata mesajı yazdırıyoruz
    return 1;
    }
    
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
 
    if (*current == NULL) { //timestan sonra loop döngüsü boş gelmişse
        printf("Error: Loop body missing.\n"); // hata mesajı yazdırıyoruz
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
 
    if (strcmp((*current)->token.type, "Keyword") == 0) { // Token tipi Keyword mü kontrol ediyoruz
        if (strcmp((*current)->token.value, "repeat") == 0) { // repeat komutu varsa parseLoop çağırıyoruz
            return parseLoop(current);
        }
        else {
            return 0;
            printf("Error: Unknown keyword in parseLine: %s\n", (*current)->token.value); // bilinmeyen keyword hatası
            return 1;
        }
    }
    else if (strcmp((*current)->token.type, "Identifier") == 0) { // parseAssignment, parseIncrement ve parseDecrement metodlaronı sırasıyla çağırıyoruz
       return 0;

    }
    else {
        printf("Error: Unexpected token type in parseLine: %s\n", (*current)->token.type); // keyword veya identifier gelmezse hata
        return 1;
    }
}
 
int parseCodeBlock(TokenNode** current) {
   
    if (*current == NULL || strcmp((*current)->token.value, "{") != 0) { // current şu an '{' tokeninde olmalı
        printf("Error: Expected '{' at the beginning of code block.\n"); //hata mesajı yazdırıyoruz
        return 1;
    }
 
    *current = (*current)->next; // bir sonraki tokene geçiyoruz
 
   
    while (*current != NULL && strcmp((*current)->token.value, "}") != 0) { // '}' gelene kadar döngüyü devam ettirip her satırda parseLine'ı çağır
        int x = parseLine(current); // Her satırı parseLine ile kontrol ediyoruz
        if (x != 0) {
            return 1;
        }
    }
 
   
    if (*current == NULL || strcmp((*current)->token.value, "}") != 0) { // Döngüden çıkınca '}' token'ını kontrol et
        printf("Error: Expected '}' at the end of code block.\n"); //hata mesajı yazdırıyoruz
        return 1;
    }

 
    return 0;
}
 



VariableNode* findVariable(const char* name) { // executeLoop metodu çalışması için ekledim
    VariableNode* current = variableHead; // Başlangıç node'u alıyoruz
    while (current != NULL) {
        if (strcmp(current->val.value_name, name) == 0) {
            return current; // Değişkeni buluyoruz
        }
        current = current->next; // Sonraki node'a geçiyoruz
    }
    return NULL; // bulunamazsa null döndürüyoruz
}
 
 
int executeLoop(TokenNode** current) { // loopumuzun içinde yazanı uygulayan metod
    int loopCount = 0; // Döngü sayısını tutacak değişken
    VariableNode* varNode = NULL; // identifier değişkeni için pointer
 
    if (strcmp((*current)->token.type, "IntConstant") == 0) { // parseLoop parse ettiği için  current 'IntConstant' veya 'Identifier'
        loopCount = atoi((*current)->token.value); // String değeri integera çeviriyoruz
    } else if (strcmp((*current)->token.type, "Identifier") == 0) {
        varNode = findVariable((*current)->token.value); // Değişkeni buluyoruz
        if (varNode == NULL) {
            printf("Error: Undefined variable '%s' in loop.\n", (*current)->token.value); // Bilinmeyen değişken hatası
            return 1;
        }
        loopCount = varNode->val.value; // Değeri alıyoruz
    }
 
    *current = (*current)->next; // 'times'
    *current = (*current)->next; // loop body'si (ya '{' ya da tek satır)
    
    if (strcmp((*current)->token.value, "{") == 0) {
        return parseCodeBlock(current); //parseCodeBlock metodunu çağırıyoruz
    } else {
        return parseLine(current);  //parseLine metodunu çağırıyoruz
    }

    TokenNode* loopBodyStart = *current; // Döngü başını tutuyoruz
 
    for (int i = loopCount; i >= 1; i--) {
        if (varNode != NULL) {
            varNode->val.value = i; // Değişken varsa her iterasyonda değerini güncelliyoruz
        }
 
       
        TokenNode* inner = loopBodyStart; // Loop içeriğini baştan başlatıyoruz
 
        if (strcmp(inner->token.value, "{") == 0) {
            if (executeCodeBlock(&inner) != 0) return 1; // Kod bloğunu çalıştırıyoruz
        } else {
            if (executeLine(&inner) != 0) return 1; // Tek satır komutu çalıştırıyoruz
        }
    }
 
    if (varNode != NULL) {
        varNode->val.value = 0; // Döngü bitince değeri sıfırlıyoruz
    }
 
    return 0;
}
 
int executeLine(TokenNode** current) {
    if (*current == NULL) {
        printf("Error: Unexpected end of input in executeLine.\n");
        return 1;
    }
 
    if (strcmp((*current)->token.type, "Keyword") == 0) { // Keyword tokeni gelmiş mi diye bakıyoruz
        if (strcmp((*current)->token.value, "write") == 0) {
            return parseWrite(current);  // parseWrite yazılınca güncellenir
        }
        else if (strcmp((*current)->token.value, "repeat") == 0) {
            return executeLoop(current); // executeLoop'u çağırıyoruz
        }
        else {
            printf("Error: Unknown keyword in executeLine: %s\n", (*current)->token.value);
            return 1;
        }
    }
    else if (strcmp((*current)->token.type, "Identifier") == 0) { //  Identidfier tokeni gelmiş mi diye bakıyoruz.Sırayla assigmentleri çalıştırıyoruz(buranın içi değişebilir)
        if(strcmp(tokenCurrent->next->token.type,"Operator")!=0)
        {
            printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
            return 1;
        }
        
        if(strcmp(tokenCurrent->next->token.value,":=")==0)
        {
            return parseAssignment();
        }
        else if(strcmp(tokenCurrent->next->token.value,"+=")==0)
        {
            return parseIncrement();
        }
        else if(strcmp(tokenCurrent->next->token.value,"-=")==0)
        {
            return parseDecrement();   
        }
        else{
            printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
            return 1;
        }
    }
    else {
        printf("Error: Unexpected token type in executeLine: %s\n", (*current)->token.type); // İdentifier veya keywords gelmezse hata mesajı veriyoruz
        return 1;
    }
}
 
int executeCodeBlock(TokenNode** current) {
    if (*current == NULL || strcmp((*current)->token.value, "{") != 0) { // '{' ile başlamamışsa hata mesahı veriyoruz
        printf("Error: Expected '{' at the beginning of code block.\n"); // Hata mesajı yazıdırıyoruz
        return 1;
    }
 
    *current = (*current)->next; // Bir sonraki tokene geçiyoruz
 
 
    while (*current != NULL && strcmp((*current)->token.value, "}") != 0) {
        int res = executeLine(current); // Satırı çalıştırıyoruz
        if (res != 0) {
            return 1;
        }
    }
 
    if (*current == NULL || strcmp((*current)->token.value, "}") != 0) {
        printf("Error: Expected '}' at the end of code block.\n"); // '}' gelmiş mi diye kontrol ediyoruz
        return 1;
    }
 
    *current = (*current)->next; // Sonraki tokene geçiyoruz
    return 0;
}
  
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
            returnValue = parseDeclaration(); 
        }

        if (strcmp(tokenCurrent->token.type, "Identifier") == 0) {
            if(strcmp(tokenCurrent->next->token.type,"Operator")!=0)
            {
                printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                return 1;
            }
            
            if(strcmp(tokenCurrent->next->token.value,":=")==0)
            {
                returnValue = parseAssignment();
            }
            else if(strcmp(tokenCurrent->next->token.value,"+=")==0)
            {
                returnValue = parseIncrement();
            }
            else if(strcmp(tokenCurrent->next->token.value,"-=")==0)
            {
                returnValue = parseDecrement();   
            }
            else{
                printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTİFİER\n");
                return 1;
            }
            
        }

        if (strcmp(tokenCurrent->token.type, "Keyword") == 0 && strcmp(tokenCurrent->token.value, "write") == 0) {
            returnValue = parseWrite(); 
        }

        if (strcmp(tokenCurrent->token.type, "Keyword") == 0 && strcmp(tokenCurrent->token.value, "repeat") == 0) {  // repeat kelimesi gelmiş mi diye kontrol ediyoruz
            returnValue = parseLoop(tokenCurrent); 
        }
        

        if (returnValue != 0) 
        {
            return 1;
        }
        moveCurrentToken(); //currenToken'ı bir ileri alıyoruz
    }


    //TEST DECLARATİON
    testDeclarations();
    return 0;
}