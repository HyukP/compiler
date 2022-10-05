#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define KEYWORDMAX 32
typedef struct Token {
    char* Index;
    char tokenType[100];
    char* tokenStr;
    int line;
} Token;

char operator[] = {':','+','-','/','*','='};
int tokencount = 0;
int identifierNum = 1;
int stringNum = 1;

bool isDelimiter(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || ch == '@' || ch == '#' ||
        ch == '/' || ch == ',' || ch == '>' || ch == ':' || ch == '$' || ch == '^' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' || ch == '!' || ch == '&' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == ';' || ch == '%' || ch == '?')
        return (true);
    return (false);
}

bool isError(char ch){
    if (ch == '(' || ch == ')' || ch == '!' || ch == '@' || ch == '#' || ch == '^' || ch == '$' || ch == '?' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '<' || ch == '>' || ch == '%') return true;

    return false;
}

bool isOper(char ch){
    if (ch == '+'||ch == '-'||ch == '='||ch == ':'||ch == '*'||ch == '/'){
        return true;
    } else return false;
}


bool validIdentifier(char* str)
{
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isDelimiter(str[0]) == true)
        return (false);
    return (true);
}

bool duplicateToken(Token* tokenList, int tokenLength, char *str){
    for(int i=0; i<tokenLength; i++){
        if(strcmp(tokenList[i].tokenType, "ERROR") &&tokenList[i].tokenStr[0] == str[0]) return false;
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
    }
    return true;
}

char* parseString(char* str, int begin, int end){
    char* string = (char*)malloc(sizeof(char) * (end - begin + 2));
    int count = 0, j = 0;
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
    int i, len = strlen(str);
    bool hasDecimal = false;
 
    if (len == 0)
        return (false);
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' && str[i] != '.' ||
            (str[i] == '-' && i > 0))
            return (false);
        if (str[i] == '.')
            hasDecimal = true;
    }
    return (hasDecimal);
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

