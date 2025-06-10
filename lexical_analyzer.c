#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DECIMAL_LENGTH 100
#define MAX_VALUES_LENGTH 20

const char* keywords[] = {"number", "repeat", "times", "write", "and", "newline"};
const char* operators[] = {":=", "+=", "-="};

int keywordsCount = sizeof(keywords) / sizeof(keywords[0]); //Keywords boyutu
int operatorsCount = sizeof(operators) / sizeof(operators[0]); //operators boyutu

int isLetter(char currentCharacter) //İlk karakterin harf mi olduğunu check ediyoruz
{
    if((currentCharacter >= 'a' && currentCharacter <= 'z') || (currentCharacter >= 'A' && currentCharacter <= 'Z'))
    {
        return 1;
    }
    return 0;
}

int isInteger(char currentCharacter)  //İlk karakterin sayı mı olduğunu check ediyoruz
{
    if(currentCharacter >= '0' && currentCharacter <= '9')
    {
        return 1;
    }
    return 0;
}

char readingFileChar(FILE* codeFile) //Dosyadan okuma yapıyoruz
{
    char currentCharacter=fgetc(codeFile); //Kodtan bir char okuyoruz
    while(currentCharacter==' ') //Okunan değerin boşluk olma durumunda boşluk gelmeyene kadar okuma yapıyoz
    {
        currentCharacter=fgetc(codeFile);
    }
    return currentCharacter; //Okuduğumz karakteri döndürüyoruz
}

//Tokenların boyunu bilmediğimiz için her okumada boyutu 1 arttırıyoruz. İlk başta 2 byte boyut
char* writingToken(char* currentToken,char currentCharacter) 
{
    int length=strlen(currentToken);
    currentToken=realloc(currentToken,length+2);
    currentToken[length]=currentCharacter;
    currentToken[length+1]='\0';
    return currentToken;
}

//Okuduğumuz değer * ise bu fonksiyon çalşıyor
int checkingComment(char* currentToken, FILE *codeFile, FILE *lexicalFile)
{
    char ch=readingFileChar(codeFile); //Yıldız gelene kadar okuma
    while (!feof(codeFile) && ch!='*')
    {
        ch=readingFileChar(codeFile);
    }

    if(ch!='*' && feof(codeFile)) //Yıldız gelmediyse ve dosya bittiyse
    {
        printf("Error: Comment not closed \n"); // Hata mesajı yazdır
    }
    return 0;
}

//Constant stringler için '"' gördüğümüzde bu fonksiyon çalışıyor
int checkingString(char* currentToken, FILE *codeFile, FILE *lexicalFile)
{
    char ch=fgetc(codeFile); //Dosyadan bir char okuyoruz
    while (!feof(codeFile) && ch!='"') //Burada '"' görene kadar okuma yapıyoruz
    {
        currentToken=writingToken(currentToken,ch); //Her okuduğumuz değeri Token'a kaydediyoruz
        ch=fgetc(codeFile);
    }

    if(ch!='"' && feof(codeFile)) //'"'  görülmediyse ve dosya bittiyse
    {
        printf("Error: String Constant not closed \n");
    }
    else // string constant bittiyse çıktı veriyoruz dosyaya yazıyoruz
    {
        currentToken=writingToken(currentToken,ch); //En son okuduğumuz '"' ekleyip dosyaya yazdırıyoruz
        fprintf(lexicalFile, "StringConst(%s)\n",currentToken);
    }
    return 0;
}

