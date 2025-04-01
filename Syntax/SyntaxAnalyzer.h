#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

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
    bool vdecassign();  //Ethan
    bool stmtlist();    //Ethan
    int stmt();         //ME
    bool ifstmt();      //ME
    bool elsepart();    //Ethan
    bool whilestmt();   //David L
    bool assignstmt();  //David L
    bool inputstmt();   //David L
    bool outputstmt();  //ME
    bool expr();        //ME
    bool simpleexpr();  //Ethan
    bool term();        //David L
    bool logicop();     //ME
    bool arithop();     //David L
    bool relop();       //Ethan

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
