%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    ID,INT,DOUBLE,PLUS,MINUS,MUL,DIV,ASSIGN,LB,RB
} tokenType;

typedef struct SymbolTable {
    char symbol[17];
    double symbolValue;
    bool isVisited;
} SymbolTable;

typedef struct Token {
    tokenType token;
    char tokenValue[11];
}Token;

Token token[100];
SymbolTable symbolTable[100];

int token_num = 0;
int symbol_count = 0;
int errorCheck = 0;

bool check_symbolTable(char* string);
void initialSymbol(char* symbolToken);
int getSymbol(char* string);
void initialToken(tokenType tokenType, char* tokenValue);
void printSymbol();
%}
identifier [a-zA-Z_][a-zA-Z0-9_]*
number [1-9][0-9]*
delimiter [ \t\r]
whitespace {delimiter}+
realNumber (0|[1-9][0-9]*)\.(0|[0-9]*[1-9])
under_bar [_]+
errorType1 [0-9]+[a-zA-Z|0-9|_]+
symbol ^\$symbol$
%%
{whitespace} ;
"+" {initialToken(PLUS,"");}
"-" {initialToken(MINUS,"");}
"/" {initialToken(DIV,"");}
"=" {initialToken(ASSIGN,"");}
"*" {initialToken(MUL,"");}
"(" {initialToken(LB,"");}
")" {initialToken(RB,"");}
{number} {initialToken(INT, yytext);}
{realNumber} {initialToken(DOUBLE,yytext);}
{identifier} {initialSymbol(yytext); initialToken(ID,yytext);}
{errorType1} {errorCheck = -1;}
{symbol} {printSymbol();}
. {errorCheck = -1;}
"\n" {return 0;}
%%
typedef struct Tree {
    int idx;
    struct Tree* leftChild;
    struct Tree* rightChild;
}Tree;

Tree* grammarA();
Tree* grammarAprime(Tree* rightChild);
Tree* grammarE();
Tree* grammarEprime(Tree* leftChild);
Tree* grammarT();
Tree* grammarTprime(Tree* leftChild);
Tree* grammarF();
Tree* grammarFprime();
double interpreter(Tree* parseTree);
int count;
Tree* parseTree;
double value;
int checkDouble;

void printSymbol(){
    printf("name\ttype\tvalue\n");
    for(int i = 0;i<symbol_count;i++){
        printf("%s\t%d\t%lf\n",symbolTable[i].symbol, token[parseTree->leftChild->idx].token, symbolTable[i].symbolValue);
    }
}
int main() {
    for(int i = 0; i< 100; i++) {
        symbolTable[i].symbol[0] = '\0';
        symbolTable[i].symbolValue = 0;
        symbolTable[i].isVisited = false;
    }

    while(!feof(stdin)){
        printf(">");
        errorCheck = 0;
        count = token_num;
        yyin = stdin;
        checkDouble = 0;
        yylex();
        yyrestart(yyin);
        if(errorCheck == -1) {
            printf("lexical error 발생\n");
            break;
        }
        parseTree = (Tree*)malloc(sizeof(Tree));
        parseTree->idx = -1;
        parseTree->leftChild = NULL;
        parseTree->rightChild = NULL;
        while(true) {
            parseTree = grammarA();
            if(parseTree == NULL) {
                break;
            }
            if(token[count].token == 0) {
                break;
            }
        }
        if(token_num == 0) {
            continue;
        } else if(errorCheck == 3){
            printf("syntax error 발생!\n");  
            break;
        } else {
            value = interpreter(parseTree);
        }
        if(errorCheck == 0) {
            if(checkDouble == 0)
                printf("%d\n",(int)value);
            else printf("%lf\n",value);
        }
    }
    fclose(yyin);
    return 0;
}

bool check_symbolTable(char* string){
    for(int i = 0; i<symbol_count;i++) {
        if(!strcmp(symbolTable[i].symbol, string)) {
            return true;
        }
    }
    return false;
}

int getSymbol(char* string) {
    for(int i = 0; i<symbol_count;i++) {
        if(!strcmp(symbolTable[i].symbol, string)) {
            return i;
        }
    }
    return -1;
}

void initialSymbol(char* symbolToken) {
    if(!check_symbolTable(symbolToken)) {
        strcpy(symbolTable[symbol_count].symbol, symbolToken);
        symbol_count++;
    }
}

void initialToken(tokenType tokenType, char* tokenValue){
    token[token_num].token = tokenType;
    strcpy(token[token_num].tokenValue,tokenValue);
    token_num++;
}

Tree* grammarA(){
    if(token[count].token != ID) {
        Tree* first = grammarFprime();
        Tree* second = grammarTprime(first);
        return grammarEprime(second);
    } else {
        Tree* id = (Tree*)malloc(sizeof(Tree));
        id->idx = count;
        id->leftChild = NULL;
        id->rightChild = NULL;
        count++;
        return grammarAprime(id);
    }
}
Tree* grammarAprime(Tree* rightChild){
    if(token[count].token != ASSIGN) {
        Tree* first = grammarTprime(rightChild);
        return grammarEprime(first);
    } else {
        int idx = count;
        Tree* assign = (Tree*)malloc(sizeof(Tree));
        count++;
        Tree* first = grammarA();
        assign->idx = idx;
        assign->leftChild = first;
        assign->rightChild = rightChild;
        return assign;
    }
}

