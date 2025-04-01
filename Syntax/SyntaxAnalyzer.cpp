#include "SyntaxAnalyzer.h"


bool simpleexpr(){return true;} bool stmtlist(){return true;} bool whilestmt(){return true;}
bool assignstmt(){return true;} bool inputstmt(){return true;} bool vdecassign(){return true;}
bool elsepart(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (*tokitr == "t_else") {
        tokitr++; lexitr++;
        return true;
    }
    return true;
}

bool logicop(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end()) {
        if (*tokitr == "t_and" || *tokitr == "t_or" ) {
            ++tokitr; ++lexitr;
            return true;
        }
    }
    return false;
}

bool expr(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end()) {
        if ( simpleexpr() ) {
            ++tokitr;++lexitr;
            if (tokitr != tok.end() && logicop(tok,lex,tokitr,lexitr)) {
                if (tokitr != tok.end() && simpleexpr()) {
                    ++tokitr;++lexitr;
                }

            }

            return true;
        }
    }
    return false;
}

bool outputstmt(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end()) {
        if (*tokitr == "t_text") {
            ++tokitr; ++lexitr;
            return true;
        }
        else if ( expr(tok,lex,tokitr,lexitr) ) {
            return true;
        }
    }
    return false;
}



bool ifstmt(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    ++tokitr; ++lexitr;
    if (tokitr != tok.end()) {
        if (*tokitr == "s_lparen") {
            ++tokitr; ++lexitr;
            if ( expr(tok,lex,tokitr,lexitr)) {
                if (*tokitr == "s_rparen") {
                    ++tokitr; ++lexitr;
                    if (*tokitr == "s_lbrace") {
                        ++tokitr; ++lexitr;
                        if (stmtlist()) {
                            ++tokitr; ++lexitr;
                            if (*tokitr == "s_rbrace") {
                                ++tokitr; ++lexitr;
                                if(elsepart(tok,lex,tokitr,lexitr)) {
                                    return true;
                                }

                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}


int stmt(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end()) {
        if ( ifstmt(tok,lex,tokitr,lexitr) ) {
            return 1;
        }

        if (outputstmt(tok,lex,tokitr,lexitr)) {
            return 5;
        }

        if ( whilestmt()) {
            return 2;
        }
        if ( assignstmt()) {
            return 3;
        }
        if ( inputstmt()) {
            return 4;
        }

        if (vdecassign()) {
            return 6;
        }
    }
    return 0;
}



int main() {
    vector<string> tokens = {"t_if", "s_lparen","simpleExpr","s_rparen","s_lbrace","StatementList","s_rbrace"};
    vector<string> lexemes= {"if", "(","simpleExpr",")","{","StatementList","}"};
    vector<string>::iterator tokitr;
    vector<string>::iterator lexitr;
    tokitr = tokens.begin();
    lexitr = lexemes.begin();
    while (tokitr != tokens.end()) {
        if (ifstmt(tokens,lexemes,tokitr,lexitr)) {
            cout << "Works" << endl;
        }
        else {
            cout << "Dosnt work" << endl;
            break;
        }
    }

    // vector<string> tokens = {"t_if", "t_while","s_assign","t_input","t_output","t_text"};
    // vector<string> lexemes= {"if(){}", "while(){}","=","input","output","text"};
    // vector<string>::iterator tokitr;
    // vector<string>::iterator lexitr;
    // tokitr = tokens.begin();
    // lexitr = lexemes.begin();
    // while (tokitr != tokens.end()) {
    //     if (stmt(tokens,lexemes,tokitr,lexitr)) {
    //         cout << "Works" << endl;
    //     }
    //     else {
    //         cout << "Dosnt work" << endl;
    //         break;
    //     }
    // }

}




