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

    bool stmtlist();    //Ethan
    int stmt();         //Ethan
    bool ifstmt();      //David G
    bool elsepart();    //Ethan
    bool whilestmt();
    bool assignstmt();
    bool inputstmt();
    bool outputstmt();
    bool expr();
    bool simpleexpr();
   bool term();
   bool logicop(); //and, or
    bool arithop(); // +, -, /
    bool relop(); // Ethan ==, <=, =>, !=

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

bool SyntaxAnalyzer::simpleexpr(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexit){
    if (!term(tok,lex,tokitr,lexitr)) {
        return false;
    }
    if (tokitr != tok.end()) {
        if (relop(lex,tok,tokitr,lexit)) {
            return true;
        } else if (arithop(lex,tok,tokitr,lexit)) {
            return true;
        }
    }
    return false;
}

bool SyntaxAnalyzer::relop(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexit) {
    if (tokitr != tok.end() && (*tokitr == "s_eq" || *tokitr == "s_lt" ||
        *tokitr == "s_gt" || tokitr == "s_ne")) {
        ++tokitr;
        ++lexitr;
        return true;
        }
    return false;
}