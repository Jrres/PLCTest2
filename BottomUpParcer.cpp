#include <iostream>
#include <string>
#include <ios>
#include <iomanip>
#include <cstring>
#include <vector>

using namespace std;

#define FAILED 0
#define SUCCESS 1

//token codes
#define LETTER 2
#define DIGIT 3
#define UNKNOWN 99

/* Token codes */
#define Float_Lit 9
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define MOD_OP 25
#define LEFT_PAREN 26
#define RIGHT_PAREN 27
#define EQUALS_OP 28
#define LT_OP 29
#define LTE_OP 30
#define GT_OP 31
#define GTE_OP 32
#define AND_OP 33
#define OR_OP 34
#define NOT_EQUALS_OP 35

//keywords 
#define IF_CODE 50
#define WHILE_CODE 51
#define BLOCK_CODE 52
#define ELSE_CODE 53
#define DATATYPE 54

class TOKEN{

    public:
    int charClass;
    char lexeme[100];
    vector<char> tokenarray;
    char nextChar;
    int lexLen;
    int token;
    int nextToken;
    char lastToken;
    const char *tokenNames[100];
    FILE *in_fp;
    string pattern;
    TOKEN(){
        setTokens();
/* Open the input data file and process its contents */
        if ((in_fp = fopen("TestDoc.in", "r")) == NULL){
            printf("ERROR - cannot open front.in \n");
        }
        else {
            getChar();
        }
    }
    void setTokens(){
    tokenNames[LETTER]="LETTER";
    tokenNames[DIGIT ] ="DIGIT";
    tokenNames[UNKNOWN] = "UNKNOWN";
    tokenNames[Float_Lit ] = "FLOATING_POINT_LIT";
    tokenNames[INT_LIT ] = "INT_LIT";
    tokenNames[IDENT] = "IDENT";
    tokenNames[ASSIGN_OP ] = "ASSIGN_OP";
    tokenNames[ADD_OP ] = "ADD_OP";
    tokenNames[SUB_OP ] = "SUB_OP";
    tokenNames[MULT_OP] = "MULT_OP";
    tokenNames[DIV_OP ] = "DIV_OP";
    tokenNames[MOD_OP ] = "MOD_OP";
    tokenNames[LEFT_PAREN ] ="LEFT_PAREN";
    tokenNames[RIGHT_PAREN] ="RIGHT_PAREN";
    tokenNames[EQUALS_OP] ="EQUALS_OP";
    tokenNames[LT_OP ] = "LESS_THAN_OP";
    tokenNames[LTE_OP ]= "LESS_THAN_EQUAL_TO_OP";
    tokenNames[GT_OP]  = "GREATER_THAN_OP";
    tokenNames[GTE_OP] = "GREATER_THAN_EQUAL_TO_OP";
    tokenNames[AND_OP ] = "AND_OP";
    tokenNames[OR_OP] = "OR_OP"; 
    tokenNames[IF_CODE] = "IF_CODE"; 
    tokenNames[WHILE_CODE] = "WHILE_CODE"; 
    tokenNames[BLOCK_CODE] = "BLOCK"; 
    tokenNames[DATATYPE] = "DATATYPE"; 
    tokenNames[ELSE_CODE]="ELSE_CODE";
    }
    void getChar(void) {
	 if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
		 	charClass = LETTER;
		 else if (isdigit(nextChar))
		 	charClass = DIGIT;
		 else
		 	charClass = UNKNOWN;
	 } else
	 	charClass = EOF;
        if(isalpha(nextChar))
        pattern += nextChar;
    }
    bool Type(string str){
        return str == "int" || str == "float" || str =="bool" || str == "double" || str =="char";
    }
    bool isKeyWord(string str){
        if(Type(str)){
            nextToken = DATATYPE;
        }
        return str == "if" || str == "while" 
        || str == "else" || Type(str);
    }
    bool isIf(string str){
        return str == "if";
    }
    bool isWhile(string str){
        return str == "while";
    }
    bool isElse(string str){
        return str == "else";
    }
    int lex(void) {
        if(pattern.size() == 0 )
        nextToken=0;
        lexLen = 0;
        getNonBlank();
        
        bool isfloat = false;
        if(isKeyWord(pattern)){
            if(isIf(pattern)){
                nextToken=IF_CODE;
            }
            else if(isWhile(pattern)){
                nextToken=WHILE_CODE;
            }
            else if(isElse(pattern)){
                nextToken = ELSE_CODE;
            }
        }
        else{
            
            switch (charClass) {
                /* Identifiers */
                
                case LETTER:
                    addChar();
                    getChar();
                    
                    while (charClass == LETTER || charClass == DIGIT) {
                        if(isKeyWord(pattern)){
                            if(isIf(pattern)){
                                nextToken=IF_CODE;
                            }
                            else if(isWhile(pattern)){
                                nextToken=WHILE_CODE;
                            }
                            else if(isElse(pattern)){
                                nextToken = ELSE_CODE;
                            }
                        }

                        addChar();
                        getChar();
                    }
                    if(nextToken== IF_CODE || nextToken == WHILE_CODE || nextToken==ELSE_CODE || nextToken == DATATYPE)
                    break;
                    nextToken = IDENT;
                 
                    break;
                /* Integer && floating point literals */
                case DIGIT:
                    addChar();
                    getChar();
            
                    while (charClass == DIGIT) {
                    
                        addChar();
                        getChar();
                    }
                    if (nextChar == '.'){
                        addChar();
                        getChar();
                        isfloat = true;
                    } 
                    while(charClass == DIGIT){
                        addChar();
                        getChar();
                    }
                    if(!isfloat)
                        nextToken = INT_LIT;
                    else
                        nextToken=Float_Lit;
                    break;
                /* Parentheses and operators */
                case UNKNOWN:
                    lookup(nextChar);
                    break;
                    /* EOF */
                case EOF:
                    nextToken = EOF;
                    lexeme[0] = 'E';
                    lexeme[1] = 'O';
                    lexeme[2] = 'F';
                    lexeme[3] = '\0';
                    break;
            } /* End of switch */
        }
        return nextToken;
    } /* End of function lex */  
    int lookup(char ch) {
	 switch (ch) {
		 case '(':
			 addChar();
			 getChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 getChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 getChar();
			 nextToken = ADD_OP;
			 break;
		 case '-':
			 addChar();
			 getChar();
			 nextToken = SUB_OP;
			 break;
		 case '*':
			 addChar();
			 getChar();
			 nextToken = MULT_OP;
			 break;
		 case '/':
			 addChar();
			 getChar();
			 nextToken = DIV_OP;
			 break;
		 case '%':
		 	addChar();
			getChar();
			nextToken = MOD_OP;
			break;
		 case '=':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken= ASSIGN_OP;
			if(nextChar=='='){
			addChar();
			nextToken = EQUALS_OP;
			getChar();
			}
			break;
		 case '<':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken=LT_OP;
			if(nextChar=='='){
			addChar();
			nextToken=LTE_OP;
			getChar();
			}
		 	break;
		 case '>':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken = GT_OP;
			if(nextChar=='='){
			addChar();
			nextToken=GTE_OP;
			getChar();
			}
			
		 	break;
		 case '&':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken=AND_OP;
			if(nextChar=='&'){
			addChar();
			nextToken=AND_OP;
			getChar();
			}
			
		 	break;
		 case '|':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken=OR_OP;
			if(nextChar=='|'){
			addChar();
			nextToken=OR_OP;
			getChar();
			}
		 	break;
        case '{':
            addChar();
            nextToken = BLOCK_CODE;
            break;
		 default:
			 addChar();
			 getChar();
			 nextToken = EOF;
			 break;
	 }
	 return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
        tokenarray.push_back(nextChar);
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
} 
};  

