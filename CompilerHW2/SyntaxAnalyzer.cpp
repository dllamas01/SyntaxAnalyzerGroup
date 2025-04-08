#include "SyntaxAnalyzer.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool SyntaxAnalyzer::parse() {
    if (tokitr != tokens.end() && *tokitr == "t_main") {
        ++tokitr; ++lexitr;
        if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
            ++tokitr; ++lexitr;
            if (stmtlist()) {
                if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
                    ++tokitr; ++lexitr;
                    if (tokitr == tokens.end()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//Ethan
bool SyntaxAnalyzer::relop() {
    if (tokitr != tokens.end() && (*tokitr == "s_eq" || *tokitr == "s_lt" ||*tokitr == "s_gt" || *tokitr == "s_ne")) {
        ++tokitr;++lexitr;
        return true;
    }
    return false;
}

//Ethan
bool SyntaxAnalyzer::simpleexpr() {
    if (!term()) return false;
    if (tokitr != tokens.end()) {
        if (relop()) {
            return term(); // must follow relop with a term
        } else if (arithop()) {
            return term(); // must follow arithop with a term
        }
    }
    return true; // just a TERM is also valid!
}

//Ethan
bool SyntaxAnalyzer::vdecassign() {
    if (tokitr == tokens.end()) {
        return false;
    }
    if (*tokitr == "t_integer" || *tokitr == "t_string") {
        string type = *lexitr;
        ++tokitr; ++lexitr;
        if (tokitr != tokens.end() && *tokitr == "t_id") {
            string varName = *lexitr;
            if (symboltable.find(varName) != symboltable.end()) {
                cout << "Error: Variable '" << varName << "' already declared." << endl;
                return false; // variable already declared
            }
            symboltable[varName] = type;
            ++tokitr; ++lexitr;
            if (tokitr != tokens.end() && *tokitr == "s_assign") {
                ++tokitr; ++lexitr;
                if (expr()) {
                    if (tokitr != tokens.end() && *tokitr == "s_semi") {
                        ++tokitr; ++lexitr;
                        return true;
                    } else {
                        cout << "Error: Missing semicolon in variable declaration.\n";
                    }
                } else {
                    cout << "Error: Invalid expression in variable declaration.\n";
                }
            } else {
                cout << "Error: Missing '=' in variable declaration.\n";
            }
        }
    }
    return false;
}


//David G
int SyntaxAnalyzer::stmt() {
    if (tokitr != tokens.end()) {
        if (*tokitr == "t_if") {
            if (ifstmt()) {
                return 1;
            }
            else {return 0;}
        }
        else if (*tokitr == "t_while") {
            if (whilestmt()) {
                return 1;
            }
            else {return 0;}
        }
        else if (*tokitr == "t_id") {
            if (assignstmt()) {
                return 1;
            }
            else {return 0;}
        }
        else if (*tokitr == "t_input") {
            if (inputstmt()) {
                return 1;
            }
            else {return 0;}
        }
        else if (*tokitr == "t_output") {
            if (outputstmt()) {
                return 1;
            }
            else {return 0;}
        }
        else if (*tokitr == "t_integer" || *tokitr == "t_string") {
            if (vdecassign()) {
                return 1;
            }
            else {return 0;}
        }
        return 2;
    }
}

//Ethan
bool SyntaxAnalyzer::stmtlist() {
    if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
        return true;
    }
    if (!stmt()) {
        return false;
    }
    while (tokitr != tokens.end() && stmt() != 2) {
    }

    return true;
}
//Ethan
bool SyntaxAnalyzer::elsepart() {
    if (*tokitr != "t_else") {
        return true;
    }
    else{
        ++tokitr;++lexitr;
        return true;
    }
    return true;
}


//David G
bool SyntaxAnalyzer::logicop() {
    if (tokitr != tokens.end()) {
        if (*tokitr == "t_and" || *tokitr == "t_or") {
            ++tokitr; ++lexitr;
            return true;
        }
    }
    return false;
}


//David G
bool SyntaxAnalyzer::expr() {
    if (!simpleexpr()) return false;

    // Optional: logicop followed by another simpleexpr
    if (tokitr != tokens.end() && (*tokitr == "t_and" || *tokitr == "t_or")) {
        if (!logicop()) return false;  // logicop will advance
        if (!simpleexpr()) return false;
    }

    return true;
}


//David G
bool SyntaxAnalyzer::checkStmtList() {
    if (*tokitr == "s_lbrace" && tokitr != tokens.end()) {
        ++tokitr; ++lexitr;
        if (stmtlist()) {
            if (*tokitr == "s_rbrace" && tokitr != tokens.end()) {
                ++tokitr; ++lexitr;
                return true;
            }
        }
    }
    return false;
}


//David G
bool SyntaxAnalyzer::checkExpr() {
    if (*tokitr == "s_lparen") {
        ++tokitr; ++lexitr;
        if (expr() && tokitr != tokens.end()) {
            if (*tokitr == "s_rparen") {
                ++tokitr; ++lexitr;
                return true;
            }
        }
    }
    return false;
}


//David G
bool SyntaxAnalyzer::outputstmt() {
    ++tokitr; ++lexitr; // skip 't_output'

    if (tokitr != tokens.end() && *tokitr == "s_lparen") {
        ++tokitr; ++lexitr;

        // Option 1: string literal
        if (tokitr != tokens.end() && *tokitr == "t_text") {
            ++tokitr; ++lexitr;
        }
            // Option 2: expression like t_id
        else if (!expr()) {
            cout << "ERROR: Invalid expression in output statement.\n";
            return false;
        }

        if (tokitr != tokens.end() && *tokitr == "s_rparen") {
            ++tokitr; ++lexitr;
            return true;
        } else {
            cout << "ERROR: Missing ')' in output statement.\n";
        }
    }

    return false;
}




//David G
bool SyntaxAnalyzer::ifstmt() {
    ++tokitr; ++lexitr;
    if (tokitr != tokens.end()) {
        if (checkExpr()) {
            if (checkStmtList()) {
                if (elsepart()) {
                    return true;
                }
            }
        }
    }
    return false;
}




bool SyntaxAnalyzer::checkDeclaration(const string& varName) {
    if (symboltable.find(varName) == symboltable.end()) {
        cout << "ERROR: Variable '" << varName << "' used without declaration.\n";
        return false; // Variable was not declared
    }
    return true;
}
void SyntaxAnalyzer::declareVariable(const string& varName) {
    if (symboltable.find(varName) == symboltable.end()) {
        symboltable[varName] = "unknown";
    }
}

//David LL
bool SyntaxAnalyzer::whilestmt() {
    if (tokitr != tokens.end() && *tokitr == "t_while") {
        ++tokitr; ++lexitr;
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            ++tokitr; ++lexitr;
            if (expr()) {
                if (tokitr != tokens.end() && *tokitr == "s_rparen") {
                    ++tokitr; ++lexitr;
                    if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
                        ++tokitr; ++lexitr;
                        if (stmtlist()) {
                            if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
                                ++tokitr; ++lexitr;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

//David LL
bool SyntaxAnalyzer::assignstmt() {
    if (tokitr != tokens.end() && *tokitr == "t_id") {
        string varName = *lexitr;
        declareVariable(varName);
        ++tokitr; ++lexitr;
        if (tokitr != tokens.end() && *tokitr == "s_assign") {
            ++tokitr; ++lexitr;
            if (expr()) {
                if (tokitr != tokens.end() && *tokitr == "s_semi") {
                    ++tokitr; ++lexitr;
                    return true;
                } else {
                    cout << "ERROR: Missing semicolon at end of assignment statement.\n";
                }
            } else {
                cout << "ERROR: Invalid expression on right-hand side of assignment.\n";
            }
        } else {
            cout << "ERROR: Expected '=' after identifier in assignment.\n";
        }
    }
    return false;
}

//David LL
bool SyntaxAnalyzer::inputstmt() {
    if (tokitr != tokens.end() && *tokitr == "t_input") {
        ++tokitr; ++lexitr;
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            ++tokitr; ++lexitr;
            if (tokitr != tokens.end() && *tokitr == "t_id") {
                string varName = *lexitr;
                if (checkDeclaration(varName)) {
                    ++tokitr; ++lexitr;
                    if (tokitr != tokens.end() && *tokitr == "s_rparen") {
                        ++tokitr; ++lexitr;
                        return true;
                    }
                } else {
                    return false; // declaration check failed
                }
            }
        }
    }
    return false;
}

//David LL
bool SyntaxAnalyzer::term() {
    if (tokitr != tokens.end()) {
        if (*tokitr == "t_id") {
            string varName = *lexitr;
            if (checkDeclaration(varName)) {
                ++tokitr; ++lexitr;
                return true;
            } else {
                return false;
            }
        }
        if (*tokitr == "t_number" || *tokitr == "t_text") {
            ++tokitr; ++lexitr;
            return true;
        }
        if (*tokitr == "s_lparen") {
            ++tokitr; ++lexitr;
            if (expr()) {
                if (tokitr != tokens.end() && *tokitr == "s_rparen") {
                    ++tokitr; ++lexitr;
                    return true;
                } else {
                    cout << "ERROR: Expected ')' after expression.\n";
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return false;
}

//David LL
bool SyntaxAnalyzer::arithop() {
    bool valid = false;
    if (tokitr != tokens.end()) {
        if (*tokitr == "s_plus" || *tokitr == "s_minus" || *tokitr == "s_div") {
            ++tokitr;++lexitr;
            valid = true;
        }
    }
    return valid;
}

SyntaxAnalyzer::SyntaxAnalyzer(istream& infile) {
    string wordPair;
    while (getline(infile, wordPair)) {
        size_t pos = wordPair.find(" ");
        if (pos != string::npos) {
            string token = wordPair.substr(0, pos);
            string lex = wordPair.substr(pos + 1);
            tokens.push_back(token);
            lexemes.push_back(lex);
        }
    }
    tokitr = tokens.begin();
    lexitr = lexemes.begin();
}

int main() {
    ifstream test("test.txt");
    SyntaxAnalyzer syntax(test);
    if(syntax.parse()) {
        cout << "SYNTAX is Correct";
    }
    else {
        cout << "SYNTAX ERROR";
    }
}