Tree* grammarE() {
    Tree* first = grammarT();
    return grammarEprime(first);
}

Tree* grammarEprime(Tree* leftChild) {
    if(token[count].token == MINUS) {
        int idx = count; count++;
        Tree* first = grammarT();
        Tree* minus = (Tree*)malloc(sizeof(Tree));
        minus->idx = idx;
        minus->leftChild = leftChild;
        minus->rightChild = first;
        return grammarEprime(minus);
    }
    if(token[count].token == PLUS) {
        int idx = count; count++;
        Tree* first = grammarT();
        Tree* plus = (Tree*)malloc(sizeof(Tree));
        plus->idx = idx;
        plus->leftChild = leftChild;
        plus->rightChild = first;
        return grammarEprime(plus);
    }
    return leftChild;
}
Tree* grammarT(){
    Tree* first = grammarF();
    return grammarTprime(first);
}
Tree* grammarTprime(Tree* leftChild) {
    if(token[count].token == DIV) {
        int idx = count; count++;
        Tree* first = grammarF();
        Tree* div = (Tree*)malloc(sizeof(Tree));
        div->idx = idx;
        div->leftChild = leftChild;
        div->rightChild = first;
        return grammarTprime(div); 
    }
    if(token[count].token == MUL) {
        int idx = count; count++;
        Tree* first = grammarF();
        Tree* div = (Tree*)malloc(sizeof(Tree));
        div->idx = idx;
        div->leftChild = leftChild;
        div->rightChild = first;
        return grammarTprime(div); 
    }
    return leftChild;
}

Tree* grammarF() {
    if(token[count].token != ID) {
        return grammarFprime();
    } else {
        Tree* id = (Tree*)malloc(sizeof(Tree));
        id->idx = count;
        id->leftChild = NULL;
        id->rightChild = NULL;
        count++;
        return id;
    }
}
Tree* grammarFprime() {
        if(token[count].token == DOUBLE){
            Tree* realNum = (Tree*)malloc(sizeof(Tree));
            realNum->idx = count;
            realNum->leftChild = NULL;
            realNum->rightChild = NULL;
            count++;
            return realNum;
        }
        else if(token[count].token == INT){
            Tree* id = (Tree*)malloc(sizeof(Tree));
            id->idx = count;
            id->leftChild = NULL;
            id->rightChild = NULL;
            count++;
            return id;
        }
        else if(token[count].token == MINUS){
            Tree* minus = (Tree*)malloc(sizeof(Tree));
            int idx = count;
            Tree* first = grammarF();
            minus->idx = idx;
            minus->leftChild = first;
            minus->rightChild = NULL;
            count++;
            return minus;
        }
        else if(token[count].token == LB){
            Tree* first = grammarE();
            count++;
            if(token[count].token == RB) {
                count++;
                return first;
            } else {
                errorCheck = 3;
                return NULL;
            }
        }
        errorCheck = 3;
        return NULL;
    }


double interpreter(Tree* parseTree) {
    int temp;
    switch(token[parseTree->idx].token) {
        case PLUS : 
            return interpreter(parseTree->leftChild) + interpreter(parseTree->rightChild);
        
        case MINUS : 
            if(parseTree->rightChild == NULL && parseTree->leftChild != NULL){
                return interpreter(parseTree->leftChild)*(-1);
            } else {
                return interpreter(parseTree->leftChild) - interpreter(parseTree->rightChild);
            }
        
        case MUL : 
            return interpreter(parseTree->leftChild) * interpreter(parseTree->rightChild);
        
        case DIV : 
            return interpreter(parseTree->leftChild) / interpreter(parseTree->rightChild);
        
        case INT : 
            temp = atoi(token[parseTree->idx].tokenValue);
            return temp;
        
        case DOUBLE : 
            checkDouble = 1;
            return atof(token[parseTree->idx].tokenValue);
        
        case ASSIGN : 
            temp = getSymbol(token[parseTree->rightChild->idx].tokenValue);
            if(temp != -1) {
                symbolTable[temp].isVisited = true;
                symbolTable[temp].symbolValue = interpreter(parseTree->leftChild);
            }
            return interpreter(parseTree->rightChild);
        
        case ID : 
            temp = getSymbol(token[parseTree->idx].tokenValue);
            if(symbolTable[temp].isVisited == false&&!strcmp(symbolTable[parseTree->idx].symbol, "$symbol")) {
                printf("error : %s는 정의되지 않은 identifier입니다.\n", symbolTable[parseTree->idx].symbol);
                errorCheck = 2;
            }
            return symbolTable[temp].symbolValue;
        case LB : break;
        case RB : break;
        default : break;
    }
    return 0;
}