#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
skipsym = 1 , // Skip / ignore token
identsym , // Identifier
numbersym , // Number
beginsym , // begin
endsym , // end
ifsym , // if
fisym , // fi
thensym , // then
whilesym , // while
dosym , // do
odsym , // od
callsym , // call
constsym , // const
varsym , // var
procsym , // procedure
writesym , // write
readsym , // read
elsesym , // else
plussym , // +
minussym , // -
multsym , // *
slashsym , // /
eqsym , // =
neqsym , // <>
lessym , // <
leqsym , // <=
gtrsym , // >
geqsym , // >=
lparentsym , // (
rparentsym , // )
commasym , // ,
semicolonsym , // ;
periodsym , // .
becomessym // :=
} TokenType;

#define norw 15 /* number of reserved words */
#define imax 5 /* maximum integer value */
#define cmax 11 /* maximum number of chars for idents */
#define strmax 500 /* maximum length of strings */

char *word[norw] = {"begin", "end", "if", "fi", "then", "while", "do", "od", "call", "const", "var", "procedure", "write", "read", "else"};

void Allocate(FILE *plaintext);
void BuildLexeme(); 
void BuildNameTable(); 
void BuildTokenList(); 

char *OriginalText; 

char NameTable[500][cmax+1]; 
int name_counter = 0;

int TokenList[500]; 
int token_counter = 0; 

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Incorrect number of arguments\n");
        exit(1); 
    }

    FILE *plaintext = fopen(argv[1], "r"); 
    Allocate(plaintext); 

    //Build Lexeme Table
    BuildLexeme(); 

    return 0;
}

void Allocate(FILE *plaintext)
{
    char buffer[256]; 
    int total_length = 0; 

    if(plaintext == NULL)
    {
        printf("Unable to open file\n");
        exit(1);
    }

    //make it a valid empty string 
    OriginalText = malloc(1); 
    if(OriginalText == NULL)
    {
        printf("Unable to allocate memory\n");
        exit(1);
    }
    OriginalText[0] = '\0'; 

    while(fgets(buffer, sizeof(buffer), plaintext)!=NULL)
    {
        size_t buffer_len = strlen(buffer); 
        size_t new_buffer_len = buffer_len + total_length + 1; 

        //reallocate memory 
        char *Temp = realloc(OriginalText, new_buffer_len); 
        if(Temp == NULL)
        {
            printf("Cannot read in Original Plaintext\n");
            exit(1);
        }
        OriginalText = Temp; 
        strcpy(OriginalText + total_length, buffer); 
        total_length += buffer_len; 
    }
    printf("Source Program :\n\n"); 
    printf("%s\n\n", OriginalText); 
}

