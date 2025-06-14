#include <stdio.h>
#include "token.h"
#include <string.h>
#include <stdlib.h>
 
typedef struct
{
    char value_name[21];     // Variable name (maximum 20 characters + \0)
    long long value;         // Large integers for 100-digit numbers
}
Values;
 
typedef struct VariableNode // Structure to hold variables in a linked list structure
{
    Values val; // Variable value and name
    struct VariableNode* next; // Pointer to the next node
}
VariableNode;
 
VariableNode* variableHead=NULL; // Head of the variable list
int blockCount=0; // Code block counter

int moveCurrentToken(TokenNode** current) //to advance currentToken
{
    *current=(*current)->next;
}
 
int moveCurrentTokenToHead() //Set currentToken back to head
{
    tokenCurrent=tokenHead;
}
 
void declareVariable(char*name){//To declare a variable
 
    VariableNode* current; //We create a current node to traverse the variable nodes.
    current=variableHead; //We assign this to variableHead.
 
    if (current==NULL){ //current is NULL, which means head is NULL
        current = (VariableNode*)malloc(sizeof(VariableNode)); //we are making space in memory for this
        strcpy(current->val.value_name, name); //we are recording the name
        current->val.value=0; //We record 0 as the result of all declarations.
        current->next = NULL; // We set next to null.
        variableHead = current;//then we set variableHead equal to current
        return; //so that the function does not continue
    }
    if (strcmp(current->val.value_name, name)==0){ //If the name in current is equal to the new name, we return it.
        return;
    }
 
    while (current->next!=NULL)  
    {
        if (strcmp(current->next->val.value_name, name)==0){ //Here we look at the next values. If any of them are equal to a value, we simply return it.
            return;
        }
        current=current->next; //moving on to the next node
 
               
    }
    current->next = (VariableNode*)malloc(sizeof(VariableNode)); //If it exited without returning in the last while, it means we have reached the new value and are allocating memory for next.
    current=current->next; // We set current equal to next.
    strcpy(current->val.value_name, name); //We set the value of current to the value of name.
    current->val.value=0; //We set the value of current to 0.
    current->next = NULL;//We set the next of current to null.
    return;
}
 
int parseDeclaration(TokenNode* current)
{
    current=current->next;
    if (current == NULL || strcmp(current->token.type, "Identifier") != 0) { //if the type of currentToken is not identifier
        printf("Error: Expected variable name after 'number'.\n");//we are giving an error
        return 1;
    }
 
    TokenNode* previous=current;//We are saving the instant identifier token.
    current=current->next;
 
 
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) { //We are checking if Currentoken is endofline
        printf("Error: Expected ';' after variable declaration.\n");//otherwise we throw an error
        return 1;
    }
    else //endofline then we call the declareVariable function and send the values of the previousTaki identifier
    {
        declareVariable(previous->token.value);
    }
    return 0;
}

int executeDeclaration(TokenNode** current)
{
    moveCurrentToken(current);
    if ((*current) == NULL || strcmp((*current)->token.type, "Identifier") != 0) { //if the type of currentToken is not identifier
        printf("Error: Expected variable name after 'number'.\n");//we are giving an error
        return 1;
    }
    moveCurrentToken(current);//We are advancing currentToken
    if ((*current) == NULL || strcmp((*current)->token.type, "EndOfLine") != 0) { //We are checking if Currentoken is endofline
        printf("Error: Expected ';' after variable declaration.\n");//otherwise we throw an error
        return 1;
    }
    return 0;
}
 