class BOOL_EXPR{
    /*
    15 points |<BOOL_EXPR> --> <BTERM> {(`>`|`<`|`>=`|`<=`) <BTERM>}

    <BTERM> --> <BAND> {(`==`|`!=`) <BAND>}
    <BAND> --> <BOR> {`&&` <BOR>}
    <BOR> --> <EXPR> {`&&` <EXPR>}
    */
    public:
    char ptr;
    TOKEN v1;
    BOOL_EXPR(TOKEN v){
        v1 = v;
        ptr = v1.nextChar;
        cout <<" ------Enter the bool expr---------: \n";
    }
    BOOL_EXPR(){
        ptr = v1.nextChar;
        cout << "Parser for logical expressions\n";
        cout << "Step" << setw(25) << "" << "Input\n" ;
        cout << "------------------------------------\n";
        if(BEXP() && v1.nextChar == EOF){
            cout << "this boolean expression is successfully parsed\n";
        }
        else 
            cout << "parsing error\n";
    }
    bool isVALID(){
        if(BEXP() && v1.nextChar == ')'){
                return SUCCESS;
        }
        else 
            return FAILED;
    }
    bool isid(){
        char curr = ptr;
        if(!isalpha(curr))
        return 0;
        while(isdigit(curr) || isalpha(curr)){
            v1.getChar();
            ptr = v1.nextChar;
        }
        return true;
    }
    void removeWhiteSpace(){
        while(ptr==' '){
            ptr++;
        }
        return;
    }
    bool BINQ(){
        v1.getNonBlank();   
        if(ptr == '<' || ptr == '>'){
            if(ptr == '<'){
                v1.lex();
                ptr = v1.nextChar;
                if(v1.nextToken == LTE_OP){
                    cout<<"BEXP -> <= BTERM BEXP"<<setw(8)<<""<<"<="<< "\n";
                }
                else
                cout<<"BEXP -> < BTERM BEXP"<<setw(9)<<""<<ptr<< "\n";
         
            }
            else{
                v1.lex();

                ptr = v1.nextChar;

                if(v1.nextToken == GTE_OP){
                    cout<<"BEXP -> >= BTERM BEXP"<<setw(8)<<""<<">="<< "\n";

                }
                else
                cout<<"BEXP -> > BTERM BEXP"<<setw(8)<<""<<ptr<< "\n"; 
               
            }
            if(BTERM()){
                v1.getNonBlank();
                if(BINQ()){
                    return SUCCESS;
                }
                else{ 
                    return FAILED;
                }
            }
            else{
                return FAILED;
            }
        }
        else{
            cout<<"BEXP -> $"<< std::setw(20) << ""<< ptr << "\n";
            return SUCCESS;
        }
    }
     bool BEXP(){
        v1.getNonBlank();
        cout <<"BEXP -> BTERM BEXP'" << setw(10) << "" <<ptr<< "\n";
        if(BTERM()){
            if(BINQ()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else return FAILED;
    }
    bool BEQ(){
        v1.getNonBlank();
        if(ptr == '='){
            v1.lex();

            ptr = v1.nextChar;

            if(v1.nextToken==EQUALS_OP){
                cout << "BAND -> == BAND BTERM" <<setw(8)<<""<< "==" <<"\n";

                if(BAND()){
                    if(BEQ()){
                        return SUCCESS;
                    } 
                    else{
                        return FAILED;
                    }
                    return SUCCESS;
                }
                else return FAILED;
            }
            else{
                return FAILED;
            }
        }
        else if(ptr =='!'){

            v1.lex();

            ptr = v1.nextChar;
            if(v1.nextToken==NOT_EQUALS_OP){
                cout << "BAND -> != BAND BTERM" <<setw(8)<<""<< "!=" <<"\n";
                if(BOR()){
                    v1.getNonBlank();
                    if(BEQ()){
                        return SUCCESS;
                    }
                    else {
                    return FAILED;
                    }                
                }
                else{
                    return FAILED;
                }
            }
            else{
                return FAILED;
            }
        }
        else{
            cout << "BAND -> $" <<setw(20)<<""<< ptr <<"\n";
            return SUCCESS;
        }
    }
    bool BTERM(){
        v1.getNonBlank();
        cout <<"BTERM -> BAND BTERM'" << setw(9) << "" <<ptr<< "\n";
        if(BAND()){
            if(BEQ()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else{
            return FAILED;
        }
    }
    bool BINC(){
        v1.getNonBlank();
        if(ptr == '&'){
   
            v1.lex();

            ptr = v1.nextChar;
            if(v1.nextToken==AND_OP){
                cout << "BOR -> && BOR BAND "<< setw(10)<<""<<"&&"<<"\n";

                if(BOR()){
                    v1.getNonBlank();
                    if(BINC()){
                        return SUCCESS;
                    }
                    else{
                        return FAILED;
                    }
                }
                else{
                    return FAILED;
                }
            }else 
            return FAILED;
        }
        else{
            cout << "BOR -> $"<< setw(21)<<""<<ptr<<"\n";
            return SUCCESS;
        }
    }
    bool BAND(){
        v1.getNonBlank();
        cout << "BAND -> BOR BAND'" << setw(12) << ""<<ptr << "\n";
        if(BOR()){
            if(BINC()){
                return SUCCESS;
            }
            else
                return FAILED;
        }
        else
            return FAILED;
    }
    bool BOR(){
        v1.getNonBlank();
        if(ptr == '&'){
            if(BINC()){
                if(BEXP()){
                    return SUCCESS;
                }
                else{
                    return FAILED;
                }
            }
            else 
            {
                return FAILED;
            }
        }
        else if(isdigit(ptr) || isid()){
            cout << "BOR -> val"<< setw(19)<<""<<ptr<<"\n";

            v1.lex();
            ptr= v1.nextChar;
            return SUCCESS;
        }
        else{
            return FAILED;
        }
    }
};
class EXPR{
    /*
    Recursive descent parser 
    E –> T E’ 
    E’ –> + T E’ | e 
    T –> F T’ 
    T’ –> * F T’ | e 
    F –> ( E ) | id
    */
    public:
        char ptr;
        TOKEN v1;
        EXPR(){
           //start Symbol
            cout << "Parser for arithmetic expressions\n";
            cout << "Step" << setw(11) << "" << "Input\n" ;
            cout << "-------------------------\n";
            ptr = v1.nextChar;        
            if(E() && v1.nextChar == EOF){
                cout<<"expression is parsed\n";
            }else{
                cout<<"parsing error\n";
            }

        }
        EXPR(TOKEN v){
           //start Symbol
            v1=v;
            cout << "Step" << setw(11) << "" << "Input\n" ;
            cout << "------------Enter the arithmetic expr-------------\n";
            ptr = v1.nextChar;        

        }

        bool EXP(){
            v1.getNonBlank();
            if(ptr == '+' || ptr == '-' ){
                v1.getNonBlank();
                if(ptr == '+')
                cout << "E' -> +TE"<< std::setw(6) << "" <<ptr << "\n";
                else 
                cout << "E' -> -TE"<< std::setw(6) << ""<<ptr << "\n";
                v1.lex();
                
                ptr=v1.nextChar;
                if(T()){
                    if(EXP()){
                        return SUCCESS;
                    }
                    else{
                        return FAILED;
                    }
                }
                else{
                    return FAILED;
                }
            }
            else{
                cout << "E' -> $"<< std::setw(8) << ""<<ptr << "\n";
                return SUCCESS;
            }
        }
        bool E(){
            v1.getNonBlank();
            cout << "E -> T E'" << std::setw(6) << ""<< ptr << "\n";
            if(T()){
                if(EXP()){
                    return SUCCESS;
                }
                else{
                    return FAILED;
                }
            }
            else{
               return FAILED;
            }
        }
        bool TERM(){
            v1.getNonBlank();
            
            if(ptr == '*'|| ptr == '/' || ptr == '%'){
                if(ptr == '*')
                cout<< "T -> *F T'"<< std::setw(5) << "" << ptr << "\n";
                else if(ptr == '/')
                cout<< "T -> /F T'" << std::setw(5) << ""<< ptr << "\n";
                else
                cout<< "T -> %F T'"<< std::setw(5) << "" << ptr << "\n";
                v1.lex();
                
                ptr=v1.nextChar;
                if(F()){
                    v1.getNonBlank();
                    if(TERM()){
                        return SUCCESS;
                    }
                    else{
                        return FAILED;
                    }
                }
                else{
                    return FAILED;
                }
            }
            else{
                cout << "T' -> $" << std::setw(8) << ""<< ptr << "\n";
                return SUCCESS;
            }
        }
        bool T(){
            v1.getNonBlank();
            cout << "T -> F T'"<< std::setw(6) << ""<<ptr<<"\n";
            
            if(F()){
                v1.getNonBlank();
                
                if(TERM()){
                    
                    return SUCCESS;
                }
                else{
                    return FAILED;
                }
            }
            else{
                return FAILED;
            }
        }
        bool isid(){
            char curr = ptr;
            if(!isalpha(curr))
            return 0;
            v1.lex();
            
            ptr=v1.nextChar;
            return true;
        }
        bool F(){
            v1.getNonBlank();
            if(ptr == '('){
                cout<<"F -> ( E )"<< std::setw(5) << ""<< ptr << "\n";
                v1.lex();
                
                ptr=v1.nextChar;
                if(E()){
                    if(ptr ==')'){
                        v1.lex();
                        v1.getChar();
                        ptr=v1.nextChar;

                        return SUCCESS;
                    }
                    else{
                        return FAILED;
                    }
                }
                else{
                    return FAILED;
                }
            }
            else if(isdigit(ptr) || isid()){
                cout<<"F -> val"<< std::setw(7) << ""<< ptr << "\n";
                v1.lex();
                
                ptr=v1.nextChar;
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
};
class STMT{
    //<BLOCK> --> `{` <STMT_LIST> `}`
    public:
    char ptr;
    TOKEN v1;
    STMT(){
        cout << "Parser for Statement List\n";
        cout << "Step" << setw(25) << "" << "Input\n" ;
        cout << "------------------------------------\n";
        ptr = v1.nextChar;
        if(STMT_LIST() && ptr == EOF){
            cout << "All the statements have been successfully parsed.\n"<<endl;
        }
        else{
            cout << "There was an error parsing one of the statements\n";
        }
        
    }
    bool STMT_LIST(){

     
        v1.getChar();
    
        v1.lex();
        if(STMT_TYPE()){
            
            if(STMT_LIST()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else {
            return SUCCESS;
        }
    }
    bool DECLARE(){
        //check if it has a datatype keyword 

        //lex will check for ID
        cout << "type -> "<< v1.pattern  <<endl;
        v1.pattern = "";
        v1.lex();
        ptr = v1.nextChar;

        if(v1.nextToken==IDENT || v1.charClass==LETTER){
            cout << "the variable name is " << v1.pattern << endl;
            if(ptr == ';'){
                v1.pattern = "";
                v1.getChar();
                v1.getNonBlank();
                if(v1.nextChar!= ' ')
                v1.getChar();
                
                ptr = v1.nextChar;
                cout << "this is a valid variable declaration"<<endl;
                return SUCCESS;
            }
            else{
                cout << "this is an invalid declaration"<<endl;
                return FAILED;
            }
        }
        else{
            cout << "the declaration didn't have a valid ID"<<endl;
            return FAILED;
        }
    }
    bool ASSIGNSTMT(){
        cout << "Enter Assignment statement:" << v1.pattern << "=\n";
        v1.getNonBlank();
        ptr = v1.nextChar;
        if(ptr == '='){
            v1.pattern = "";
            cout << " ID = EXPR <ASGNSTMT>\n";
            v1.getChar();
            EXPR e(v1);
            if(e.E() && e.v1.nextChar == ';'){
                cout << "This is a valid assignment\n";
                v1 = e.v1;
                v1.getChar();
                v1.getNonBlank();
                
                ptr = v1.nextChar;
               
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else{
            return FAILED;
        }
    }
    bool ELSESTMT(){
        cout << " <STMT> --> else <IF_STMT>" << setw(8) << "" << "else" <<"\n";
        // v1.getNonBlank();
        // v1.getChar();
        v1.pattern = "";
        v1.lex();
        ptr = v1.nextChar;


        if(BLOCK()){
            return SUCCESS;
        }
        else{
            return FAILED;
        }
    }
    bool IFSTMT(){
        cout << " <STMT> --> if <IF_STMT>" << setw(8) << "" << "if(" <<"\n";
        
        ptr = v1.nextChar;
        if(ptr == '('){
            v1.pattern = "";
            v1.getChar();
            BOOL_EXPR b1(v1);
            if(b1.BEXP() && b1.v1.nextChar == ')'){
                cout << "this boolean expression is successfully parsed\n";
                v1 = b1.v1;
                v1.getChar();
                ptr = v1.nextChar;
                if(BLOCK()){
                    return SUCCESS;
                }
                else{
                    return FAILED;
                }
            }
            else {
                cout << "not a bool expr\n";
                return FAILED;
            }
        }
        else{
            return FAILED;
        }

    }
    bool WHILESTMT(){
        cout << "<STMT> -> while <while_stmt>"<<setw(8)<<""<<"while("<<"\n";
        ptr = v1.nextChar;
        v1.getNonBlank();
        v1.getChar();

        if(ptr == '('){
            v1.pattern = "";
            BOOL_EXPR b1(v1);
            if(b1.BEXP() && b1.v1.nextChar == ')'){
                cout << "this boolean expression is successfully parsed\n";
                v1 = b1.v1;
                v1.getChar();
                v1.getNonBlank();
                ptr = v1.nextChar;
                
                if(BLOCK()){
                    return SUCCESS;
                }
                else{
                    return FAILED;
                }
            }
            else {
                cout << "not a bool expr\n";
                return FAILED;
            }
        }
        else{
            return FAILED;
        }
    }
    bool EXP(){
  
        cout << " <STMT> -->  EXP <EXPR>" << setw(8) << "" <<"\n";
        v1.getNonBlank();
        EXPR e1(v1);
        if(e1.E() && e1.v1.nextChar == '}'){
            cout << "this arithmetic expression is successfully parsed\n";
            v1 = e1.v1;
            return SUCCESS;
        }else{
            cout << "not an arithmetic expr "<<ptr<<"\n";
            return FAILED;
        }
    }
    bool BLOCK(){
        
        ptr = v1.nextChar;
    
        if(ptr == '{'){
            v1.getNonBlank();
            cout << "BLOCK ->  '{'" << setw(16) << "" << ptr <<"\n";
            v1.pattern = "";
            if(STMT_LIST()){
            //recursive stmt list parsing
                v1.getNonBlank();
                cout << v1.nextChar << "\n";
                if(ptr == '}'){
                    cout << "end of block "<<ptr <<"\n";
                    v1.getChar();
                    ptr = v1.nextChar;
                    
                    return SUCCESS;
                }
                else{
                    return FAILED;
                }
            }
            else{
                    return FAILED;
            }
        }
        else{
            return FAILED;
        }
    }
    bool STMT_TYPE(){
        if(v1.nextToken==BLOCK_CODE){
            if(BLOCK()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else if(v1.nextToken==IF_CODE){
            if(IFSTMT()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else if(v1.nextToken == ELSE_CODE){
            if(ELSESTMT()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else if(v1.nextToken==WHILE_CODE){
            if(WHILESTMT()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else if(v1.nextToken == IDENT){
            if(ASSIGNSTMT()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else if(v1.nextToken==DATATYPE){
            if(DECLARE()){
                return SUCCESS;
            }
            else{
                return FAILED;
            }
        }
        else{
            return FAILED;
        }
    }

};

/*
function to initalize parsers for expressions bool expression and statements. 

for test cases 
expressions: you can try any algebraic expression like 1+2*3/2
bool_exprs: you can use &&, <=, >=, etc in a statement like 1 <= 2 == 3 && 2
statements: you can start with a block {if(1<2)}, {1==2}

use the file front.in to read input

*/
int main() {
    cout << std::left;
    cout << "Reading from TestDoc.in"<< endl;
    STMT c1;
    return 0;
}
