#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DECIMAL_LENGTH 100
#define MAX_VALUES_LENGTH 20

const char* keywords[] = {"number", "repeat", "times", "write", "and", "newline"};
const char* operators[] = {":=", "+=", "-="};
int keywordsCount = sizeof(keywords) / sizeof(keywords[0]); //Keyword size
int operatorsCount = sizeof(operators) / sizeof(operators[0]); //operator size
int isError=0; // We keep track of whether there are any errors.
TokenNode* tokenHead = NULL; // Root node for token list
TokenNode* tokenCurrent= NULL; // Current node for navigating the token list
Token *tokens = NULL;


void addToken(const char* type, const char* value) { // Adding a new token to the token list
    TokenNode* newNode = (TokenNode*)malloc(sizeof(TokenNode)); // Allocating memory for the new node
    newNode->token.type = strdup(type);     // Copy a string with strdup
    newNode->token.value = strdup(value); // We are copying the token value
    newNode->next = NULL;

    if (tokenHead == NULL) { // If the list is empty
        tokenHead = tokenCurrent = newNode; // The head and current node will be this
    } else {
        tokenCurrent->next = newNode; // Add after the current node
        tokenCurrent = newNode; // Update the current node
    }
}

TokenNode* getTokenList() { // We are spinning the token list
    return tokenHead;
}

int isLetter(char currentCharacter) //We check whether the first character is a letter.
{
    if((currentCharacter >= 'a' && currentCharacter <= 'z') || (currentCharacter >= 'A' && currentCharacter <= 'Z'))
    {
        return 1;
    }
    return 0;
}

int isInteger(char currentCharacter)  //We check whether the first character is a number.
{
    if(currentCharacter >= '0' && currentCharacter <= '9')
    {
        return 1;
    }
    return 0;
}

char readingFileChar(FILE* codeFile) //We are reading from the file
{
    char currentCharacter=fgetc(codeFile); //We are reading a character from the code
    while(currentCharacter==' ') //If the read value is empty, we keep reading until there is no more space.
    {
        currentCharacter=fgetc(codeFile);
    }
    return currentCharacter; //We are rotating the character we read.
}


char* writingToken(char* currentToken,char currentCharacter)  //Since we do not know the length of the tokens, we increase the size by 1 with each read. Initially, the size is 2 bytes.
{
    int length=strlen(currentToken); // Current token length
    currentToken=realloc(currentToken,length+2); // We are reallocating memory for the new character + null terminator.
    currentToken[length]=currentCharacter; // We are adding the last character
    currentToken[length+1]='\0'; // Returning the new token
    return currentToken;
}


int checkingComment(char* currentToken, FILE *codeFile) //If the value we read is *, this function is working.
{
    char ch=readingFileChar(codeFile); //Read until the star appears
    while (!feof(codeFile) && ch!='*')
    {
        ch=readingFileChar(codeFile);
    }

    if(ch!='*' && feof(codeFile)) //If the star did not appear and the file is finished
    {
        printf("Error: Comment not closed \n"); // We are printing an error message.
        isError=1;
    }
    return 0;
}


int checkingString(char* currentToken, FILE *codeFile) //This function runs when we see “"” for constant strings.
{
    char ch=fgetc(codeFile); //We are reading a character from the file.
    while (!feof(codeFile) && ch!='"') //We read until we see “"” here.
    {
        currentToken=writingToken(currentToken,ch); //We store every value we read in the Token.
        ch=fgetc(codeFile);
    }

    if(ch!='"' && feof(codeFile)) //“"”  if not seen and the file is finished
    {
        printf("Error: String Constant not closed \n");
        isError=1;
    }
    else // If the string constant is finished, we output it and write it to the file.
    {
        currentToken=writingToken(currentToken,ch); //We add the last thing we read and write it to the file.
        addToken("StringConst", currentToken);
    }
    return 0;
}