int parseAssignment(TokenNode* current)// To assign a value using the := operator
{
    TokenNode* firstVarToken=current; // First token
    current=current->next; // moving on to the next token
 

    if (current == NULL || strcmp(current->token.type, "Operator") != 0 || strcmp(current->token.value, ":=") != 0) {  // := is not an assignment
        return 1;
    }
 
    current=current->next; //moving on to the next value
   

    if (current == NULL ||
        (strcmp(current->token.type, "Identifier") != 0 && strcmp(current->token.type, "IntConstant") != 0)) { // The value to be assigned must be an identifier or an int.
        printf("Error: Expected value after ':=' operator.\n");
        return 1;
    }
    int isIdentifier=0;
    if(strcmp(current->token.type, "Identifier") == 0)
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=current;
    current=current->next;  //moving on to the next value
 
  
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) {   // end of line check
        printf("Error: Expected ';' after assignment.\n");
        return 1;
    }
    VariableNode* variableCurrent=variableHead; // Head of list
    VariableNode* firstvar=NULL; // Variable to be assigned
    if(isIdentifier==1)
    {
        VariableNode* secondvar=NULL; // Source variable
        while(variableCurrent!=NULL)
        {
            if(strcmp(variableCurrent->val.value_name,firstVarToken->token.value)==0)
            {
                firstvar=variableCurrent; // First variable found
               
            }
            if(strcmp(variableCurrent->val.value_name,secondVarToken->token.value)==0)
            {
                secondvar=variableCurrent; // Second variable found
            }
            variableCurrent=variableCurrent->next; // Browse the list
        }
 
        if(firstvar!=NULL && secondvar!=NULL)
        {
            firstvar->val.value=secondvar->val.value;  // We are making an appointment
        }
        else if(firstvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value); // First variable not found error
            return 1;
        }    
        else if(secondvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",secondVarToken->token.value); // Second variable not found error
            return 1;
        }
        else{
            printf("ERROR:Some variables are not defined!!\n"); // General error message
            return 1;
        }
    }
    else {
 
        while(variableCurrent!=NULL && strcmp(variableCurrent->val.value_name,firstVarToken->token.value)!=0)
        {
            variableCurrent=variableCurrent->next; // We find the first variable
        }
 
        if(variableCurrent->val.value_name!=NULL)
        {
            const char* tempValue=secondVarToken->token.value; // We are taking the numerical value
            char *endptr=NULL;
            long long tempInt=strtoll(tempValue,&endptr,10); // We are converting it to a number.
            if (*endptr != '\0') {
                printf("ERROR: NOT AN INTEGER: %s\n", endptr); 
            } else {
                variableCurrent->val.value=tempInt; // We are assigning a value
            }
        }
        else{
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value); // Variable not defined
            return 1;
        }
    }
    return 0;
}

int parseIncrement(TokenNode* current) // Increment operation using the += operator
{
    TokenNode* firstVarToken=current;   // First variable
    current=current->next; // Moving on to the next token
 
    if (current == NULL || strcmp(current->token.type, "Operator") != 0 || strcmp(current->token.value, "+=") != 0) {     // If not +=, it does not belong to this function.
        return 1;
    }
 
    current=current->next;
 

    if (current == NULL ||     // If the value on the right is not appropriate, we return an error.
        (strcmp(current->token.type, "Identifier") != 0 && strcmp(current->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after '+= operator.\n");
        return 1;
    }
    int isIdentifier=0; // We are looking at the value identifier.
    if(strcmp(current->token.type, "Identifier") == 0)
    {
        isIdentifier=1; // We are marking the identifier
    }
    TokenNode* secondVarToken=current; // 2nd value
    current=current->next; // Moving on to the next token
 
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) {
        printf("Error: Expected ';' after increment.\n"); // We are giving an end-of-line missing message.
        return 1;
    }
 
    VariableNode* variableCurrent=variableHead; // Top of list
    VariableNode* firstvar=NULL; // First variable
    if(isIdentifier==1)
    {
        VariableNode* secondvar=NULL; // Second variable
        while(variableCurrent!=NULL)
        {
            if(strcmp(variableCurrent->val.value_name,firstVarToken->token.value)==0)
            {
                firstvar=variableCurrent; // First variable found
               
            }
            if(strcmp(variableCurrent->val.value_name,secondVarToken->token.value)==0)
            {
                secondvar=variableCurrent; // Second variable found
            }
            variableCurrent=variableCurrent->next; // We are browsing the list
        }
 
        if(firstvar!=NULL && secondvar!=NULL)
        {
            firstvar->val.value=firstvar->val.value+secondvar->val.value; // We are performing an increment operation.
        }
        else if(firstvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value); // No first variable
            return 1;
        }    
        else if(secondvar==NULL)
        {
            printf("ERROR:%s is not defined!!\n",secondVarToken->token.value); // No second variable
            return 1;
        }
    }
    else {
 
        while(variableCurrent!=NULL && strcmp(variableCurrent->val.value_name,firstVarToken->token.value)!=0)
        {
            variableCurrent=variableCurrent->next; // Find the first variable
        }
 
        if(variableCurrent!=NULL)
        {
            const char* tempValue=secondVarToken->token.value; // Numerical value
            char *endptr=NULL;
            long long tempInt=strtoll(tempValue,&endptr,10); // We are converting it to a number.
            if (*endptr != '\0') {
                printf("ERROR: NOT AN INTEGER: %s\n", endptr); // Incorrect number message
            } else {
                variableCurrent->val.value=variableCurrent->val.value+tempInt; // We are printing the total
            } 
           
           
           
        }
        else{
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value); // No variables
            return 1;
        }
    }
    return 0;
}
 
