//
// Created by David Llamas on 4/1/25.
//
#include "SyntaxAnalyzer.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool SyntaxAnalyzer::parse() {
    if (stmtlist(tokens, lexemes, tokitr, lexitr) && tokitr == tokens.end()) {
        return true;
    } else {
        cout << "SYNTAX ERROR near ";
        if (tokitr != tokens.end() && lexitr != lexemes.end()) {
            cout << "[" << *tokitr << " : " << *lexitr << "]" << endl;
        } else {
            cout << "EOF" << endl;
        }
        return false;
    }
}
bool SyntaxAnalyzer::expr(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    // EXPR → TERM [LOGICOP TERM]
    if (term(tok, lex, tokitr, lexitr)) {
        // Optional LOGICOP TERM
        if (tokitr != tok.end() && (*tokitr == "t_and" || *tokitr == "t_or")) {
            ++tokitr;++lexitr; // consume logicop
            if (!term(tok, lex, tokitr, lexitr)) {
                return false; // expected another term
            }
        }
        return true;
    }
    return false;
}

bool SyntaxAnalyzer::stmtlist(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    bool foundStmt = true;
    while (tokitr != tok.end() && foundStmt) {
        foundStmt = false;
        if (whilestmt(tok, lex, tokitr, lexitr)) {
            foundStmt = true;
        } else {
            if (assignstmt(tok, lex, tokitr, lexitr)) {
                foundStmt = true;
            } else {
                if (inputstmt(tok, lex, tokitr, lexitr)) {
                    foundStmt = true;
                }
            }
        }
    }
    return true;
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

bool SyntaxAnalyzer::whilestmt(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    if (tokitr != tok.end() && *tokitr == "t_while") {
        ++tokitr;
        ++lexitr;

        if (tokitr != tok.end() && *tokitr == "t_lparen") {
            ++tokitr;
            ++lexitr;

            if (expr(tok, lex, tokitr, lexitr)) {
                if (tokitr != tok.end() && *tokitr == "t_rparen") {
                    ++tokitr;
                    ++lexitr;

                    if (tokitr != tok.end() && *tokitr == "t_lbrace") {
                        ++tokitr;
                        ++lexitr;

                        if (stmtlist(tok, lex, tokitr, lexitr)) {
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


bool SyntaxAnalyzer::assignstmt(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    bool valid = false;
    if (tokitr != tok.end() && *tokitr == "t_id") {
        ++tokitr; ++lexitr;
        if (tokitr != tok.end() && *tokitr == "t_eq") {
            ++tokitr; ++lexitr;
            if (expr(tok, lex, tokitr, lexitr)) {
                if (tokitr != tok.end() && *tokitr == "t_semicolon") {
                    ++tokitr; ++lexitr;
                    valid = true;
                }
            }
        }
    }
    return valid;
}

bool SyntaxAnalyzer::inputstmt(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    bool valid = false;
    if (tokitr != tok.end() && *tokitr == "t_input") {
        ++tokitr; ++lexitr;
        if (tokitr != tok.end() && *tokitr == "t_lparen") {
            ++tokitr; ++lexitr;
            if (tokitr != tok.end() && *tokitr == "t_id") {
                ++tokitr; ++lexitr;
                if (tokitr != tok.end() && *tokitr == "t_rparen") {
                    ++tokitr; ++lexitr;
                    valid = true;
                }
            }
        }
    }
    return valid;
}

bool SyntaxAnalyzer::term(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokitr, vector<string>::iterator& lexitr) {
    bool valid = false;
    if (tokitr != tok.end()) {
        if (*tokitr == "t_num" || *tokitr == "t_str" || *tokitr == "t_id") {
            ++tokitr;++lexitr;
            valid = true;
        } else if (*tokitr == "t_lparen") {
            ++tokitr;++lexitr;
            if (arithop(tok, lex, tokitr, lexitr) || term(tok, lex, tokitr, lexitr)) {
                if (tokitr != tok.end() && *tokitr == "t_rparen") {
                    ++tokitr;++lexitr;
                    valid = true;
                }
            }
        }
    }

    return valid;
}


bool SyntaxAnalyzer::arithop(vector<string>& tok, vector<string>& lex, vector<string>::iterator& tokIt, vector<string>::iterator& lexIt) {
    bool valid = false;
    if (tokIt != tok.end()) {
        if (*tokIt == "t_plus" || *tokIt == "t_minus" || *tokIt == "t_div") {
            ++tokIt; ++lexIt;
            valid = true;
        }
    }
    return valid;
}
int main() {
    ifstream infile("valid_input.txt");
    if (!infile) {
        cerr << "Error: Could not open 'valid_input.txt'\n";
        return 1;
    }

    SyntaxAnalyzer parser(infile);

    cout << "Running parser on 'valid_input.txt'...\n";
    if (parser.parse()) {
        cout << "Parsing succeeded ✅\n";
    } else {
        cout << "Parsing failed ❌\n";
    }

    return 0;
}