//The first function that each token will enter when it starts
int checkingFirstChar(char* currentToken,char currentCharacter, FILE *codeFile)
{
    //We check numbers and letters (ident., keyw.) separately.
    if (isLetter(currentCharacter)==1 || isInteger(currentCharacter)==1) 
    {
        return 1; //Here, the number 1 we return means that the token continues, and we will continue reading.
    }

    char ch;
    switch (currentCharacter) {
        //Here we return tokens that are single characters. return 0 means that we are no longer performing operations on this token and that the token is complete.
        case ';':
            addToken("EndOfLine", currentToken); 
            return 0;
        case '{':
            addToken("OpenBlock", currentToken);
            return 0;
        case '}':
            addToken("CloseBlock", currentToken);
            return 0;
        //We evaluate comment lines and string constants in functions.
        case '*':
            /*We return the result of the function, which is always 0, because 
            we read and write the file for comment within the comment function, not within main.*/
            return checkingComment(currentToken, codeFile); 
        case '"':
            /*This applies to the comment string.
            The reason we don't read them separately in the main is that they are not subject to any checks until they reach * or "
            and we don't need to perform all the checks for each character unnecessarily.*/
            return checkingString(currentToken,codeFile);
        //Since these characters indicate that we have moved to the next line while reading the file, we return them without reading them directly and saving them to the token.
        case '\r':
        case '\n':
            return 0;
            //Compared to other operators, we check it in two different ways because it can appear at the beginning of integers.
        case '-':
            ch = fgetc(codeFile);
            if (isInteger(ch)) //-After reading the character, if it is a number
            {
                ungetc(ch, codeFile); // We retrieve the last character we read.
                return 1; //And since Token continues, we will continue reading, so we return 1.
            }
            ungetc(ch,codeFile);
            //We are not putting a break here because if the number after - is not the number, we will check if it is = below.
        //Since these are the first characters of operators, if the character following them is =, we treat it as an operator; otherwise, we treat it as unknown.
        case '+':
        case ':':
            ch = fgetc(codeFile);
            if (ch != '=')
            {
                printf("ERROR UNKNOWN CHARACTER(%c) DETECTED!!!\n",currentCharacter);
                isError=1;
                ungetc(ch, codeFile);
                return 0;
            }
            else
            {
                currentToken=writingToken(currentToken,ch);
                addToken("Operator", currentToken); 
                return 0;
            }
        
        default:
        //If it entered here, it is neither a letter nor a number nor a special character, so we print ‘unknown’ to the console.
            printf("ERROR UNKNOWN CHARACTER(%c) DETECTED!!!\n",currentCharacter);
            isError=1;
            return 0; //Technically, the token is finished before it starts, so we return 0 to show that it is finished.
    }

    return 1;
}
//Here we check whether it is a keyword or an identifier.
int checkingKeywordOrIdent(char* currentToken,const char* keywords[],FILE *codeFile,int keywordsCount)
{
    //If our value is above a certain value, we finish with the part that has been read up to this point and print the token.
    if(strlen(currentToken)>=MAX_VALUES_LENGTH) 
    {
        addToken("Identifier", currentToken);
        return 0;//Token has ended, so we are returning 0.
    }
    for(int i=0;i<keywordsCount;i++) //We compare all keywords
    {
        if(strcmp(currentToken,keywords[i])==0)
        {
            char ch=fgetc(codeFile);
            //If it matches a keyword, we look at the next character. If it is not a letter, number, or “_”, we mark it as a keyword.
            if (!(isLetter(ch) || isInteger(ch) || ch=='_')) 
            {
                ungetc(ch, codeFile); //We print the last value we read
                addToken("Keyword", currentToken);
                return 0; //The current token has ended, so we are returning 0.
            } 
            ungetc(ch, codeFile); //We are printing the last value we read.   
        }
    }
    char ch=fgetc(codeFile); //We read the next char for the identifier
    if (!(isLetter(ch) || isInteger(ch) || ch=='_')) //If the character is not a number or _, the identifier is complete.
    {
        ungetc(ch, codeFile);  //We retrieve the last value we read.
        addToken("Identifier", currentToken);
        return 0;  //The token has ended, so we return 0 so that it moves on to the new token.
    }    

    ungetc(ch, codeFile);//We retrieve the last value we read.
    return 1;//If it did not enter any if block, it means that the token has not ended, so we return 1.
}

int checkingIntegerConst(char* currentToken,FILE* codeFile)
{
    if(strlen(currentToken)>=MAX_DECIMAL_LENGTH) //When we reach a certain number of characters, we end the token.
    {
        addToken("IntConstant", currentToken);
        return 0; //We return 0 because it does not continue.
    }
    //If it matches a keyword, we look at the next character. If it is not a letter, number, or “_”, we mark it as a keyword.
    char ch=fgetc(codeFile);
    if (isInteger(ch)==0) //If it is not a number we read, the token is finished and we print it.
    {
        addToken("IntConstant", currentToken);
        ungetc(ch, codeFile);   
        return 0; 
    } 


    ungetc(ch, codeFile); 
    return 1; //It has not entered any if block, so the token continues.
}

int LexicalAnalyzer(const char* sourceFileName)
{

    FILE *codeFile; //The file we will read the code from
    codeFile=fopen(sourceFileName,"rb"); //We open the code file to read it.
    if(codeFile==NULL) 
    {
        printf("File doesnot exist : %s\n", sourceFileName);
        return 1;
    }

    while (!feof(codeFile)) //The loop will continue until the file is finished.
    {
        if(isError!=0)
        {
            return 1;
        }
        int isContinue=1; //(BOOLEAN) Are we currently continuing with a token?
        char* currentToken=malloc(2); /*Since we don't know how long a lexeme will be,
        we are currently allocating 2 bytes of memory for it. */
        char currentCharacter; //The symbol we are currently on while navigating through the code
        currentCharacter=readingFileChar(codeFile);
        currentToken[0]=currentCharacter; //We assign the first value we read to the token's initial value.
        currentToken[1]='\0';//2. We assign the end value to the value.
        /*If the value we read is a letter or “_”, our startLetter value becomes true */
        while(isContinue==1 && !feof(codeFile)) //As long as we are in the same lexeme and the file is not finished
        {
            if(strlen(currentToken)==1) //If there is only one character in the token
            {
                isContinue=checkingFirstChar(currentToken,currentCharacter,codeFile); //We are throwing whether the token continues or not
            }

            if(isContinue==0){continue;} //If isContinue is 0, we exit the while loop; if it is 1, we continue.

            if(isLetter(currentToken[0])) //Here we are interested in lexemes that start with a letter (Identifier, keyword)
            {
                //Check and discard whether the token continues or not
                isContinue=checkingKeywordOrIdent(currentToken,keywords,codeFile,keywordsCount);
            }
            else if(isInteger(currentToken[0]) || currentToken[0]=='-') //First, we check whether the first character is an integer or a negative number.
            {
                //We are checking whether IntegerConstant continues or not.
                isContinue=checkingIntegerConst(currentToken,codeFile);
            }

            if(isContinue==0){continue;} //If the token continues, we return 1; if not, we return 0.
            currentCharacter=readingFileChar(codeFile);// If the token continues, we read 1 character from the file.
            currentToken=writingToken(currentToken,currentCharacter); //We write the character we read to the token.
        }
    }
    
    if(isError!=0)
    {
        return 1;
    }
    fclose(codeFile);
}