int parseDecrement(TokenNode* current) // Decrement operation using the -= operator
{
    TokenNode* firstVarToken=current;   // Store the token of the first variable
    current=current->next; // Moving on to the next token
    // -= if not, it does not belong to this function
    if (current == NULL || strcmp(current->token.type, "Operator") != 0 || strcmp(current->token.value, "-=") != 0) {
        return 1;
    }
 
    current=current->next;  // Moving on to the next token
 
    // If the value on the right is not appropriate, an error is generated.
    if (current == NULL ||
        (strcmp(current->token.type, "Identifier") != 0 && strcmp(current->token.type, "IntConstant") != 0)) {
        printf("Error: Expected value after '-=' operator.\n");
        return 1;
    }
    int isIdentifier=0; // We are checking whether the value on the right is an identifier.
    if(strcmp(current->token.type, "Identifier") == 0) // If the token type is identifier
    {
        isIdentifier=1;
    }
    TokenNode* secondVarToken=current; // Save the token of the value on the right
    // test çıktısı
    current=current->next; // Moving on to the next token
 
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) { // If no end of line is expected, throw an error
        printf("Error: Expected ';' after decrement.\n");
        return 1;
    }
 
    VariableNode* variableCurrent=variableHead; // Head of the variable list
    VariableNode* firstvar=NULL; // First variable pointer
    if(isIdentifier==1) // If the value on the right is also a variable
    {
        VariableNode* secondvar=NULL; // Second variable pointer
        while(variableCurrent!=NULL)  // We are going through all the variables in the list.
        { 
            if(strcmp(variableCurrent->val.value_name,firstVarToken->token.value)==0)  
            {
                firstvar=variableCurrent; // First variable found
               
            }
            if(strcmp(variableCurrent->val.value_name,secondVarToken->token.value)==0)
            {
                secondvar=variableCurrent; // Second variable found
            }
            variableCurrent=variableCurrent->next; // We are moving forward with the list.
        }
 
        if(firstvar!=NULL && secondvar!=NULL) // If both variables are found
        {
            firstvar->val.value=firstvar->val.value-secondvar->val.value; // Subtract the values from each other
        }
        else if(firstvar==NULL) // If the first variable is not defined
        {
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value); // Error message
            return 1;
        }    
        else if(secondvar==NULL) // If the variable on the right is not defined
        {
            printf("ERROR:%s is not defined!!\n",secondVarToken->token.value); // Error message
            return 1;
        }
    }
    else { // If the value on the right is a constant number
 
        while(variableCurrent!=NULL && strcmp(variableCurrent->val.value_name,firstVarToken->token.value)!=0) // We loop until we find the first variable
        {
            variableCurrent=variableCurrent->next; // We are moving forward with the list
        }
 
        if(variableCurrent!=NULL) // If the variable is found
        {
            const char* tempValue=secondVarToken->token.value; // Take the fixed value
            char *endptr=NULL;
            long long tempInt=strtoll(tempValue,&endptr,10); // Convert string to integer
            if (*endptr != '\0') { // If it is not a valid number
                printf("ERROR: NOT AN INTEGER: %s\n", endptr); // Error message
            } else {
                variableCurrent->val.value=variableCurrent->val.value-tempInt; // Remove from value
            }
           
           
           
        }
        else{ // If the variable cannot be found
            printf("ERROR:%s is not defined!!\n",firstVarToken->token.value); // Error message
            return 1;
        }
    }
    return 0;
}

