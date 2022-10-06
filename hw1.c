#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Token {
    char* Index;
    char tokenType[100];
    char* tokenStr;
    int line;
} Token;

typedef struct symbolTable {
    char* Index;
    char tokenType[100];
    char* tokenStr;
} symbolTable;

typedef struct stringTable {
    char* Index;
    char tokenType[100];
    char* tokenStr;
} stringTable;

char operator[] = {':','+','-','/','*','='};
int tokencount = 0;
int identifierNum = 1;
int stringNum = 1;
int symbolTableNum = 0;
int stringTableNum = 0;

void insertSymbolToken(symbolTable* symbol, Token token) {

    symbolTable newSymbol;

    newSymbol.Index = malloc(sizeof(char)*strlen(token.Index)-1);
    newSymbol.tokenStr = malloc(sizeof(char)*strlen(token.tokenStr)-1);
    strcpy(newSymbol.Index, token.Index);
    strcpy(newSymbol.tokenType, token.tokenType);
    strcpy(newSymbol.tokenStr, token.tokenStr);

    symbol[symbolTableNum++] = newSymbol;
}

void insertStringToken(stringTable* string, Token token){
    stringTable newStringToken;
    
    newStringToken.Index = malloc(sizeof(char)*strlen(token.Index)-1);
    newStringToken.tokenStr = malloc(sizeof(char)*strlen(token.tokenStr)-1);
    strcpy(newStringToken.Index, token.Index);
    strcpy(newStringToken.tokenType, token.tokenType);
    strcpy(newStringToken.tokenStr, token.tokenStr);

    string[stringTableNum++] = newStringToken;
}

bool isDelimiter(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || ch == '@' || ch == '#' ||
        ch == '/' || ch == ',' || ch == '>' || ch == ':' || ch == '$' || ch == '^' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' || ch == '!' || ch == '&' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == ';' || ch == '%' || ch == '?')
        return true;
    return false;
}

bool isError(char ch){
    if (ch == '(' || ch == ')' || ch == '!' || ch == '@' || ch == '#' || ch == '^' || ch == '$' || ch == '?' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '<' || ch == '>' || ch == '%') return true;

    return false;
}

bool isOper(char ch){
    if (ch == '+'||ch == '-'||ch == '='||ch == ':'||ch == '*'||ch == '/'){
        return true;
    }
    
    return false;
}


bool isIdentifier(char* str)
{
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isDelimiter(str[0]) == true)
        return false;
    return true;
}

bool duplicateToken(Token* tokenList, int tokenLength, char *str){
    for(int i=0; i<tokenLength; i++){
        if(strcmp(tokenList[i].tokenType, "ERROR") &&tokenList[i].tokenStr[0] == str[0]) return false;
    }
    return true;
}

bool duplicateString(stringTable* string, char* str){
    for(int i=0; i<stringTableNum; i++){
        if(!strcmp(string[i].tokenStr, str)) return false;
    }
    return true;
}

bool isString(char* str){

    if(str[0]=='\"'&&str[strlen(str)-1]=='\"') return true;
        
    return false;
}

bool isSemicolon(char ch) {
    if(ch == ';') {
        return true;
    } else return false;
}


bool isCharacter(char ch){
    if((ch >='a'&& ch<='z') || (ch >='A' && ch <= 'Z')) return true;

    return false;
}

bool isNum(char * str){
    if(strlen(str)==0) return false;
    for(int i=0;i<strlen(str);i++){
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' || (str[i] == '-' && i > 0))
            return (false);
        if(i!=0) {
            if(str[0]=='0' && (str[i]>='0'&&str[i]<='9')) return false;
        }
    }
    return true;
}

char* parseString(char* str, int begin, int end){
    char* string = (char*)malloc(sizeof(char) * (end - begin + 2));
    int count = 0;
    for(int i = begin; i<= end; i++){
        string[i - begin] = str[i];
    } 
    string[end - begin + 1] = '\0';
    return string;
}

