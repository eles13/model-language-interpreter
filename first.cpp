#include<iostream>
#include<fstream>
#include <cstring>
#include <vector>
#include <map>
#include <stack>
using namespace std;

int pars = 0;
int upars = 0;
string lpar = "(";
string rpar = ")";
string lupar = "{" ;
string rupar = "}";

enum typo
{
    LEX_NULL,LEX_PROGRAM,
    LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FOR,LEX_FALSE, LEX_INT, LEX_GOTO, LEX_BREAK, LEX_NOT, LEX_OR, LEX_MAIN, LEX_READ, LEX_TRUE, LEX_STRING, LEX_WHILE, LEX_WRITE, 
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_RBC, LEX_LBC,
    LEX_NUM,LEX_CONST_STRING,LEX_UN_PLUS, LEX_UN_MINUS,LEX_CONCAT,LEX_POINT, LEX_EQ_STRING, LEX_LSS_STRING, LEX_GTR_STRING, LEX_LEQ_STRING, LEX_NEQ_STRING, LEX_GEQ_STRING,
    LEX_ID,LEX_END,
    POLIZ_FGO,
    POLIZ_GO,
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_STACK_CLEAR,
    LEX_POST_UN_PLUS,
    LEX_POST_UN_MINUS,
    LEX_PRE_UN_PLUS,
    LEX_PRE_UN_MINUS
};


class Lex 
{     
    typo t_lex;    
    int v_lex; 
    public:    
    Lex ( typo t = LEX_NULL, int v = 0)     { t_lex = t; v_lex = v; }        
    typo get_type () { return t_lex; } 
    int get_value () { return v_lex; } 
    friend ostream& operator << ( ostream &s, Lex l )     {  s << '(' << l.t_lex << ',' << l.v_lex << ");"; return s; }    
};

class Ident 
{ 
    char* name;     
    bool declare;         
    typo type;    
    bool assign;     
    int value; 
    public:     
    Ident (){ declare = false; assign = false;}    
    char *get_name () {return name;}    
    void put_name (const char *n)     
    {
        name =new char [ strlen(n) + 1 ];         
        strcpy ( name, n ); 
    }    
    bool get_declare () {return declare;}    
    void put_declare () { declare = true; } 
    typo get_type () {return type;}    
    void put_type ( typo t )     { type = t;}    
    bool get_assign () { return assign; }    
    void put_assign () { assign = true; }    
    int get_value () { return value; }    
    void put_value (int v) { value = v; }    
};


class table_id 
{
    Ident * p;
    int size;
    int top;
public:
    table_id ( int max_size ) 
    {
        p = new Ident[size=max_size];
        top = 1;
    }

    ~table_id () 
    {
        delete []p;
    }

    Ident& operator[] ( int k ) 
    {
        return p[k];
    }

    int put ( const char *buf ) 
    {
        for ( int j=1; j<top; ++j )
            if ( !strcmp(buf, p[j].get_name()) )
                return j;
        p[top].put_name(buf);
        ++top;
        return top-1;
    }
};

class Scanner {
    enum state { H, IDENT, NUMB, COM, STR, DELIM};
    state CS;
    FILE *fp;
    char c;
    string buf;
    //int buftop;


    void clear () 
    {
        buf = "";
    }


    void add () 
    {
        buf += c;
    }


    int look ( const char * buf, const char ** list ) 
    {
        int i = 0;
        while ( list[i] ) {
            if ( !strcmp(buf,list[i]))
                return i;
            ++i;
        }
        return -1;
    }


    void gc () 
    {
        c = fgetc(fp);
        if (buf == lpar) {pars++;}
        else if (buf == lupar) { upars++;}
        else if (buf == rpar) {pars--;}
        else if (buf == rupar) { upars--;}
        if ((pars<0)||(upars<0)) throw "error in number of parentheses";
    }


public:

    static const char *  TW[];
    static typo words[];
    static const char *  TD[];
    static typo dlms[];
    // //int upars = 0;
    // //int pars = 0;

    // map<typo,string> getDelims()
    // {
    //     map<typo,string> tmp;
    //     for (int i= 1; i < 22; i++)
    //         tmp[dlms[i]] = TD[i];
    //     return tmp;
    // }


    // map<typo,string> getWords()
    // {
    //     map<typo,string> tmp;
    //     for (int i = 0; i < 20;i++)
    //         tmp[words[i]] = TW[i];
    //     return tmp;
    // }




    Lex get_lex ();


    Scanner ( const char * program ) 
    {
        fp = fopen(program,"r");
        CS = H;
        clear();
        gc();
    }
};

const char * Scanner::TW[] = 
{
    "" ,
    "and",
    "bool",
    "do",
    "else",
    "if",
    "for",
    "false",
    "int",
    "not",
    "or",
    "program",
    "read",
    "break",
    "goto",
    "true",
    "string",
    "while",
    "write",
    NULL
};

typo Scanner::words[] = 
{
    LEX_NULL,
    LEX_AND,
    LEX_BOOL,
    LEX_DO,
    LEX_ELSE,
    LEX_IF,
    LEX_FOR,
    LEX_FALSE,
    LEX_INT,
    LEX_NOT,
    LEX_OR,
    LEX_PROGRAM,
    LEX_READ,
    LEX_BREAK,
    LEX_GOTO,
    LEX_TRUE,
    LEX_STRING,
    LEX_WHILE,
    LEX_WRITE,
    LEX_NULL
};


const char *  Scanner::TD[] = 
{
    "" ,
    ";",
    "==",
    ",",
    ":",
    "=",
    "(",
    ")",
    "<",
    ">",
    "+",
    "-",
    "*",
    "/",
    "<=",
    "!=",
    ">=",
    "{",
    "}",
    "++",
    "--",
    ".",
    NULL
};


typo Scanner::dlms[] = {
    LEX_NULL,
    LEX_SEMICOLON,
    LEX_EQ,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_LEQ,
    LEX_NEQ,
    LEX_GEQ,
    LEX_LBC,
    LEX_RBC,
    LEX_UN_PLUS,
    LEX_UN_MINUS,
    LEX_POINT,
    LEX_NULL
};


table_id TID(100);
vector<string> TCS;