int executeAssignIncrDecre(TokenNode** current)// Assign values using the := operator
{
    moveCurrentToken(current);
    moveCurrentToken(current); 
    moveCurrentToken(current);
    return 0;
}

int parseWrite(TokenNode* current) {
    current=current->next;
 
    while (current != NULL && strcmp(current->token.type, "EndOfLine") != 0) { // Continue to the end of the line
        if (strcmp(current->token.type, "StringConst") == 0 || // String constant?
            strcmp(current->token.type, "Identifier") == 0 || // Identifier?
            strcmp(current->token.value, "newline") == 0 || // Is it written as a new line?
            strcmp(current->token.type, "IntConstant") == 0) { // Is it a fixed number?
 
            // special control for writing newlines
            if (strcmp(current->token.value, "newline") == 0) { // newline
                printf("\n"); // move to the next line
            }
            else if(strcmp(current->token.type, "Identifier") == 0) // if it is an identifier
            {
                VariableNode* variableCurrent=variableHead; // beginning of variable list
                VariableNode* variable1=NULL; // for the variable being searched for
                while(variableCurrent!=NULL) // loop through the list of variables
                {
                    if(strcmp(variableCurrent->val.value_name,current->token.value)==0)// if there is a matching name
                    {
                        variable1=variableCurrent; // save match
                        break;
                    }
                    variableCurrent=variableCurrent->next; // moving on to the next variable
                   
                }
 
                if(variable1==NULL){ // if the variable is not defined
                    printf("\nERROR: %s not declareted.\n",current->token.value); // error message
                    return 1;
                }
                else if(strcmp(variable1->val.value_name,current->token.value)==0) // if variable is true
                {
                    printf("%d",variableCurrent->val.value); // printing the value
                }
               
            }else if(strcmp(current->token.type, "StringConst") == 0) // string constant
            {
                char* tempValue= current->token.value; // get value
                size_t len = strlen(tempValue); // find its length
                // Allocate memory for the new string (add +1 for the terminating \0)
                char* trimmed = (char*)malloc(len - 1);  // len - 2 characters + \0
                if (trimmed == NULL) return 0;
                // Copy between input[1] and input[len - 2]
                strncpy(trimmed, tempValue + 1, len - 2);
                trimmed[len - 2] = '\0'; // null terminator
                printf("%s", trimmed);
            }else if(strcmp(current->token.type, "IntConstant") == 0) // if it is a fixed number
            {
                printf("%s", current->token.value); // printing the value
            }
            else { // Diğer her şey için
                printf("%s", current->token.value); // printing the value
            }
        } else {
            // Return an error if there is an invalid expression
            printf("\nError: Invalid expression in write: %s (%s)\n", current->token.type, current->token.value);
            return 1;
        }
        if(current->next!=NULL) // if there is a next token
        {
            current=current->next; // Moving on to the next token
            if(strcmp(current->token.value, ";") == 0) // if it is a semicolon
            {
                break;// we are exiting the loop
            }
        }
        if(strcmp(current->token.value, ";") != 0 && strcmp(current->token.value, "and") != 0)  // in between and if not
        {
            printf("\nERROR: EXCEPTED AND."); // error message
            return 1;
        }
        if(current->next!=NULL) 
        {
            current=current->next;
        }
        
    }
 
    // EndOfLine check
    if (current == NULL || strcmp(current->token.type, "EndOfLine") != 0) { // if endofline is missing
        printf("\nError: Missing ';' at the end of write.\n"); // error message
        return 1;
    }
 
    printf("\n"); // move to the next line after each write
   
    return 0;
}