bool isEscapeChar(char ch) {
    if(ch == '\r' || ch == '\b' || ch == '\a') return false;

    return true;
}

bool isDigit(char ch){
    if(ch == '0'||ch == '1'||ch == '2' ||
       ch == '3'||ch == '4'||ch == '5' ||
       ch == '6'||ch == '7'||ch == '8' ||
       ch == '9') return true;

    return false;
}

bool isRealNumber(char* str)
{
    bool hasDecimal = false;
 
    if (strlen(str) == 0) return false;

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' && str[i] != '.' || (str[i] == '-' && i > 0))
            return false;

        if (str[i] == '.') hasDecimal = true;
    }
    return hasDecimal;
}

void itoaSub(int num,char *str,int radix){
    int tmp=num;
    int cnt=0;    
    
    while(tmp!=0){
        tmp/=10;
        cnt++;
    }
    
    str[cnt]='\0'; 
    do{ 
        cnt--;              
        str[cnt]=(char)(num%10+48);
        num = num/10;       
    }while(num!=0);        
}

void lexical(Token* tokenList, char* str, int line, symbolTable* symbolTable, stringTable* stringTable)
{
    int begin = 0, end = 0;
    int length = strlen(str);
    bool checkString = false;
    bool hasDecimal = false;
    int idcount = 0;
    Token token;

    while (end <= length && begin <= end) {
        if (str[begin]=='\"') {
            checkString = true;
        }
        if (isDelimiter(str[end]) == false || checkString == true) {
            if(end > begin && str[begin]==str[end] && isDigit(str[begin]) == false && isDigit(str[end]) == false) {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[begin];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                begin++;
                end--;
            }
            if(end > begin && isDigit(str[begin]) == true && isCharacter(str[end])==true){
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[begin];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                begin++;
                end--;
            }
            if(end > begin && isCharacter(str[begin])==true && isDigit(str[end]) == true && isCharacter(str[end+1]) == true){
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[begin];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                begin++;
                end--;
            }
            if(end > begin && (str[end] ==92 || str[begin] == 92)){
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[begin];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                begin++;
                end--;
            }
            end++;
        }
        if (isDelimiter(str[end]) == true && begin == end) {
            
            if (isOper(str[end]) == true) {
                switch(str[end]) {
                    case '+' : {
                        token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                        token.Index = malloc(sizeof(char)*strlen(str)-1);
                        strcpy(token.tokenStr, "+");
                        strcpy(token.Index, "");
                        strcpy(token.tokenType, "PLUS");
                        tokenList[tokencount++] = token;
                        break;
                    }
                    case '/' : {
                        token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                        token.Index = malloc(sizeof(char)*strlen(str)-1);
                        strcpy(token.tokenStr, "/");
                        strcpy(token.Index, "");
                        strcpy(token.tokenType, "DIV");
                        tokenList[tokencount++] = token;
                        break;
                    }
                    case '-' : {
                        token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                        token.Index = malloc(sizeof(char)*strlen(str)-1);
                        strcpy(token.tokenStr, "-");
                        strcpy(token.Index, "");
                        strcpy(token.tokenType, "MINUS");
                        tokenList[tokencount++] = token;
                        break;
                    }
                    case '*' : {
                        token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                        token.Index = malloc(sizeof(char)*strlen(str)-1);
                        strcpy(token.tokenStr, "*");
                        strcpy(token.Index, "");
                        strcpy(token.tokenType, "MUL");
                        tokenList[tokencount++] = token;
                        break;
                    }
                    case '=' : {
                        token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                        token.Index = malloc(sizeof(char)*strlen(str)-1);
                        strcpy(token.tokenStr, "=");
                        strcpy(token.Index, "");
                        strcpy(token.tokenType, "ASSIGN");
                        tokenList[tokencount++] = token;
                        break;
                    }
                    case ':' : {
                        token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                        token.Index = malloc(sizeof(char)*strlen(str)-1);
                        strcpy(token.tokenStr, ":");
                        strcpy(token.Index, "");
                        strcpy(token.tokenType, "COLON");
                        tokenList[tokencount++] = token;
                        break;
                    }
                }
            }
           else if (isSemicolon(str[end]) == true) {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                strcpy(token.tokenStr, ";");
                strcpy(token.Index, "");
                strcpy(token.tokenType, "SEMICOLON");
                tokenList[tokencount++] = token;
            }
            else if (isError(str[end]) == true) {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[end];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                begin++;
            }
            end++;
            begin = end;
        }
        else if (isDelimiter(str[end]) == true && begin != end  && checkString == false || (end == length && begin != end)) {
            char* subStr;
            subStr = parseString(str, begin, end - 1);
            if (subStr[begin] == '\"' && subStr[strlen(subStr)-2] != '\"'){
                Token errorToken;
                errorToken.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                errorToken.Index = malloc(sizeof(char)*strlen(str)-1);
                strcpy(errorToken.tokenStr,subStr);
                strcpy(errorToken.Index, "");
                strcpy(errorToken.tokenType, "ERROR");
                errorToken.line = line;
                tokenList[tokencount++] = errorToken;
            }
            if (subStr[strlen(subStr)-2] == '\"') {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char* newIndex = malloc(sizeof(char)*strlen(subStr)-1);
                char errorch[2];
                errorch[0] = subStr[strlen(subStr)-2];
                if(isEscapeChar(subStr[strlen(subStr)-2])==true) {
                    itoaSub(identifierNum++, newIndex, 0);
                    strncpy(token.tokenStr, subStr, 10);
                    strcpy(token.tokenType, "ID");
                    strcpy(token.Index, newIndex);
                    tokenList[tokencount++] = token;
                    insertSymbolToken(symbolTable, token);
                }
                Token errorToken;
                errorToken.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                errorToken.Index = malloc(sizeof(char)*strlen(str)-1);
                strcpy(errorToken.tokenStr,errorch);
                strcpy(errorToken.Index, "");
                strcpy(errorToken.tokenType, "ERROR");
                errorToken.line = line;
                tokenList[tokencount++] = errorToken;
            }
            else if (isNum(subStr) == true) {
                printf("%s\n",subStr);
                token.tokenStr = malloc(sizeof(char)*strlen(subStr)-1);
                token.Index = malloc(sizeof(char)*strlen(subStr)-1);
                strcpy(token.tokenStr, subStr);
                strcpy(token.tokenType, "INT");
                strcpy(token.Index, subStr);
                tokenList[tokencount++] = token;
            }
            else if (isRealNumber(subStr) == true) {
                token.tokenStr = malloc(sizeof(char)*strlen(subStr)-1);
                token.Index = malloc(sizeof(char)*strlen(subStr)-1);
                strcpy(token.tokenStr, subStr);
                strcpy(token.tokenType, "DOUBLE");
                strcpy(token.Index, subStr);
                tokenList[tokencount++] = token;
            }
            else if (str[begin]!='\"' && str[end-1]!='\"' && isIdentifier(subStr) == true && isDelimiter(str[end - 1]) == false) {
                if(duplicateToken(tokenList, tokencount, subStr)==true) {
                    token.tokenStr = malloc(sizeof(char)*strlen(subStr)-1);
                    token.Index = malloc(sizeof(char)*strlen(subStr)-1);
                    char* newIndex = malloc(sizeof(char)*strlen(subStr)-1);
                    if(isEscapeChar(subStr[0])==true) {
                        itoaSub(identifierNum++, newIndex, 0);
                        strncpy(token.tokenStr, subStr, 10);
                        strcpy(token.tokenType, "ID");
                        strcpy(token.Index,newIndex);
                        tokenList[tokencount++] = token;
                        insertSymbolToken(symbolTable, token);
                    }
                }
            }
            else if(str[end-1]=='\"' && isIdentifier(subStr) == true && isDelimiter(str[end - 1]) == false) {

                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char* newIndex = malloc(sizeof(char)*strlen(subStr)-1);
                char errorch[2];
                errorch[0] = str[end-1];
                if(isEscapeChar(str[end-1])==true) {
                    itoaSub(identifierNum++, newIndex, 0);
                    strncpy(token.tokenStr, subStr, strlen(subStr)-1);
                    strcpy(token.tokenType, "ID");
                    strcpy(token.Index, newIndex);
                    tokenList[tokencount++] = token;
                    insertSymbolToken(symbolTable, token);
                }
                Token errorToken;
                errorToken.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                errorToken.Index = malloc(sizeof(char)*strlen(str)-1);
                strcpy(errorToken.tokenStr,errorch);
                strcpy(errorToken.Index, "");
                strcpy(errorToken.tokenType, "ERROR");
                errorToken.line = line;
                tokenList[tokencount++] = errorToken;
            }
            begin = end;
        } else if (str[begin]=='\"' && str[end]=='\"' && begin != end) {

            char* subStr = parseString(str, begin, end);

            if(isString(subStr)==true) {
                token.tokenStr = malloc(sizeof(char)*strlen(subStr)-1);
                token.Index = malloc(sizeof(char)*strlen(subStr)-1);
                char* newIndex = malloc(sizeof(char)*strlen(subStr)-1);
                    if(duplicateString(stringTable, subStr)){
                    itoaSub(stringNum++, newIndex, 0);
                    strcpy(token.tokenStr, subStr);
                    strcpy(token.tokenType, "STRING");
                    strcpy(token.Index, newIndex);
                    tokenList[tokencount++] = token;
                    insertStringToken(stringTable, token);
                }
                checkString = false;
            } else if (str[begin] != '\"' && str[end] == '\"'){
                for(int i=0;i<strlen(str); i++) {
                    if(str[i]=='\"') {
                        printf("%c Error Line!\n", str[i]);
                        str[i] = '\0';
                    }
                }
                continue;
            }
            end++;
            begin = end;
        } 
    }
}