//Her token'a yeni başladığında ilk olarak gireceği fonksiyon
int checkingFirstChar(char* currentToken,char currentCharacter, FILE *codeFile, FILE *lexicalFile)
{
    //sayıları ve harfle başlayanları(ident.,keyw.) ayrı olarak check ediyoruz.
    if (isLetter(currentCharacter)==1 || isInteger(currentCharacter)==1) 
    {
        return 1; //Burada return ettiğimiz 1 sayısı token'ın devam ettiği anlamına geliyor ve okuma yapmaya devam edeceğiz demek 
    }

    char ch;
    switch (currentCharacter) {
        //Burada tek char'lık olan tokenları döndürüyoruz. return 0 demek artık bu token'da işlem yapmıyoruz ve token tamamlandı demek
        case ';':
            fprintf(lexicalFile, "EndOfLine\n"); 
            return 0;
        case '{':
            fprintf(lexicalFile, "OpenBlock\n"); 
            return 0;
        case '}':
            fprintf(lexicalFile, "CloseBlock\n"); 
            return 0;
        //Yorum satırı ve string constant için fonksiyonlarda değerlendirme yapıyoruz
        case '*':
            /*Fonksiyonun sonucunu return ediyoruz her türlü 0 return etmiş oluyo çünkü 
            comment için dosya okuyup yazdırmayı main içinde değil comment fonksiyonu içinde tamamlıyoruz.*/
            return checkingComment(currentToken, codeFile, lexicalFile); 
        case '"':
            /*Comment için olan durum string içinde geçerli.
            Diğerlerinden ayrı olarak onları mainin içinde okumamamızın sebebi * veya " gelene kadar hiçbir kontrole tabi olmamaları
            ve gereksiz olarak tekrar tekrar her char için bütün kontrolleri yapmamıza gerek yok.*/
            return checkingString(currentToken,codeFile,lexicalFile);
        //Dosyada okuma yaparken bir alt satıra geçtiğimizi gösterdiği için bu karakterleri direkt okuyup token'a kaydetmeden return ediyoruz
        case '\r':
        case '\n':
            return 0;
        //Diğer operatorlere nazaran - integerların başına gelebilidiği için onu iki farklı şekilde check ediyoruz
        case '-':
            ch = fgetc(codeFile);
            if (isInteger(ch)) //-'den sonra okuduğumuz karakter eğer sayıysa
            {
                ungetc(ch, codeFile); // Son okuduğumuz karakteri geri alıyoruz
                return 1; //Ve Token devam ettiği içim okumaya devam edicez o yüzden 1 return ediyoruz 
            }
            ungetc(ch,codeFile);
            //Buraya break koymuyoruz çünkü eğer -'den sonraki sayı değilse aşağıda = mi diye kontrol edicez
        //Bunlar operatorlerin ilk işareti olduğu için eğer onlardan sonraki karakter = ise operatordür değilse unknown olarak alıyoruz
        case '+':
        case ':':
            ch = fgetc(codeFile);
            if (ch != '=')
            {
                printf("ERROR UNKNOWN CHARACTER(%c) DETECTED!!!\n",currentCharacter);
                ungetc(ch, codeFile);
                return 0;
            }
            else
            {
                currentToken=writingToken(currentToken,ch);
                fprintf(lexicalFile,"Operator(%s)\n",currentToken);  
                return 0;
            }
        
        default:
        //Buraya girdiyse ne harf ne sayı ne de özel karakterlerden ve o yüzden konsola unknown print ediyoruz
            printf("ERROR UNKNOWN CHARACTER(%c) DETECTED!!!\n",currentCharacter);
            return 0; //Teknik olarak token başlamadan bitmiş oluyo o yüzden bittiğini göstermek için 0 return ediyoruz
    }

    return 1;
}
//Burada Keyword mü identifier mı onu check ediyoruz
int checkingKeywordOrIdent(char* currentToken,const char* keywords[],FILE *lexicalFile,FILE *codeFile,int keywordsCount)
{
    //Value'miz belli bir değerin üstündeyse eğer direkt olarak bu zaman kadar okunmuş olan kısmıyla bitirip token'ı yazdırıyoruz
    if(strlen(currentToken)>=MAX_VALUES_LENGTH) 
    {
        fprintf(lexicalFile,"Identifier(%s)\n",currentToken); 
        return 0;//Token bitti o yüzden 0 döndürüyoruz
    }
    for(int i=0;i<keywordsCount;i++) //Bütün keywordler ile karşılaştırıyoruz
    {
        if(strcmp(currentToken,keywords[i])==0)
        {
            char ch=fgetc(codeFile);
            //Eğer bir keyword ile eşleştiyse bir sonraki char'a bakıyoruz eğer harf,sayı ve '_' değilse keyword olarak işaretliyoruz
            if (!(isLetter(ch) || isInteger(ch) || ch=='_')) 
            {
                fprintf(lexicalFile,"Keyword(%s)\n",currentToken);
                ungetc(ch, codeFile); //Son okuduğumz değeri geri yazdırıyoruz  
                return 0; //Güncel token bitti o yüzden 0 return ediyoruz
            } 
            ungetc(ch, codeFile); //Son okuduğumuz değeri geri yazdırıyoruz    
        }
    }
    char ch=fgetc(codeFile); //Identifier için bir sonraki char'ı okuyoruz
    if (!(isLetter(ch) || isInteger(ch) || ch=='_')) //Eğer harf sayı veya _ değilse identifier bitmiştir 
    {
        fprintf(lexicalFile,"Identifier(%s)\n",currentToken); //Token'ı yazdırıyoruz
        ungetc(ch, codeFile);  //Son okduğumuz değeri geri alıyoruz 
        return 0;  //Token bitti o yüzden 0 return ediyoruz ki yeni token'a geçsin
    }    

    ungetc(ch, codeFile);//Son okuduğumuz değeri geri alıyoruz    
    return 1;//Eğer hiçbir if bloğuna girmediyse demekki token bitmemiş o yüzden 1 return ediyoruz
}