int executeWrite(TokenNode** current)
{
    moveCurrentToken(current); // Moving on to the token after write
 
    while ((*current) != NULL && strcmp((*current)->token.type, "EndOfLine") != 0) { // Move to the end of the line
        moveCurrentToken(current); // moving on to the next token
    }
 
    if ((*current) == NULL || strcmp((*current)->token.type, "EndOfLine") != 0) { // if end of line is missing
        printf("\nError: Missing ';' at the end of write.\n");  // error message
        return 1;
    }  
    return 0;
}



int blockParse(TokenNode** current) // Blok yapısını parçalayan metod
{
    while((*current)!=NULL && strcmp((*current)->token.value, "}")!=0) // If the current token is not NULL and the token value is not ‘}’, the loop continues.
    {
        *current=(*current)->next; // moving on to the next token
        if(strcmp((*current)->token.value, "{")==0)   // If there is an opening bracket, parse the nested block as well.
        {
            
            blockParse(current); // parse the inner block as well
            *current=(*current)->next; // move to the next tokenERROR 
        }
    } 
}

int parseLoop(TokenNode** current) // method that breaks down the repeat loop
{
    moveCurrentToken(current); // move to the next token
    if((*current)==NULL) // if our token is empty
    {
        printf("ERROR.INVALID EXPRESSİON\n"); // error message
        return 1;
    }
    if(strcmp((*current)->token.type,"Identifier")!=0 && strcmp((*current)->token.type,"IntConstant")!=0)   // After the repeat statement, there must be an Identifier or IntConstant.
    {
        printf("ERROR : HAVE TO BE AN IDENTIFIER AFTER REPEAT.\n"); // error message
        return 1;
    }  
    long long size = 0; // number of loop
    VariableNode* var=NULL; // if it is a variable, we put it here
    if (strcmp((*current)->token.type, "Identifier") == 0) {   // If a variable comes after repeat
        VariableNode* currentVariable=variableHead;
        while (currentVariable!=NULL) // Searching the variable list
        {
            if(strcmp(currentVariable->val.value_name,(*current)->token.value)==0)
            {
                var=currentVariable; // matching variable found
            }
            currentVariable=currentVariable->next;
        }
        
        if (var == NULL) { // Error if variable is not defined
            printf("ERROR: Variable not defined: %s\n", (*current)->token.value); // error message
            return 1;
        }
        size = var->val.value; // The value of the variable becomes the number of iterations
    } else if (strcmp((*current)->token.type, "IntConstant") == 0) {
        const char* tempValue = (*current)->token.value; // If it is a fixed number, convert it to long long.
        char *endptr = NULL;
        size = strtoll(tempValue, &endptr, 10);
        if (*endptr != '\0') { // error if number is not valid
            printf("ERROR: NOT AN INTEGER: %s\n", tempValue); // error message
            return 1;
        }
    } else { // whether variable or constant
        printf("ERROR: Expected integer or variable before 'times'.\n"); // error message
        return 1;
    }
    
    moveCurrentToken(current); // moving on to the next token
    if(strcmp((*current)->token.value,"times")!=0) // If the word ‘times’ is not present
    {
        printf("ERROR : HAVE TO BE AN times KEYWORD.\n"); // error message
        return 1;
    }
    moveCurrentToken(current); // moving on to the next token
    int returnValue=0;
    if(strcmp((*current)->token.value, "{")==0) // If it starts with {
    {
        for(int i=size;i>=1;i--) // we are looping up to size
        { 
        
            TokenNode* localCurrent=*current; // geçici current
            moveCurrentToken(&localCurrent); // moving on to the next token
            while(strcmp(localCurrent->token.value, "}")!=0)
            {
                if (strcmp(localCurrent->token.value, "number") == 0) {//keyword ve number
                    returnValue = parseDeclaration(localCurrent);
                    if(returnValue==0)
                    {
                        returnValue=executeDeclaration(&localCurrent);
                    }
                    
                    
                }
                else if (strcmp(localCurrent->token.type, "Identifier") == 0) { //identifier 
                    if(strcmp(localCurrent->next->token.type,"Operator")!=0)
                    {
                        printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTIFIER\n");
                        return 1;
                    }
                
                    if(strcmp(localCurrent->next->token.value,":=")==0) // Assignment operation
                    {
                        returnValue = parseAssignment(localCurrent);
                        if(returnValue==0)
                        {
                            returnValue=executeAssignIncrDecre(&localCurrent);
                        }
                    }
                    else if(strcmp(localCurrent->next->token.value,"+=")==0) // Increment operation
                    {
                        returnValue = parseIncrement(localCurrent);
                        if(returnValue==0)
                        {
                            returnValue=executeAssignIncrDecre(&localCurrent);
                        }
                    }
                    else if(strcmp(localCurrent->next->token.value,"-=")==0) // Decrement operation
                    { 
                        returnValue = parseDecrement(localCurrent);
                        if(returnValue==0)
                        {
                            returnValue=executeAssignIncrDecre(&localCurrent);
                        }  
                    }
                    else{
                        printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTIFIER\n"); // Invalid operator error message
                        return 1;
                    }
                
                }
                else if (strcmp(localCurrent->token.value, "write") == 0) { // If there is a write command
                    returnValue = parseWrite(localCurrent);
                    if(returnValue==0)
                    {
                        returnValue=executeWrite(&localCurrent);
                    }  

                }
                else if (strcmp(localCurrent->token.value, "repeat") == 0) {  // We check if the word ‘repeat’ has appeared.
                    returnValue = parseLoop(&localCurrent);
                }
                else
                {
                    printf("ERROR: WRONG SYNTAX : %s \n",localCurrent->token.value); // Inappropriate expression message
                    return 1;
                }
            
                
                if (returnValue != 0)
                {
                    return 1;
                }
                
                moveCurrentToken(&localCurrent); //We are advancing currenToken by one
            }
            if(var!=NULL) // If there is a loop variable
            {
                var->val.value=var->val.value-1; // We are reducing its value
            }
        }
        blockParse(current); // Parse operation at the end of the block
        if(*current==NULL)
        {
            return 1;
        }
        return 0;

    }
    else // If it is not curly bracketed
    {
        for(int i=size;i>=1;i--)
        {
            TokenNode* localCurrent=*current; // Temporary token
            if(strcmp((*current)->token.value, "repeat")==0) // Recursive repeat
            {
                parseLoop(&localCurrent);
            }
            else if (strcmp((*current)->token.type, "Keyword") == 0 && strcmp((*current)->token.value, "number") == 0) {//keyword ve number
                returnValue = parseDeclaration(*current); // Variable definition
                if(returnValue)
                {
                    return 1;
                }
            }
            else if (strcmp((*current)->token.type, "Identifier") == 0) {
                if(strcmp((*current)->next->token.type,"Operator")!=0)
                {
                    printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTIFIER\n");
                    return 1;
                }
            
                if(strcmp((*current)->next->token.value,":=")==0) // Assignment operation
                {
                    returnValue = parseAssignment(*current);
                    if(returnValue)
                    {
                        return 1;
                    }
                }
                else if(strcmp((*current)->next->token.value,"+=")==0) // Increment operation
                {
                    returnValue = parseIncrement(*current);
                    if(returnValue)
                    {
                        return 1;
                    }
                }
                else if(strcmp((*current)->next->token.value,"-=")==0) // Decrement operation
                {
                    returnValue = parseDecrement(*current);
                    if(returnValue)
                    {
                        return 1;
                    }
                }
                else{
                    printf("b");
                    printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTIFIER\n"); // Invalid operator error message
                    return 1;
                }
            
            }
            else if (strcmp((*current)->token.type, "Keyword") == 0 && strcmp((*current)->token.value, "write") == 0) { // If there is a write command
                returnValue = parseWrite(*current); // Writing process
                
                if(returnValue)
                {
                    return 1;
                }
            }
            else{
                printf("ERROR: WRONG SYNTAX : %s \n",localCurrent->token.value); // Invalid expression error message
                return 1;
            }
            if(var!=NULL)
            {
                var->val.value=var->val.value-1; // We are reducing the value
            }
        }
         // Process based on tokens remaining after loop
        if(strcmp((*current)->token.value, "number")==0) //number
        {
            executeDeclaration(current);
            return 0;
        }
        else if(strcmp((*current)->token.type, "Identifier")==0) //identifier
        {
            executeAssignIncrDecre(current);
            return 0;
        }
        else if(strcmp((*current)->token.value, "write")==0) //write
        {
            executeWrite(current);
            return 0;
        }
        else if(strcmp((*current)->token.value, "repeat")==0) // repeat
        {
            
            while ((*current) != NULL && strcmp((*current)->token.value, ";") != 0) { // If the code block starts with curly brackets, parse it accordingly.
                moveCurrentToken(current);//move to the next token
                if(strcmp((*current)->token.value, "{") == 0) // If the block start has been found
                {
                    blockParse(current); // Separate the block
                    break;
                }

            }    

            return 0;
        }
    }


}
 