Lex Scanner::get_lex() {

    CS = H;

    int  out;

    clear();

    do 
    {
        switch (CS) 
        {
            case H:
                if(c == '\n' || c == ' ' || c == '\r' || c == '\t') 
                {
                    gc();
                    break;
                }
                else if(c == '/')
                {
                    gc();
                    if(c == '*')
                        CS = COM;
                    else
                    {
                        //cout<<"slash "<<endl;
                        return Lex(LEX_SLASH,13);
                    } 
                } else if(c == '\"')
                {
                    CS = STR;
                }
                else if(isalpha(c)) 
                {
                    CS = IDENT;
                    add();
                } else if(isdigit(c)) 
                {
                    CS = NUMB;
                    add();
                } else if(c == EOF)
                {
                    fclose(fp);
                    //cout<<"end"<<endl;
                    return Lex(LEX_END);
                } else 
                {
                    CS = DELIM;
                    add();
                }
                gc();
                break;


            case COM:
                if(c == EOF)
                {   
                    //cout<<"end"<<endl;
                    return Lex(LEX_END);
                }    
                else if (c == '*')
                {
                    gc();
                    if (c == '/') 
                    {
                        gc();
                        CS = H;
                    } else if(c == EOF)
                    {
                        //cout<<"end"<<endl;
                        return Lex(LEX_END);
                    }    
                } 
                else  gc();
                break;

            case STR:
                if (c == EOF)
                    throw "incomplete string";
                else if (c == '\"')
                {
                    gc();
                    TCS.push_back(buf);
                    out = TCS.size() - 1;
                    //cout<<"const str "<<buf;
                    return Lex(LEX_CONST_STRING,out);
                }
                else 
                {
                    add();
                    gc();
                }
                break;


            case NUMB:
                if(isdigit(c))
                {
                    add();
                    gc();
                } else 
                {
                    //cout<<"num "<<buf<<endl;
                    return Lex(LEX_NUM, atoi(buf.c_str()));
                }
                break;


            case IDENT:
                if(isalpha(c) || isdigit(c)) 
                {
                    add();
                    gc();
                } else if ( (out = look(buf.c_str(),TW)) != -1)
                {
                    //cout<<TW[out]<<" as "<<buf<<endl;
                    return Lex(words[out],out);
                } else 
                {
                    out = TID.put(buf.c_str());
                    //cout<<"id put on "<<out<<" "<<buf<<endl;
                    return Lex(LEX_ID,out);
                }
                break;

            case DELIM:
                add();
                if((out = look(buf.c_str(),TD)) != -1)
                {
                    gc();
                    //cout<<"dlm "<<buf<<endl;
                    return Lex(dlms[out],out);
                } else if ((out = look(buf.substr(0,1).c_str(),TD)) != -1)
                {
                    //cout<<"dlm "<<buf;
                    return Lex(dlms[out],out);
                } else
                    throw "sequence could not be parsed";
        }
    } while (true);


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Parser
{
    Lex curr_lex; // текущая лексема
    typo type;
    typo def_type;
    bool marked_op;
    bool assignable = true;
    int in_cycle;
    int val;
    string id_type;
    Scanner sc;
    bool enable_assignations = true;
    stack<int> break_labels;
    stack<int> number_of_labels;

    stack < int > int_;
    stack < string > string_;
    stack < typo > lex_;

    map<string,typo> ids_map;
    map<typo,typo> ops_map;

    void gl()
    {
        curr_lex = sc.get_lex();
        type = curr_lex.get_type();
        val = curr_lex.get_value();
    }

    void P();
    void S();
    void E();
    void E1();
    void D();
    void T();
    void F();
    void I();


    void check_id();
    void check_id(Ident ident);
    void check_op();
    void check_not();
    void push_break_labels(int pos);
    void eq_bool();
    bool isnum(typo lex);
    bool isstring(typo lex);
    void fill_gotos_table();

public:
    Parser(const char * program) : sc(program)
    {
        in_cycle = 0;
        ops_map[LEX_EQ] = LEX_EQ_STRING;
        ops_map[LEX_NEQ] = LEX_NEQ_STRING;
        ops_map[LEX_LSS] = LEX_LSS_STRING;
        ops_map[LEX_LEQ] = LEX_LEQ_STRING;
        ops_map[LEX_GEQ] = LEX_GEQ_STRING;
        ops_map[LEX_GTR] = LEX_GTR_STRING;
    }

    void analyse();

};


void Parser::analyse() 
{
    gl();
    P();
    cout<<"all right, man"<<endl;
}


void Parser::P() 
{
    if(type == LEX_PROGRAM)
    {
        gl();
        S();
        if(type != LEX_END)
            throw curr_lex;
    }
    else
        throw curr_lex;
}


void Parser::S() 
{

    if(type == LEX_LBC)
    {
        gl();
        if(type == LEX_RBC)
        {
            gl();
            return;
        }
        S();
        while (true)
        {
            if(type == LEX_RBC)
            {
                gl();
                return;
            }
            S();
        }
    }
    else if(type == LEX_IF)
    {
        enable_assignations = false;
        gl();
        if(type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        if(type != LEX_RPAREN)
            throw curr_lex;
        gl();
        S();
        if (type == LEX_ELSE)
        {
            gl();
            S();
        }
    }
    else if(type == LEX_WHILE)
    {
        enable_assignations = false;
        gl();
        in_cycle++;
        if(type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        if (type != LEX_RPAREN)
            throw curr_lex;
        gl();
        S();
        in_cycle--;
    }
    else if (type == LEX_READ)
    {
        enable_assignations = false;
        gl();
        if(type != LEX_LPAREN)
            throw curr_lex;
        gl();
        I();
        if(!assignable)
            throw curr_lex;
        if (type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if(type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if(type == LEX_WRITE)
    {
        enable_assignations = false;
        gl();
        if(type != LEX_LPAREN)
            throw curr_lex;
        int ctr = 1;
        gl();
        E();
        while (type == LEX_COMMA)
        {
            gl();
            E();
            ++ctr;
        }
        if(type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if(type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if(type == LEX_ID && !TID[val].get_declare())
    {
        enable_assignations = false;
        TID[val].put_declare();
        gl();
        if(type != LEX_COLON)
            throw "bad syntax";
        gl();
        S();
    }
    else if (type == LEX_STRING || type == LEX_INT )
    {
        if(!enable_assignations) 
            throw "wrong assignation";
        def_type = type;
        do{
            gl();
            D();
        } while (type == LEX_COMMA);
        if (type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if (type == LEX_ID)
    {
        
        E();
        gl();
    }
    else throw "bad syntax";

}

void Parser::D()
{
    if (type != LEX_ID)
        throw curr_lex;
    if (TID[val].get_declare())
        throw "already declared";
    else
    {
        TID[val].put_declare();
        TID[val].put_type(def_type);
    }
    gl();
    if(type == LEX_ASSIGN)
    {
        lex_.push(def_type);
        lex_.push(type);
        gl();
        E();
        check_op();
    }
}

void Parser::E() 
{
    E1();
    if (type == LEX_LSS || type == LEX_GTR || type == LEX_GEQ || type == LEX_EQ || type == LEX_LEQ)
    {
        lex_.push(type);
        gl();
        E1();
        check_op();
    }
}


void Parser::E1() 
{
    T();
    while (type == LEX_PLUS || type == LEX_MINUS || type == LEX_OR)
    {
        lex_.push(type);
        gl();
        T();
        check_op();
    }
}


void Parser::T() 
{
    F();
    while (type == LEX_TIMES || type == LEX_AND || type == LEX_SLASH)
    {
        lex_.push(type);
        gl();
        F();
        check_op();
    }
}


void Parser::F() 
{
    if(type == LEX_NUM)
    {
        lex_.push(LEX_INT);
        gl();
    } else if(type == LEX_CONST_STRING)
    {
        lex_.push(LEX_STRING);
        gl();
    } 
    else if (type == LEX_TRUE)
    {
        lex_.push(LEX_BOOL);
        gl();
    } else if(type == LEX_FALSE )
    {
        lex_.push(LEX_BOOL);
        gl();
    }    
    else if(type == LEX_MINUS)
    {
        lex_.push(LEX_INT);
        lex_.push(LEX_MINUS);
        gl();
        E();
        check_op();
    }
    else if(type == LEX_NOT)
    {
        gl();
        F();
        check_not();
    } else if (type == LEX_LPAREN)
    {
        gl();
        E();
        if(type != LEX_RPAREN)
            throw curr_lex;
        gl();
    } else if(type == LEX_ID || type == LEX_UN_MINUS || type == LEX_UN_PLUS) 
    {
        //cout<<"almost in I"<<endl;
        I();
        if(type == LEX_ASSIGN)
        {
            if(!assignable)
                throw curr_lex;
            lex_.push(type);
            gl();
            E();
            check_op();
        }
    } else 
    {
        I();
    }
}


void Parser::I() 
{
    string name;
    assignable = true;
    //cout<<"in table on pos "<<val<<endl;
    //cout<<"dec "<<TID[val].get_declare()<<endl;
    if(type == LEX_UN_PLUS || type == LEX_UN_MINUS)
    {
        assignable = false;
        gl();
        if(type != LEX_ID || !TID[val].get_declare())
            throw curr_lex;
        
        if(TID[val].get_type() != LEX_INT || !TID[val].get_declare() )
            throw curr_lex;
        check_id();
        gl();
        if(type == LEX_UN_PLUS || type == LEX_UN_MINUS)
            throw curr_lex;
    } else if(type == LEX_ID && TID[val].get_declare())
    {
        name = TID[val].get_name();
        int prev_v = val;
        gl();
        check_id(TID[prev_v]);
        if(type == LEX_UN_PLUS || type == LEX_UN_MINUS)
        {
            if(TID[prev_v].get_type() != LEX_INT || !TID[prev_v].get_declare())
                throw curr_lex;
            gl();
            assignable = false;
                
            

        }
    }
}




void Parser::check_id(Ident ident)
{
    if(ident.get_declare())
        lex_.push(ident.get_type());
    else
        throw "not declared";
}



void Parser::check_id()
{
    if ( TID[val].get_declare() )
    {
        lex_.push(TID[val].get_type());
    }
    else
        throw "not declared";
}


void Parser::eq_bool() 
{
    if(lex_.top() != LEX_BOOL)
        throw "the expression is not boolean!";
    lex_.pop();
}


void Parser::check_op ()
{
    typo t1, t2, op = LEX_INT, r = LEX_BOOL;
    t2 = lex_.top();
    lex_.pop();
    op = lex_.top();
    lex_.pop();
    t1 = lex_.top();
    lex_.pop();
    //cout<<"in check"<<endl;
    //cout<<t1<<endl;
    //cout<<op<<endl;
    //cout<<t2<<endl;
    if(isnum(t1) && isnum(t2)){
        if(op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH|| op == LEX_ASSIGN)
        {
            r = LEX_INT;
            //cout<<"int" <<endl;
        }   
        else if(op == LEX_LEQ || op == LEX_LSS || op == LEX_GEQ || op == LEX_GTR || op == LEX_NEQ || op == LEX_EQ)
        {
            r = LEX_BOOL;
            //cout<<"bool"<<endl;
        }    
        else
            throw "wrong types";
    } else if(t1 == LEX_BOOL && t2 == LEX_BOOL)
    {
        if(op == LEX_OR || op == LEX_AND || op == LEX_ASSIGN)
            r = LEX_BOOL;
        else
            throw "wrong types";
    } else if(isstring(t1) && isstring(t2))
    {
        if(op == LEX_PLUS || op == LEX_ASSIGN)
        {
            r = LEX_STRING;
            if(op == LEX_PLUS)
                op = LEX_CONCAT;
        }
        else if(op == LEX_LEQ || op == LEX_LSS || op == LEX_GEQ || op == LEX_GTR || op == LEX_NEQ || op == LEX_EQ)
        {
            op = ops_map[op];
            r = LEX_BOOL;
        }
        else 
        {
            throw "wrong types";
        }    
    }
    else if (isnum(t2))
    {
        if (op == LEX_PLUS || op == LEX_MINUS)
        {
            r = LEX_INT;
            lex_.push(t1);
        }
        else throw "wrong types";
    }    
    else throw "wrong types";
    lex_.push(r);
}


void Parser::check_not() 
{
    if (lex_.top() != LEX_BOOL)
        throw "wrong type in not";
}


bool Parser::isnum(typo lex)
{
    return lex == LEX_NUM || lex == LEX_INT;
}

bool Parser::isstring(typo lex)
{
    return lex == LEX_STRING || lex == LEX_CONST_STRING;
}

int main()
{
    try 
    {
        Parser p("./test.txt");
        p.analyse();
    }
    catch (Lex& lex)
    {
        cout<<lex<<endl;
    }
    catch (const char* & err)
    {
        cout<< err << endl;
    }

    return 0;
}