void TokenDump(Token* tokenList) {
    for(int i=0; i < tokencount; i++){
        if(strcmp(tokenList[i].tokenType,"ERROR")) {
            printf("<%s, %s>   %s\n",tokenList[i].tokenType, tokenList[i].Index,tokenList[i].tokenStr);
        } else {
            printf("error: line %d %s\n",tokenList[i].line, tokenList[i].tokenStr);
        }
    }
}

void deleteEnter(char *ch) {
    int i = 0;
    while(*(ch+i)!=0) {
        if(*(ch+i)=='\n') *(ch+i) = 0;
        i++;
    }
}

void dumpTables(symbolTable* symbol, stringTable* string) {
    printf("---------------------Symbol Table---------------------\n");
    printf("    Index                             Identifier\n");
    printf("------------------------------------------------------\n");
    for(int i = 0; i < symbolTableNum; i++) {
        printf("    %s                                  %s\n",symbol[i].Index, symbol[i].tokenStr);
    }
    printf("\n---------------------String Table---------------------\n");
    printf("    Index                             STRING\n");
    printf("------------------------------------------------------\n");
    for(int i = 0; i < stringTableNum; i++) {
        printf("    %s                                  %s\n",string[i].Index, string[i].tokenStr);
    }
}

int main()
{
    char str[1024];
    char * copyStr;
    int line = 1;
    Token* tokenList = (Token*)malloc(sizeof(Token)*1024);
    symbolTable* symbol = (symbolTable*)malloc(sizeof(symbolTable)*100);
    stringTable* string = (stringTable*)malloc(sizeof(stringTable)*100);

    while(fgets(str,1024,stdin)){
        copyStr = malloc(sizeof(char)*strlen(str)-1);
        strcpy(copyStr, str);
        deleteEnter(copyStr);
        if(copyStr[strlen(copyStr)-1]=='\r')
            copyStr[strlen(copyStr)-1] = '\0';
        lexical(tokenList, copyStr, line++, symbol, string);
        free(copyStr);
    }
    TokenDump(tokenList);
    dumpTables(symbol, string);
    return 0;
}