int testDeclarations() // Test method that prints all defined variables
{
    VariableNode* while2current = variableHead; // Pointer is moved to the beginning to navigate through the variable list
    while (while2current != NULL) // Return to the end of the list
    {
        printf("Variable Name: %s \n", while2current->val.value_name); // Print variable name
        printf("Variable Value: %d \n", while2current->val.value);     // Print the variable value
        while2current = while2current->next; // Move on to the next variable
    }

    return 0; // Indicate that the function has completed successfully
}

 
int Parser(TokenNode* tokens) { // Main function that parses tokens
    moveCurrentTokenToHead();  // Go to the top of the token list
    int returnValue = 0; // Variable to hold the result of the operation
 
    while (tokenCurrent != NULL) { // Loop until the token list is exhausted
        blockCount=0; // We are resetting the block counter.
        if (strcmp(tokenCurrent->token.value, "number") == 0) {//keyword ve number
            returnValue = parseDeclaration(tokenCurrent); // Parsing the variable definition
            if(returnValue==0) // If the parse is error-free
            {
                returnValue=executeDeclaration(&tokenCurrent); // We are running the variable definition.
            }
        }
        else if (strcmp(tokenCurrent->token.type, "Identifier") == 0) { //identifier
            
            if(strcmp(tokenCurrent->next->token.type,"Operator")!=0) // operator
            {
                
                printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTIFIER\n"); // We are printing an error message.
                return 1;
            }
           
            if(strcmp(tokenCurrent->next->token.value,":=")==0) // The assignment operator is
            {
                returnValue = parseAssignment(tokenCurrent); // Parsing the assignment
                if(returnValue==0)
                {
                    returnValue=executeAssignIncrDecre(&tokenCurrent); // We are executing the assignment
                }
            }
            else if(strcmp(tokenCurrent->next->token.value,"+=")==0) // The increment operator 
            {
                returnValue = parseIncrement(tokenCurrent); // Parsing the increment operation
                if(returnValue==0)
                {
                    returnValue=executeAssignIncrDecre(&tokenCurrent); // We are executing the increase
                }
            }
            else if(strcmp(tokenCurrent->next->token.value,"-=")==0) // The reduction operator
            {
                returnValue = parseDecrement(tokenCurrent);
                if(returnValue==0)
                {
                    returnValue=executeAssignIncrDecre(&tokenCurrent); // We are executing the reduction
                }  
            }
            else{ // If it is not one of the above operators
                printf("ERROR: HAVE TO BE AN OPERATOR AFTER IDENTIFIER\n"); // Print error message// If it is not one of the above operators
                return 1;
            }
           
        }
        else if (strcmp(tokenCurrent->token.value, "write") == 0) { //  "write
            returnValue = parseWrite(tokenCurrent); // Parse the write command
            if(returnValue==0) 
            {
                returnValue=executeWrite(&tokenCurrent); // Execute the write command
            }  

        }
        else if (strcmp(tokenCurrent->token.value, "repeat") == 0) {  // We are checking to see if the word ‘repeat’ has appeared.
            returnValue = parseLoop(&tokenCurrent);
        }
        else
        {
            printf("ERROR: WRONG SYNTAX : %s \n",tokenCurrent->token.value); //we are printing an error message
            return 1;
        }

       
 
        if (returnValue != 0) // If any operation returned an error
        {
            return 1;
        }
        moveCurrentToken(&tokenCurrent); //We are advancing currenToken by one
    }
    return 0;
}