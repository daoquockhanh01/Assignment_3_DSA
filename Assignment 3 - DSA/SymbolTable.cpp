#include "SymbolTable.h"

void SymbolTable::run(string filename) {
    ifstream ip;
    ip.open(filename, ios::in);
    while (!ip.eof()) {
        string s, code, name, val1, val2;
        getline(ip, s);
        checkInstruction(s);
        divideString(s, code, name, val1, val2);
        if (code == "INSERT") {
            SymbolTable::SymbolInsert(s);
            cout << endl;
        }
        else if (code == "ASSIGN") {
            SymbolTable::SymbolAssign(s);
            cout << endl;
        }
        else if (code == "CALL") {
            SymbolTable::SymbolCall(s);
            cout << endl;
        }
        else if (code == "BEGIN") {
            SymbolTable::SymbolBegin(s);
        }
        else if (code == "END") {
            SymbolTable::SymbolEnd(s);
        }
        else if (code == "LOOKUP") {
            SymbolTable::SymbolLookup(s);
            cout << endl;
        }
        else if (code == "PRINT") {
            SymbolTable::SymbolPrint(s);
        }
        else {
            SymbolTable::SymbolMethod(s);
        }
    }
    SymbolTable::CheckClose();
    ip.close();
}