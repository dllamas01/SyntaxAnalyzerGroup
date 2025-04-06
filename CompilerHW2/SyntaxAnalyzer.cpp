#include "SyntaxAnalyzer.h"

bool SyntaxAnalyzer::vdecassign(std::vector<std::string> &tok, std::vector<std::string>::iterator &tokitr,
                                std::vector<std::string>::iterator &lexitr) {
    if (tokitr == tok.end()) {
        return false;
    }
    if (*tokitr == "t_keyword" &&
        (*lexitr == "integer" || *lexitr == "string")) {
        ++tokitr;
        ++lexitr;
        if (tokitr != tok.end() && *tokitr == "t_id") {
            string varName = *lexitr;
            if (!checkDeclaration(varName)) {
                cout << "Error: Variable '" << varName << "' already declared." << endl;
                return false;
            }
            ++tokitr;
            ++lexitr;
            if (tokitr != tok.end() && *tokitr == "s_assign" && *lexitr == "=") {
                ++tokitr;
                ++lexitr;
                if (expr(tok, tokitr, lexitr)) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool SyntaxAnalyzer::elsepart(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    if (tokitr == tok.end()) {
        return false;
    }
    if (*tokitr == "t_else") {
        tokitr++;
        lexitr++;
        return true;
    }
    return true;
}

//David G
bool SyntaxAnalyzer::logicop(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    if (tokitr != tok.end()) {
        if (*tokitr == "t_and" || *tokitr == "t_or") {
            ++tokitr;
            ++lexitr;
            return true;
        }
    }
    return false;
}

//David G
bool SyntaxAnalyzer::expr(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    if (tokitr != tok.end()) {
        if (simpleexpr(tok, tokitr, lexitr)) {
            ++tokitr;
            ++lexitr;
            if (tokitr != tok.end() && logicop(tok, tokitr, lexitr)) {
                if (tokitr != tok.end() && simpleexpr(tok, tokitr, lexitr)) {
                    ++tokitr;
                    ++lexitr;
                }
            }

            return true;
        }
    }
    return false;
}


//David G
bool SyntaxAnalyzer::checkStmtList(vector<string> &tok, vector<string>::iterator &tokitr,
                                   vector<string>::iterator &lexitr) {
    if (*tokitr == "s_lbrace" && tokitr != tok.end()) {
        ++tokitr;
        ++lexitr;
        if (stmtlist(tok, tokitr, lexitr)) {
            ++tokitr;
            ++lexitr;
            if (*tokitr == "s_rbrace" && tokitr != tok.end()) {
                ++tokitr;
                ++lexitr;
                return true;
            }
        }
    }
    return false;
}

//David G
bool SyntaxAnalyzer::checkExpr(vector<string> &tok, vector<string>::iterator &tokitr,
                               vector<string>::iterator &lexitr) {
    if (*tokitr == "s_lparen") {
        ++tokitr;
        ++lexitr;
        if (expr(tok, tokitr, lexitr) && tokitr != tok.end()) {
            if (*tokitr == "s_rparen") {
                ++tokitr;
                ++lexitr;
                return true;
            }
        }
    }
    return false;
}

//David G
bool SyntaxAnalyzer::outputstmt(vector<string> &tok, vector<string>::iterator &tokitr,
                                vector<string>::iterator &lexitr) {
    ++tokitr;
    ++lexitr;
    if (tokitr != tok.end()) {
        if (*tokitr == "s_lparen") {
            ++tokitr;
            ++lexitr;
            if (tokitr != tok.end() && *tokitr == "t_text") {
                ++tokitr;
                ++lexitr;
                if (*tokitr == "s_rparen") {
                    ++tokitr;
                    ++lexitr;
                    return true;
                }
            } else if (expr(tok, tokitr, lexitr)) {
                if (*tokitr == "s_rparen") {
                    ++tokitr;
                    ++lexitr;
                    return true;
                }
            }
        }
    }
    return false;
}

//David G
bool SyntaxAnalyzer::ifstmt(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    ++tokitr;
    ++lexitr;
    if (tokitr != tok.end()) {
        if (checkExpr(tok, tokitr, lexitr)) {
            if (checkStmtList(tok, tokitr, lexitr)) {
                if (elsepart(tok, tokitr, lexitr)) {
                    return true;
                }
            }
        }
    }
    return false;
}

//Ethan
bool SyntaxAnalyzer::relop(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    if (tokitr != tok.end() && (*tokitr == "s_eq" || *tokitr == "s_lt" ||
                                *tokitr == "s_gt" || *tokitr == "s_ne")) {
        ++tokitr;
        ++lexitr;
        return true;
    }
    return false;
}


//David LL
bool SyntaxAnalyzer::inputstmt(vector<string> &tok, vector<string>::iterator &tokitr,
                               vector<string>::iterator &lexitr) {
    if (tokitr != tok.end() && *tokitr == "t_input") {
        ++tokitr;
        ++lexitr;
        if (tokitr != tok.end() && *tokitr == "t_lparen") {
            ++tokitr;
            ++lexitr;
            if (tokitr != tok.end() && *tokitr == "t_id") {
                string varName = *lexitr;
                if (checkDeclaration(varName)) {
                    ++tokitr;
                    ++lexitr;
                    if (tokitr != tok.end() && *tokitr == "t_rparen") {
                        ++tokitr;
                        ++lexitr;
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
bool SyntaxAnalyzer::term(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    if (tokitr != tok.end()) {
        if (*tokitr == "t_id") {
            string varName = *lexitr;
            if (checkDeclaration(varName)) {
                ++tokitr;
                ++lexitr;
                return true;
            } else {
                return false;
            }
        }
        if (*tokitr == "t_num" || *tokitr == "t_str") {
            ++tokitr;
            ++lexitr;
            return true;
        }
        if (*tokitr == "t_lparen") {
            ++tokitr;
            ++lexitr;
            if (expr(tok, tokitr, lexitr)) {
                if (tokitr != tok.end() && *tokitr == "t_rparen") {
                    ++tokitr;
                    ++lexitr;
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
bool SyntaxAnalyzer::arithop(vector<string> &tok, vector<string>::iterator &tokIt, vector<string>::iterator &lexIt) {
    bool valid = false;
    if (tokIt != tok.end()) {
        if (*tokIt == "t_plus" || *tokIt == "t_minus" || *tokIt == "t_div") {
            ++tokIt;
            ++lexIt;
            valid = true;
        }
    }
    return valid;
}

//Ethan
bool SyntaxAnalyzer::simpleexpr(vector<string> &tok, vector<string>::iterator &tokitr,
                                vector<string>::iterator &lexitr) {
    if (!term(tok, tokitr, lexitr)) {
        return false;
    }
    if (tokitr != tok.end()) {
        if (relop(tok, tokitr, lexitr)) {
            return true;
        } else if (arithop(tok, tokitr, lexitr)) {
            return true;
        }
    }
    return false;
}


//David LL
bool SyntaxAnalyzer::assignstmt(vector<string> &tok, vector<string>::iterator &tokitr,
                                vector<string>::iterator &lexitr) {
    bool valid = false;
    if (tokitr != tok.end() && *tokitr == "t_id") {
        ++tokitr;
        ++lexitr;
        if (tokitr != tok.end() && *tokitr == "t_eq") {
            ++tokitr;
            ++lexitr;
            if (expr(tok, tokitr, lexitr)) {
                if (tokitr != tok.end() && *tokitr == "t_semicolon") {
                    ++tokitr;
                    ++lexitr;
                    valid = true;
                }
            }
        }
    }
    return valid;
}

//Stmt still needs work, but I need the other methods for it to work.
//David G
int SyntaxAnalyzer::stmt(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    if (tokitr != tok.end()) {
        if (*tokitr == "t_if") {
            if (ifstmt(tok, tokitr, lexitr)) {
                return 1;
            } else { return 0; }
        } else if (*tokitr == "t_while") {
            if (whilestmt(tok, tokitr, lexitr)) {
                return 1;
            } else { return 0; }
        } else if (*tokitr == "t_id") {
            if (assignstmt(tok, tokitr, lexitr)) {
                return 1;
            } else { return 0; }
        } else if (*tokitr == "t_input") {
            if (inputstmt(tok, tokitr, lexitr)) {
                return 1;
            } else { return 0; }
        } else if (*tokitr == "t_output") {
            if (outputstmt(tok, tokitr, lexitr)) {
                return 1;
            } else { return 0; }
        } else if (*tokitr == "t_integer" || *tokitr == "t_string") {
            if (vdecassign(tok, tokitr, lexitr)) {
                return 1;
            } else { return 0; }
        }
        return 2;
    }
}


//Ethan, still needs work
bool SyntaxAnalyzer::stmtlist(vector<string> &tok, vector<string>::iterator &tokitr, vector<string>::iterator &lexitr) {
    if (tokitr != tok.end() && *tokitr == "s_rbrace") {
        return true;
    }
    if (!stmt(tok, tokitr, lexitr)) {
        return false;
    }
    while (tokitr != tok.end() && stmt(tok, tokitr, lexitr)) {
    }
    return true;
}

//David LL
bool SyntaxAnalyzer::whilestmt(vector<string> &tok, vector<string>::iterator &tokitr,
                               vector<string>::iterator &lexitr) {
    if (tokitr != tok.end() && *tokitr == "t_while") {
        ++tokitr;
        ++lexitr;

        if (tokitr != tok.end() && *tokitr == "t_lparen") {
            ++tokitr;
            ++lexitr;

            if (expr(tok, tokitr, lexitr)) {
                if (tokitr != tok.end() && *tokitr == "t_rparen") {
                    ++tokitr;
                    ++lexitr;

                    if (tokitr != tok.end() && *tokitr == "t_lbrace") {
                        ++tokitr;
                        ++lexitr;

                        if (stmtlist(tok, tokitr, lexitr)) {
                            if (tokitr != tok.end() && *tokitr == "t_rbrace") {
                                ++tokitr;
                                ++lexitr;
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

bool SyntaxAnalyzer::checkDeclaration(const string &varName) {
    if (symboltable.find(varName) == symboltable.end()) {
        cout << "ERROR: Variable '" << varName << "' used without declaration.\n";
        return false;
    }
    return true;
}


// void SyntaxAnalyzer::declareVariable(const string& varName) {
//     if (symboltable.find(varName) == symboltable.end()) {
//         symboltable[varName] = "unknown";
//     }
// }


SyntaxAnalyzer::SyntaxAnalyzer(istream &infile) {
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

bool SyntaxAnalyzer::parse() {
    if (tokitr != tokens.end() && *tokitr == "t_main") {
        ++tokitr;
        ++lexitr;
        if (tokitr != tokens.end() && *tokitr == "t_lbrace") {
            ++tokitr;
            ++lexitr;
            if (stmtlist(tokens, tokitr, lexitr)) {
                if (tokitr != tokens.end() && *tokitr == "t_rbrace") {
                    ++tokitr;
                    ++lexitr;
                    if (tokitr == tokens.end()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    vector<string> tokens = {"t_string"};
    vector<string> lexemes = {"int"};
    vector<string>::iterator tokitr;
    vector<string>::iterator lexitr;
    tokitr = tokens.begin();
    lexitr = lexemes.begin();
    SyntaxAnalyzer analyzer();
    while (tokitr != tokens.end()) {
        if (analyzer.stmt(tokens, tokitr, lexitr)) {
            cout << "Works" << endl;
        } else {
            cout << "Dosnt work" << endl;
            break;
        }
    }
}
