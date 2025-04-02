#include "SyntaxAnalyzer.h"


bool simpleexpr(){return true;} bool stmtlist(){return true;} bool whilestmt(){return true;}
bool assignstmt(){return true;} bool inputstmt(){return true;} bool vdecassign(){return true;}
bool //SyntaxAnalyzer::
elsepart(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (*tokitr == "t_else") {
        tokitr++; lexitr++;
        return true;
    }
    return true;
}

bool //SyntaxAnalyzer::
logicop(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end()) {
        if (*tokitr == "t_and" || *tokitr == "t_or" ) {
            ++tokitr; ++lexitr;
            return true;
        }
    }
    return false;
}

bool //SyntaxAnalyzer::
expr(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end()) {
        if ( simpleexpr() ) {
            ++tokitr;++lexitr;
            if (tokitr != tok.end() && logicop(tok,tokitr,lexitr)) {
                if (tokitr != tok.end() && simpleexpr()) {
                    ++tokitr;++lexitr;
                }

            }

            return true;
        }
    }
    return false;
}



bool //SyntaxAnalyzer::
checkStmtList(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (*tokitr == "s_lbrace" && tokitr != tok.end()) {
        ++tokitr; ++lexitr;
        if (stmtlist()) {
            ++tokitr; ++lexitr;
            if (*tokitr == "s_rbrace" && tokitr != tok.end()) {
                ++tokitr; ++lexitr;
                return true;
            }
        }
    }
    return false;
}


bool //SyntaxAnalyzer::
checkExpr(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (*tokitr == "s_lparen") {
        ++tokitr; ++lexitr;
        if (expr(tok,tokitr,lexitr)&& tokitr != tok.end() ) {
            if (*tokitr == "s_rparen") {
                ++tokitr; ++lexitr;
                return true;
            }
        }
    }
    return false;
}

bool //SyntaxAnalyzer::
outputstmt(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    ++tokitr; ++lexitr;
    if (tokitr != tok.end()) {
        if (*tokitr == "s_lparen") {
            ++tokitr; ++lexitr;
            if (tokitr != tok.end() && *tokitr == "t_text") {
                ++tokitr; ++lexitr;
                if (*tokitr == "s_rparen") {
                    ++tokitr; ++lexitr;
                    return true;
                }
            }
            else if ( expr(tok,tokitr,lexitr) ) {
                if (*tokitr == "s_rparen") {
                    ++tokitr; ++lexitr;
                    return true;
                }
            }
        }
    }
    return false;

}

bool //SyntaxAnalyzer::
ifstmt(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    ++tokitr; ++lexitr;
    if (tokitr != tok.end()) {
        if (checkExpr(tok ,tokitr,lexitr)) {
            if (checkStmtList(tok ,tokitr,lexitr)) {
                if(elsepart(tok ,tokitr,lexitr)) {
                    return true;
                }
            }
        }
    }

    return false;
}


int //SyntaxAnalyzer::
stmt(vector<string>& tok, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end()) {

        if ( ifstmt(tok ,tokitr,lexitr) ) {
            return 1;
        }

        if (outputstmt(tok ,tokitr,lexitr)) {
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
    vector<string> tokens = {"t_output", "s_lparen","simpleExpr","s_rparen","t_output", "s_lparen","t_text","s_rparen"};
    vector<string> lexemes= {"output", "(","simpleExpr",")","output","(","textHere",")"};
    vector<string>::iterator tokitr;
    vector<string>::iterator lexitr;
    tokitr = tokens.begin();
    lexitr = lexemes.begin();
    while (tokitr != tokens.end()) {
        if (outputstmt(tokens ,tokitr,lexitr)) {
            cout << "Works" << endl;
        }
        else {
            cout << "Dosnt work" << endl;
            break;
        }
    }


}