void parse(Token* tokenList, char* str, int line)
{
    int left = 0, right = 0;
    int len = strlen(str);
    bool checkString = false;
    bool hasDecimal = false;
    int idcount = 0;
    Token token;
    while (right <= len && left <= right) {
        if (str[left]=='\"') {
            checkString = true;
        }
        if (isDelimiter(str[right]) == false || checkString == true) {
            if(right > left && str[left]==str[right] && isDigit(str[left]) == false && isDigit(str[right]) == false) {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[left];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                left++;
                right--;
            }
            if(right > left && isDigit(str[left]) == true && isCharacter(str[right])==true){
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[left];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                left++;
                right--;
            }
            if(right > left && isCharacter(str[left])==true && isDigit(str[right]) == true && isCharacter(str[right+1]) == true){
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[left];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                left++;
                right--;
            }
            if(right > left && (str[right] ==92 || str[left] == 92)){
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[left];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                left++;
                right--;
            }
            right++;
        }
        if (isDelimiter(str[right]) == true && left == right) {
            
            if (isOper(str[right]) == true) {
                switch(str[right]) {
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
           else  if (isSemicolon(str[right]) == true) {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                strcpy(token.tokenStr, ";");
                strcpy(token.Index, "");
                strcpy(token.tokenType, "SEMICOLON");
                tokenList[tokencount++] = token;
            }
            else if (isError(str[right]) == true) {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char errorch[2];
                errorch[0] = str[right];
                strcpy(token.tokenStr,errorch);
                strcpy(token.Index, "");
                strcpy(token.tokenType, "ERROR");
                token.line = line;
                tokenList[tokencount++] = token;
                left++;
            }
            right++;
            left = right;
        }
        else if (isDelimiter(str[right]) == true && left != right  && checkString == false || (right == len && left != right)) {
            char* subStr;
            subStr = parseString(str, left, right - 1);
            if (subStr[left] == '\"' && subStr[strlen(subStr)-2] != '\"'){
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
                    strncpy(token.tokenStr, subStr, strlen(subStr)-2);
                    strcpy(token.tokenType, "ID");
                    strcpy(token.Index, newIndex);
                    tokenList[tokencount++] = token;
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
            else if (str[left]!='\"' && str[right-1]!='\"' && validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false) {
                if(duplicateToken(tokenList, tokencount, subStr)==true) {
                    token.tokenStr = malloc(sizeof(char)*strlen(subStr)-1);
                    token.Index = malloc(sizeof(char)*strlen(subStr)-1);
                    char* newIndex = malloc(sizeof(char)*strlen(subStr)-1);
                    if(isEscapeChar(subStr[0])==true) {
                        itoaSub(identifierNum++, newIndex, 0);
                        strcpy(token.tokenStr, subStr);
                        strcpy(token.tokenType, "ID");
                        strcpy(token.Index,newIndex);
                        tokenList[tokencount++] = token;
                    }
                }
            }
            else if(str[right-1]=='\"' && validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false) {
                token.tokenStr = malloc(sizeof(char)*strlen(str)-1);
                token.Index = malloc(sizeof(char)*strlen(str)-1);
                char* newIndex = malloc(sizeof(char)*strlen(subStr)-1);
                char errorch[2];
                errorch[0] = str[right-1];
                if(isEscapeChar(str[right-1])==true) {
                    itoaSub(identifierNum++, newIndex, 0);
                    strncpy(token.tokenStr, subStr, strlen(subStr)-1);
                    strcpy(token.tokenType, "ID");
                    strcpy(token.Index, newIndex);
                    tokenList[tokencount++] = token;
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
            left = right;
        } else if (str[left]=='\"' && str[right]=='\"' && left != right){
            char* subStr = parseString(str, left, right);
            if(isString(subStr)==true) {
                token.tokenStr = malloc(sizeof(char)*strlen(subStr)-1);
                token.Index = malloc(sizeof(char)*strlen(subStr)-1);
                char* newIndex = malloc(sizeof(char)*strlen(subStr)-1);
                itoaSub(stringNum++, newIndex, 0);
                strcpy(token.tokenStr, subStr);
                strcpy(token.tokenType, "STRING");
                strcpy(token.Index, newIndex);
                tokenList[tokencount++] = token;
                checkString = false;
            } else if (str[left] != '\"' && str[right] == '\"'){
                for(int i=0;i<strlen(str); i++) {
                    if(str[i]=='\"') {
                        printf("%c Error Line!\n", str[i]);
                        str[i] = '\0';
                    }
                }
                printf("%d %d\n", left, right);
                continue;
            }
            right++;
            left = right;
        } 
    }
}

void TokenDump(Token* tokenList){
    for(int i=0; i < tokencount; i++){
        if(strcmp(tokenList[i].tokenType,"ERROR")) {
            printf("<%s, %s>   %s\n",tokenList[i].tokenType, tokenList[i].Index,tokenList[i].tokenStr);
        } else {
            printf("error: line %d %s\n",tokenList[i].line, tokenList[i].tokenStr);
        }
    }
}
void deleteEnter(char *a){
    int i = 0;
    while(*(a+i)!=0) {
        if(*(a+i)=='\n') *(a+i) = 0;
        i++;
    }
}
int main()
{
    char str[1024];
    char * copyStr;
    Token* tokenList = (Token*)malloc(sizeof(Token)*1024);
    int line = 1;
    while(fgets(str,1024,stdin)){
        copyStr = malloc(sizeof(char)*strlen(str)-1);
        strcpy(copyStr, str);
        deleteEnter(copyStr);
        if(copyStr[strlen(copyStr)-1]=='\r')
            copyStr[strlen(copyStr)-1] = '\0';
        parse(tokenList, copyStr, line++);
        free(copyStr);
    }
    TokenDump(tokenList);
    return 0;
}