#include <iostream>
#include <fstream>
#include <vector>
#include <string>
class SyntaxAnalyzer{
private:
    vector<string> lexemes;
    vector<string> tokens;
    vector<string>::iterator lexitr;
    vector<string>::iterator tokitr;
    // map of variables and their datatype
    // i.e. sum t_integer
    map<string, string> symboltable;

    // other private methods

    bool stmtlist(); //Ethan
    int stmt(); // David G
    bool ifstmt();// David G
    bool elsepart(); //Ethan
    bool whilestmt();//David Llamas
    bool assignstmt(); // David Llamas
    bool inputstmt(); // David Llamas
    bool outputstmt();// David G
    bool expr();// David G
    bool simpleexpr();//Ethan
    bool term();//David Llamaas
    bool logicop(); //and, or, David G
    bool arithop(); // +, -, /, David Llamas
    bool relop(); // Ethan ==, <=, =>, !=
    //Ethan doing vdecassignment
public:
    SyntaxAnalyzer(istream& infile);
    // pre: 1st parameter consists of an open file containing a source code's
    //	valid scanner/lexical analyzer output.  This data must be in the form: token : lexeme
    // post: the vectors have been populated

    bool parse();
    // pre: none
    // post: The lexemes/tokens have been parsed.
    // If an error occurs, a message prints indicating the token/lexeme pair
    // that caused the error.
    // If no error, vectors contain syntactically correct source code

};

int main() {
    return 0;
}

//Ethan
bool SyntaxAnalyzer::stmtlist(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tokens.end() && *tokitr == "}") {
        return true;
    }
    if (!stmt(lex,tok,tokitr,lexitr)) {
        return false;
    }
    while (tokitr != tokens.end() && stmt(lex,tok,tokitr,lexitr)) {
    }
    return true;
}

//Ethan
bool SyntaxAnalyzer::simpleexpr(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexit){
    if (!term(tok,lex,tokitr,lexitr)) {
        return false;
    }
    if (tokitr != tok.end()) {
        if (relop(lex,tok,tokitr,lexit)){
            return true;
        } else if (arithop(lex,tok,tokitr,lexit)) {
            return true;
        }
    }
    return false;
}

//Ethan
bool SyntaxAnalyzer::relop(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexit) {
    if (tokitr != tok.end() && (*tokitr == "s_eq" || *tokitr == "s_lt" ||
        *tokitr == "s_gt" || tokitr == "s_ne")) {
        ++tokitr;
        ++lexitr;
        return true;
        }
    return false;
}