int checkingIntegerConst(char* currentToken, FILE* lexicalFile,FILE* codeFile)
{
    if(strlen(currentToken)>=MAX_DECIMAL_LENGTH) //Bell bir sayıda karaktere ulaşunca token'ı bitiriyoruz
    {
        fprintf(lexicalFile,"IntConstant(%s)\n",currentToken);  
        return 0; //Devam etmediği için 0 return ediyoruz
    }
    //Eğer bir keyword ile eşleştiyse bir sonraki char'a bakıyoruz eğer harf,sayı ve '_' değilse keyword olarak işaretliyoruz
    char ch=fgetc(codeFile);
    if (isInteger(ch)==0) //Okuduğumuz bir sayı değilse token bitmiş oluyor ve yazdırıyoruz
    {
        fprintf(lexicalFile,"IntConstant(%s)\n",currentToken);
        ungetc(ch, codeFile);   
        return 0; 
    } 


    ungetc(ch, codeFile); 
    return 1; //Hiçbir if bloğuna girmemiş o yüzden token devam ediyor
}


int main() {
    FILE *codeFile; //Kodu okuyacağımız dosya
    codeFile=fopen("myscript.plus","rb"); //Kod dosyasını okumak için açıyoruz
    if(codeFile==NULL) 
    {
        printf("File could not be found.");
        return 1;
    }

    FILE *lexicalFile; //Lexeme'lerin yazılacağı dosya    
    lexicalFile=fopen("myscript.lx","wb"); //Lexeme dosyasına yazmak için açıyoruz

    while (!feof(codeFile)) //Dosya bitene kadar döngü dönecek
    {
        int isContinue=1; //(BOOLEAN) Anlık olarak bir token'a devam ediyor muyuz
        char* currentToken=malloc(2); /*Bir lexeme'in uzunluğu ne kadar olucağını bilemediğimiz
        için şuanlık bellekte 2 byte'lık yer ayırıyoruz */
        char currentCharacter; //Kod üzerinde dolaşırken anlık olarak olduğumuz sembol
        currentCharacter=readingFileChar(codeFile);
        currentToken[0]=currentCharacter; //Tokenın ilk değerine ilk okuduğumuz değeri atıyoruz
        currentToken[1]='\0';//2. değerine bitiş değerini atıyoruz
        /*Eğer okuduğumuz değer harf veya '_' ise startLetter değerimiz true oluyor */
        while(isContinue==1 && !feof(codeFile)) //Aynı lexeme'de olduğumuz ve dosya bitmediği sürece
        {
            if(strlen(currentToken)==1) //Token'ın içinde tek char varsa
            {
                isContinue=checkingFirstChar(currentToken,currentCharacter,codeFile,lexicalFile); //Token'ın devam edip etmediğini atıyoruz
            }

            if(isContinue==0){continue;} //Eğer isContinue 0'sa while'dan çıkıyoruz 1'se devam ediyoruz

            if(isLetter(currentToken[0])) //Burada harf ile başlayan lexeme'ler ile ilgileniyoruz(Identifier,keyword)
            {
                //Check edip token'ın devam edip etmediğini atıyoruz
                isContinue=checkingKeywordOrIdent(currentToken,keywords,lexicalFile,codeFile,keywordsCount);
            }
            else if(isInteger(currentToken[0]) || currentToken[0]=='-') //İlk char integer mı ya da - mi diye bakıyoruz
            {
                //IntegerConstant'ın devam edip etmediğini kontrol ediyoruz
                isContinue=checkingIntegerConst(currentToken,lexicalFile,codeFile);
            }

            if(isContinue==0){continue;} //Eğer Token devam ediyosa 1 etmiyosa 0 döndürüyoruz
            currentCharacter=readingFileChar(codeFile);// Token devam ediyosa 1 karakter okuyoruz dosyadan
            currentToken=writingToken(currentToken,currentCharacter); //Okuduğumuz karakteri token'a yazdırıyoruz
        }
    }
    fclose(lexicalFile);
    fclose(codeFile);
    return 0;
}