void BuildLexeme()
{
    printf("Lexeme Table :\n\n"); 
    printf("lexeme    token type\n\n");
    int counter = 0; //help build the word, number, 
    int token; //what type of token will be stoed 
    char Wtemp[cmax+1];
    char Ntemp[cmax+1];
    int length = strlen(OriginalText); 

    //go through original text 
    for(int i = 0; OriginalText[i] != '\0'; i++)
    {
        //skip if space tab newline or carriage return 
        if(OriginalText[i] == ' ' || OriginalText[i] == '\t' || OriginalText[i] == '\n' || OriginalText[i] == '\r') continue; 
        if(OriginalText[i] == '/' && OriginalText[i+1] == '*')
        {
            for(int j = i + 2; OriginalText[j] != '\0'; j++)
            {
                if(OriginalText[j] == '*' && OriginalText[j+1] == '/') 
                {
                    i = j+1; 
                    break;  
                }
            }
            continue;
        }
        
        //if its a word
        if(isalpha(OriginalText[i]))
        { 
            counter = 0; 
            char overflow[500]; 
            int isitOver = 0; 
            int overflowCounter = 0; 
            while(isalpha(OriginalText[i]) || isdigit(OriginalText[i]))
            {
                if(counter < cmax)
                {
                    Wtemp[counter] = OriginalText[i];
                }
                else 
                {
                    isitOver = 1; 
                    overflow[overflowCounter] = OriginalText[i]; 
                    overflowCounter++;  
                } 
                counter++;
                i++;
            }
            i--; 
            if(counter < cmax)
            {
                Wtemp[counter] = '\0';
            }
            else Wtemp[cmax] = '\0';
            overflow[overflowCounter] = '\0';
            
            if(isitOver == 1)
            {
                printf("%-4s%s      Identifier too long\n", Wtemp, overflow); //if its too long just print it and store token as 1
                token = skipsym;
                TokenList[token_counter++] = token; 
            }
            else
            {
                int found = 0; //not found yet
                for(int j = 0; j < norw; j++)
                {
                    if(strcmp(Wtemp, word[j])==0)
                    {
                        token = beginsym + j;
                        found = 1;  
                        TokenList[token_counter++] = token; 
                        printf("%-4s      %-4d\n", Wtemp, token);
                        break; 
                    }
                }
                if(found == 0)
                {
                    int index = -1; 
                    //check for duplicates 
                    //gnore words 
                    for(int k = 0; k < name_counter; k++)
                    { 
                        if(strcmp(NameTable[k], Wtemp) == 0)
                        {
                            //if its found store that index of where it was 
                            index = k; 
                            break; 
                        }
                    }
                    //not found before (a b c)
                    if(index == -1)
                    {
                        //copy it into the NameTable
                        strcpy(NameTable[name_counter], Wtemp); 
                        index = name_counter;
                        name_counter++; 
                    }
                    
                    //store it in the token list with its index 
                    token = identsym; 
                    printf("%-4s      %-4d\n", Wtemp, token);
                    TokenList[token_counter++] = token;
                    TokenList[token_counter++] = index;
                }
            } 
        } 

        //if its a number 
        //if its a word
        else if(isdigit(OriginalText[i]))
        { 
            counter = 0;
            char overflow[500]; 
            int isitOver = 0; 
            int overflowCounter = 0; 
            while(isdigit(OriginalText[i]))
            {
                if(counter < imax)
                { 
                    Ntemp[counter] = OriginalText[i];
                }
                else 
                {
                    isitOver = 1; 
                    overflow[overflowCounter] = OriginalText[i];
                    overflowCounter++;  
                } 
                counter++;
                i++; 
            }
            i--; 

            if(counter < imax)
            {
                Ntemp[counter] = '\0';
                int length = strlen(Ntemp);
            }
            else Ntemp[imax] = '\0';
            
            overflow[overflowCounter] = '\0';
            
            if(isitOver == 1)
            {
                printf("%-4s%s      Number too long\n", Ntemp, overflow);//if the number is too long just store the token as 1
                token = skipsym;
                TokenList[token_counter++] = token; 
            }
            else{
                token = numbersym; 
                TokenList[token_counter++] = token;
                TokenList[token_counter++] = atoi(Ntemp);
                printf("%-4s      %-4d\n", Ntemp, token);
            }
        } 
        else if(!isdigit(OriginalText[i]) && !isalpha(OriginalText[i])) //its not a number or a anlpahebtical char
        {
            //geqsym , // >=
            if(OriginalText[i] == '>' && OriginalText[i+1] == '=')
            {
                token = geqsym;
                TokenList[token_counter++] = token;  
                printf("%-4s      %-4d\n", ">=", token);
                i++; //increment its other i to not seperate 
            }  
            //leqsym , // <=
            else if(OriginalText[i] == '<' && OriginalText[i+1] == '=')
            {
                token = leqsym;
                TokenList[token_counter++] = token;  
                printf("%-4s     %-4d\n", "<=", token);
                i++;
            }
            //becomessym // :=
            else if(OriginalText[i] == ':' && OriginalText[i+1] == '=')
            {
                token = becomessym; 
                TokenList[token_counter++] = token; 
                printf("%-4s      %-4d\n", ":=", token); 
                i++;
            } 
            //neqsym , // <>
            else if(OriginalText[i] == '<' && OriginalText[i+1] == '>')
            {
               token = neqsym; 
               TokenList[token_counter++] = token;  
               printf("%-4s      %-4d\n", "<>", token);
               i++;
            } 
            else
            {
                switch(OriginalText[i])
                {
                    case '+':
                        token = plussym;
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case '-':
                        token = minussym;
                        TokenList[token_counter++] = token; 
                        printf("%-4c      %-4d\n", OriginalText[i], token); 
                        break; 
                    case '*': 
                        token = multsym; 
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case '/': 
                        token = slashsym;
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break;  
                    case '=': 
                        token = eqsym;  
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case '<': 
                        token = lessym;
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case '>': 
                        token = gtrsym; 
                        TokenList[token_counter++] = token; 
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case '(': 
                        token = lparentsym;
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case ')': 
                        token = rparentsym;
          
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case ',': 
                        token = commasym;
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case ';': 
                        token = semicolonsym;
                        TokenList[token_counter++] = token;  
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    case '.': 
                        token = periodsym;
                        TokenList[token_counter++] = token; 
                        printf("%-4c      %-4d\n", OriginalText[i], token);
                        break; 
                    default:
                        //invalid symbols).
                        token = skipsym;
                        TokenList[token_counter++] = token;
                        printf("%-4c      Invalid Symbol\n", OriginalText[i]);
                        break; 
                }
            }
        }
    }
    printf("\n\n"); 
    BuildNameTable(); 
}

void BuildNameTable()
{
    printf("Name Table :\n\n"); 
    printf("Index Name\n");

    //filter out words 
    for(int i = 0; i < name_counter; i++)
    { 
        printf("%-4d      %-4s\n", i, NameTable[i]);
    }

    printf("\n\n");
    BuildTokenList(); 
}

void BuildTokenList()
{
    printf("Token List :\n\n");  

    for(int i = 0; i < token_counter; i++)
    {
        printf("%d ", TokenList[i]);
    }
    printf("\n"); 
}
