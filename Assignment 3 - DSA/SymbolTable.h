#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

enum class State{nil, available};
enum class Method{LINEAR, QUADRATIC, DOUBLE};

class SymbolTable {
    //******************************************************
    //CLASS SYMBOL******************************************
    //******************************************************
public:
    class Symbol {
    public:
        string id;
        string type;
        string value;
        int block;
        State state;
    public:
        Symbol() {
            this->id = "";
            this->type = "";
            this->value = "";
            this->block = 0;
            this->state = State::nil;
        }
    };
private:
    Symbol* arr;
    int size;
    int cBlock;
    Method metd;
    int c1;
    int c2;
public:
    //******************************************************
    //CONSTRUCTOR AND SUPPORT FUNCTIONS*********************
    //******************************************************
    SymbolTable() {
        this->arr = new Symbol[10];
        this->size = 0;
        this->cBlock = 0;
        this->metd = Method::LINEAR;
        this->c1 = 0;
        this->c2 = 0;
    }
    void run(string filename);
    int h1(int k, int m) {
        return k % m;
    }
    int h2(int k, int m) {
        return (k % (m - 2)) + 1;
    }
    int Linear(int m, int c, int k, int i) {
        return (h1(k, m) + c * i) % m;
    }
    int Quadratic(int m, int c1, int c2, int k, int i) {
        return (h1(k, m) + c1 * i + c2 * i * i) % m;
    }
    int Double(int m, int c, int k, int i) {
        return (h1(k, m) + c * i * h2(k, m)) % m;
    }
    int countComma(string s) {
        int ans = 0;
        for (unsigned int i = 0; i < s.size(); i++)
            if (s[i] == ',') 
                ans++;
        return ans;
    }
    int countSpace(string s) {
        int ans = 0;
        bool done = true;
        for (unsigned int i = 0; i < s.size(); i++) {
            if (s[i] == '(') done = false;
            if (s[i] == ')') done = true;
            if (s[i] == ' ')
                if (done == true) 
                    ans++;
        }
        return ans;
    }
    void divideString(string s, string& code, string& name, string& val1, string& val2) {
        int space = countSpace(s), pos = 0;
        bool done = true;
        while (s[pos] != ' ' && s[pos] != '\n') {
            code += s[pos];
            pos++;
            if ((unsigned int)pos > s.size() - 1) break;
        }
        if (space >= 1) {
            pos = 0;
            s.erase(0, code.size() + 1);
            while (s[pos] != ' ' && s[pos] != '\n') {
                name += s[pos];
                pos++;
                if ((unsigned int)pos > s.size() - 1) break;
            }
        }
        if (space >= 2) {
            pos = 0;
            s.erase(0, name.size() + 1);
            while (s[pos] != '\n') {
                if (s[pos] == ' ')
                    if (done == true) break;
                if (s[pos] == '(') done = false;
                if (s[pos] == ')') done = true;
                val1 += s[pos];
                pos++;
                if ((unsigned int)pos > s.size() - 1) break;
            }
        }
        if (space >= 3) {
            pos = 0;
            s.erase(0, val1.size() + 1);
            while (s[pos] != ' ' && s[pos] != '\n') {
                val2 += s[pos];
                pos++;
                if ((unsigned int)pos > s.size() - 1) break;
            }
        }
    }
    int findKey(int c, string s) {
        int ans = c;
        for (unsigned int i = 0; i < s.size(); i++) {
            int g = (int)s[i] - 48;
            if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || s[i] == '_')
                ans = ans * 100 + g;
            else 
                ans = ans * 10 + g;
        }
        return ans;
    }
    //******************************************************
    //BASIC METHODS OF HASH TABLE***************************
    //******************************************************
    int h(int k, int i) {
        if (this->metd == Method::LINEAR)
            return Linear(this->size, this->c1, k, i);
        if (this->metd == Method::QUADRATIC)
            return Quadratic(this->size, this->c1, this->c2, k, i);
        if (this->metd == Method::DOUBLE)
            return Double(this->size, this->c1, k, i);
    }
    bool insert(int key, string name, int& slot, int& pos) {
        for (int i = 0; i < this->size; i++) {
            int j = h(key, i);
            if (this->arr[j].state == State::nil) {
                this->arr[j].id = name;
                this->arr[j].state = State::available;
                pos = j;
                return true;
            }
            slot++;
        }
        return false;
    }
    int search(int key, string name, int& slot) {
        for (int i = 0; i < this->size; i++) {
            int j = h(key, i);
            if (this->arr[j].state == State::available)
                if (this->arr[j].id == name)
                    return j;
            if (this->arr[j].state == State::nil)
                return -1;
            slot++;
        }
        return -1;
    }
    void removeBlock(int gBlock) {
        for (unsigned int i = 0; i < this->size; i++) {
            if (arr[i].block == gBlock) {
                arr[i].block = 0;
                arr[i].id = "";
                arr[i].state = State::nil;
                arr[i].type = "";
                arr[i].value = "";
            }
        }
    }
    bool contain(int gBlock, string name) {
        int key = findKey(gBlock, name), slot = 0;
        int pos = SymbolTable::search(key, name, slot);
        if (pos == -1)
            return false;
        return true;
    }
    bool contain(int gBlock, string name, int& slot, int& pos) {
        int key = findKey(gBlock, name);
        pos = SymbolTable::search(key, name, slot);
        if (pos == -1)
            return false;
        return true;
    }
    //******************************************************
    //REQUIRED FUNCTIONS************************************
    //******************************************************
    //CHECK NAME
    bool checkName(string s) {
        if (s[0] < 'a' || s[0] > 'z') return false;
        for (unsigned int i = 0; i < s.size(); i++)
            if ((s[i] < 'a' || s[i] > 'z') && (s[i] < 'A' || s[i] > 'Z') && (s[i] < '0' || s[i] > '9') && s[i] != '_')
                return false;
        return true;
    }
    //CHECK NUMBER
    bool checkNumber(string typ) {
        for (unsigned int i = 0; i < typ.size(); i++)
            if (typ[i] < '0' || typ[i] > '9')
                return false;
        return true;
    }
    //CHECK STRING
    bool checkString(string typ) {
        if (typ[0] != char(39) || typ[typ.size() - 1] != char(39)) return false;
        for (unsigned int i = 1; i < typ.size() - 1; i++)
            if ((typ[i] < '0' || typ[i] > '9') && (typ[i] < 'a' || typ[i] > 'z') && (typ[i] < 'A' || typ[i] > 'Z') && typ[i] != ' ')
                return false;
        return true;
    }
    //CHECK THE CALL OF FUNCTION
    bool checkCallFunc(string s) {
        int a = 0, b = 0;
        bool done = false;
        string name = "";
        for (unsigned int i = 0; i < s.size(); i++) {
            if (s[i] == '(') {
                a++;
                done = true;
            }
            else if (s[i] == ')') {
                if (done == false)
                    return false;
                b++;
            }
            if (done == false)
                name += s[i];
        }
        if (a != 1 || b != 1)
            return false;
        if (checkName(name) == false)
            return false;
        s.erase(0, name.size() + 1);
        while (s[0] != ')') {
            int k = 0;
            string para = "";
            while (s[k] != ')' && s[k] != ',') {
                para += s[k];
                k++;
            }
            if (checkNumber(para) == false && checkString(para) == false && checkName(para) == false)
                return false;
            s.erase(0, para.size());
            if (s[0] == ',')
                s.erase(0, 1);
        }
        return true;
    }
    //CHECK THE INSTRUCION OF LINE
    void checkInstruction(string s) {
        int space = countSpace(s);
        string code, name, val1, val2;
        divideString(s, code, name, val1, val2);
        //CHECK CODE
        if (code != "LINEAR" && code != "QUADRATIC" && code != "DOUBLE" && code != "INSERT" && code != "ASSIGN" && code != "CALL" && code != "BEGIN" && code != "END" && code != "LOOKUP" && code != "PRINT")
            throw InvalidInstruction(s);
        //CHECK NAME
        if (space >= 1) {
            if (code != "LINEAR" && code != "QUADRATIC" && code != "DOUBLE" && code != "INSERT" && code != "ASSIGN" && code != "CALL" && code != "LOOKUP")
                throw InvalidInstruction(s);
            else if (code == "LINEAR" || code == "QUADRATIC" || code == "DOUBLE") {
                if (name.size() > 6)
                    throw InvalidInstruction(s);
                if (checkNumber(name) == false)
                    throw InvalidInstruction(s);
            }
            else if (code == "INSERT" || code == "ASSIGN" || code == "LOOKUP") {
                if (checkName(name) == false)
                    throw InvalidInstruction(s);
            }
            else {
                if (checkCallFunc(name) == false)
                    throw InvalidInstruction(s);
            }
        }
        //CHECK VAL1
        if (space >= 2) {
            if (code != "LINEAR" && code != "QUADRATIC" && code != "DOUBLE" && code != "INSERT" && code != "ASSIGN")
                throw InvalidInstruction(s);
            else if (code == "LINEAR" || code == "QUADRATIC" || code == "DOUBLE") {
                if (val1.size() > 6)
                    throw InvalidInstruction(s);
                if (checkNumber(val1) == false)
                    throw InvalidInstruction(s);
            }
            else if (code == "INSERT") {
                if (checkNumber(val1) == false)
                    throw InvalidInstruction(s);
            }
            else {
                if (checkNumber(val1) == false && checkString(val1) == false && checkName(val1) == false && checkCallFunc(val1) == false)
                    throw InvalidInstruction(s);
            }
        }
        //CHECK VAL2
        if (space >= 3) {
            if (code != "QUADRATIC" || checkNumber(val2) == false)
                throw InvalidInstruction(s);
            if (val2.size() > 6)
                throw InvalidInstruction(s);
        }
    }
    //CHOOSE METHOD
    void SymbolMethod(string s) {
        string code, name, val1, val2;
        divideString(s, code, name, val1, val2);
        this->size = stoi(name);
        if (code == "LINEAR") {
            this->c1 = stoi(val1);
            this->metd = Method::LINEAR;
        }
        else if (code == "QUADRATIC") {
            this->c1 = stoi(val1);
            this->c2 = stoi(val2);
            this->metd = Method::QUADRATIC;
        }
        else {
            this->c1 = stoi(val1);
            this->metd = Method::DOUBLE;
        }
        delete[] this->arr;
        this->arr = new Symbol[this->size];
    }
    //INSERT
    void SymbolInsert(string s) {
        int space = countSpace(s), slot = 0, pos = -1;
        string code, name, val1, val2;
        divideString(s, code, name, val1, val2);
        val2 = val1;
        //VARIABLE
        if (space == 1) {
            if (contain(this->cBlock, name) == true)
                throw Redeclared(s);
            int key = findKey(this->cBlock, name);
            bool done = SymbolTable::insert(key, name, slot, pos);
            if (done == false)
                throw Overflow(s);
            arr[pos].block = this->cBlock;
        }
        //FUNCTION
        else {
            if (contain(this->cBlock, name) == true)
                throw Redeclared(s);
            if (this->cBlock != 0)
                throw InvalidDeclaration(s);
            int key = findKey(0, name);
            bool done = SymbolTable::insert(key, name, slot, pos);
            if (done == false)
                throw Overflow(s);
            arr[pos].block = 0;
            arr[pos].value = "_" + val1;
        }
        cout << slot;
    }
    //ASSIGN
    void SymbolAssign(string s) {
        int slot = 0;
        string code, name, val1, val2;
        divideString(s, code, name, val1, val2);
        //GÁN BIẾN VÀ HÀM
        if ((val1[0] < '0' || val1[0] > '9') && val1[0] != char(39)) {
            int isFunc = 0;
            string str = "";
            for (unsigned int i = 0; i < val1.size(); i++) {
                if (val1[i] == '(') {
                    isFunc++;
                    break;
                }
                str += val1[i];
            }
            //GÁN HÀM
            if (isFunc == 1) {
                //KIỂM TRA SỰ TỒN TẠI CỦA HÀM
                int gBlock = this->cBlock, pos1 = 0, pos2 = 0;
                while (gBlock >= 0) {
                    if (contain(gBlock, str, slot, pos2) == true)
                        break;
                    gBlock--;
                }
                if (gBlock < 0)
                    throw Undeclared(s);
                //type: number,string,number
                //value: _number2
                //KIỂM TRA SỰ TỒN TẠI CỦA BIẾN
                gBlock = this->cBlock;
                while (gBlock >= 0) {
                    if (contain(gBlock, name, slot, pos1) == true)
                        break;
                    gBlock--;
                }
                if (gBlock < 0)
                    throw Undeclared(s);
                //KIỂM TRA SỰ PHÙ HỢP CỦA HÀM
                //Kiểm tra xem có phải hàm không
                if (arr[pos2].value[0] != '_')
                    throw TypeMismatch(s);
                //Kiểm tra số lượng tham số truyền vào
                string sav = arr[pos2].value;
                if (sav[1] >= '0' && sav[1] <= '9')
                    sav.erase(0, 1);
                else if (sav[1] == 'n' || sav[1] == 's') //number and string
                    sav.erase(0, 7);
                else if (sav[1] == 'v') //void
                    sav.erase(0, 5);
                int num_para = stoi(sav), comma = countComma(val1);
                if (num_para != comma + 1)
                    throw TypeMismatch(s);
                //Kiểm tra sự phù hợp của tham số
                val1.erase(0, str.size() + 1);
                //val1 = string,number,asf)
                if (arr[pos2].type == "") {
                    bool donev = false;
                    while (val1[0] != ')') {
                        if (donev == true)
                            arr[pos2].type += ",";
                        int k = 0;
                        string r = "";
                        while (val1[k] != ',' && val1[k] != ')') {
                            r += val1[k];
                            k++;
                        }
                        if (checkNumber(r) == true)
                            arr[pos2].type += "number";
                        else if (checkString(r) == true)
                            arr[pos2].type += "string";
                        else {
                            int dBlock = this->cBlock, pos3 = 0;
                            while (dBlock >= 0) {
                                if (contain(dBlock, r, slot, pos3) == true)
                                    break;
                                dBlock--;
                            }
                            if (dBlock < 0)
                                throw Undeclared(s);
                            if (arr[pos3].type == "")
                                throw TypeCannotBeInferred(s);
                            if (arr[pos3].type != "string" && arr[pos3].type != "number")
                                throw TypeMismatch(s);
                            arr[pos2].type += arr[pos3].type;
                        }
                        val1.erase(r.size());
                        if (val1[0] == ',')
                            val1.erase(0, 1);
                        donev = true;
                    }
                }
                else {
                    string maintype = arr[pos2].type;
                    while (val1[0] != ')') {
                        string r1 = "", r2 = "";
                        int k1 = 0, k2 = 0;
                        while (maintype[k1] != ',' && maintype[k1] != ')') {
                            r1 += maintype[k1];
                            k1++;
                        }
                        while (val1[k2] != ',' && val1[k2] != ')') {
                            r2 += maintype[k2];
                            k2++;
                        }
                        if (checkNumber(r2) == true) {
                            if (r1 != "number")
                                throw TypeMismatch(s);
                        }
                        else if (checkString(r2) == true) {
                            if (r1 != "string")
                                throw TypeMismatch(s);
                        }
                        else {
                            int dBlock = this->cBlock, pos3 = 0;
                            while (dBlock >= 0) {
                                if (contain(dBlock, r2, slot, pos3) == true)
                                    break;
                                dBlock--;
                            }
                            if (dBlock < 0)
                                throw Undeclared(s);
                            if (arr[pos3].type == "")
                                arr[pos3].type = r1;
                            else if (arr[pos3].type != r1)
                                throw TypeMismatch(s);
                        }
                        val1.erase(r2.size());
                        if (val1[0] == ',')
                            val1.erase(0, 1);
                        maintype.erase(r1.size());
                        if (maintype[0] == ',')
                            maintype.erase(0, 1);
                    }
                }
                //Đã kiểm tra xong sự phù hợp của tham số
                //KIỂM TRA SỰ PHÙ HỢP CỦA BIẾN
                //Kiểm tra xem có phải hàm không
                if (arr[pos1].value[0] == '_')
                    throw TypeMismatch(s);
                //GÁN VÀ SUY DIỄN
                //Cả 2 đều chưa có kiểu
                if (arr[pos1].type == "" && (arr[pos2].value[1] != 'n' && arr[pos2].value[1] != 's'))
                    throw TypeCannotBeInferred(s);
                //Nếu biến có kiểu
                else if (arr[pos1].type != "" && (arr[pos2].value[1] != 'n' && arr[pos2].value[1] != 's')) {
                    if (arr[pos2].value[1] == 'v')
                        throw TypeMismatch(s);
                    arr[pos2].value.erase(0, 1);
                    arr[pos2].value = "_" + arr[pos1].type + arr[pos2].value;
                    arr[pos1].value = val2;
                }
                //Nếu hàm có kiểu
                else if (arr[pos1].type == "" && (arr[pos2].value[1] == 'n' || arr[pos2].value[1] == 's')) {
                    if (arr[pos2].value[1] == 'n')
                        arr[pos1].type = "number";
                    else if (arr[pos2].value[1] == 's')
                        arr[pos1].type = "string";
                    arr[pos1].value = val2;
                }
                //Nếu cả 2 đều đã có kiểu
                else if (arr[pos1].type != "" && (arr[pos2].value[1] == 'n' || arr[pos2].value[1] == 's')) {
                    if (arr[pos2].value[1] == 'n') {
                        if (arr[pos1].type != "number")
                            throw TypeMismatch(s);
                    }
                    else {
                        if (arr[pos1].type != "string")
                            throw TypeMismatch(s);
                    }
                    arr[pos1].value = val2;
                }
            }
            //GÁN BIẾN
            else if (isFunc == 0) {
                //KIỂM TRA SỰ TỒN TẠI CỦA BIẾN 2
                int gBlock = this->cBlock, pos1 = 0, pos2 = 0;
                while (gBlock >= 0) {
                    if (contain(gBlock, str, slot, pos2) == true)
                        break;
                    gBlock--;
                }
                if (gBlock < 0)
                    throw Undeclared(s);
                //KIỂM TRA SỰ TỒN TẠI CỦA BIẾN 1
                gBlock = this->cBlock;
                while (gBlock >= 0) {
                    if (contain(gBlock, name, slot, pos1) == true)
                        break;
                    gBlock--;
                }
                if (gBlock < 0)
                    throw Undeclared(s);
                //NẾU CẢ HAI BIẾN ĐỀU CHƯA CÓ KIỂU
                if (arr[pos1].type == "" && arr[pos2].type == "")
                    throw TypeCannotBeInferred(s);
                //NẾU BIẾN 1 CÓ KIỂU
                else if (arr[pos1].type != "" && arr[pos2].type == "")
                    arr[pos2].type = arr[pos1].type;
                //NẾU BIẾN 2 CÓ KIỂU
                else if (arr[pos1].type == "" && arr[pos2].type != "")
                    arr[pos1].type = arr[pos2].type;
                //NẾU CẢ 2 BIẾN ĐỀU CÓ KIỂU
                else {
                    if (arr[pos1].type != arr[pos2].type)
                        throw TypeMismatch(s);
                }
            }
        }
        //GÁN HẰNG
        else {
            //KIỂM TRA SỰ TỒN TẠI CỦA BIẾN
            int gBlock = this->cBlock, pos = 0;
            while (gBlock >= 0) {
                if (contain(gBlock, name, slot, pos) == true)
                    break;
                gBlock--;
            }
            if (gBlock < 0)
                throw Undeclared(s);
            //HẰNG SỐ
            if (checkNumber(val1) == true) {
                if (arr[pos].type == "") {
                    arr[pos].type = "number";
                    arr[pos].value = val1;
                }
                else if (arr[pos].type == "number") 
                    arr[pos].value = val1;
                else
                    throw TypeMismatch(s);
            }
            //HẰNG CHUỖI
            else if (checkString(val1) == true) {
                if (arr[pos].type == "") {
                    arr[pos].type == "string";
                    arr[pos].value = val1;
                }
                else if (arr[pos].type == "string") 
                    arr[pos].value = val1;
                else
                    throw TypeMismatch(s);
            }
        }
        cout << slot;
    }
    //CALL
    void SymbolCall(string s) {
        int slot = 0;
        string code, name, val1, val2;
        divideString(s, code, name, val1, val2);
        string str = "";
        int k = 0;
        while (name[k] != '(') {
            str += name[k];
            k++;
        }
        name.erase(str.size() + 1);
        //KIỂM TRA SỰ TỒN TẠI CỦA HÀM
        int pos = 0;
        if (contain(0, str, slot, pos) == false)
            throw Undeclared(s);
        //KIỂM TRA KIỂU TRẢ VỀ
        if (arr[pos].value[1] != 'v') {
            if (arr[pos].value[1] >= '0' && arr[pos].value[1] <= '9') {
                arr[pos].value.erase(0, 1);
                arr[pos].value = "_void" + arr[pos].value;
            }
            else
                throw TypeMismatch(s);
        }
        //KIỂM TRA SỐ LƯỢNG THAM SỐ
        string sav = arr[pos].value;
        sav.erase(0, 5);
        int num_para = stoi(sav), comma = countComma(name);
        if (num_para != comma + 1)
            throw TypeMismatch(s);
        //KIỂM TRA SỰ PHÙ HỢP CỦA THAM SỐ
        string typ = arr[pos].type;
        if (typ == "") {
            bool done = false;
            while (name[0] != ')') {
                if (done == true)
                    arr[pos].type += ",";
                int i = 0;
                string r = "";
                while (name[i] != ',' && name[i] != ')') {
                    r += name[i];
                    i++;
                }
                if (checkNumber(r) == true)
                    arr[pos].type += "number";
                else if (checkString(r) == true)
                    arr[pos].type += "string";
                else {
                    int dBlock = this->cBlock, pos3 = 0;
                    while (dBlock >= 0) {
                        if (contain(dBlock, r, slot, pos3) == true)
                            break;
                        dBlock--;
                    }
                    if (dBlock < 0)
                        throw Undeclared(s);
                    if (arr[pos3].type == "")
                        throw TypeCannotBeInferred(s);
                    if (arr[pos3].type != "number" && arr[pos3].type != "string")
                        throw TypeMismatch(s);
                    arr[pos].type += arr[pos3].type;
                }
                name.erase(0, r.size());
                if (name[0] == ',')
                    name.erase(0, 1);
                done = true;
            }
        }
        else {
            while (name[0] != ')') {
                int i = 0, j = 0;
                string r1 = "", r2 = "";
                while (name[i] != ',' && name[i] != ')') {
                    r1 += name[i];
                    i++;
                }
                while (typ[j] != ',' && typ[j] != ')') {
                    r2 += typ[j];
                    j++;
                }
                if (checkNumber(r1) == true) {
                    if (r2 != "number")
                        throw TypeMismatch(s);
                }
                else if (checkString(r1) == true) {
                    if (r2 != "string")
                        throw TypeMismatch(s);
                }
                else {
                    int dBlock = this->cBlock, pos1 = 0;
                    while (dBlock >= 0) {
                        if (contain(dBlock, r1, slot, pos1) == true)
                            break;
                        dBlock--;
                    }
                    if (dBlock < 0)
                        throw Undeclared(s);
                    if (arr[pos1].type == "")
                        arr[pos1].type = r2;
                    else if (arr[pos1].type != r2)
                        throw TypeMismatch(s);
                }
                name.erase(0, r1.size());
                if (name[0] == ',')
                    name.erase(0, 1);
                typ.erase(0, r2.size());
                if (typ[0] == ',')
                    typ.erase(0, 1);
            }
        }
        cout << slot;
    }
    //BEGIN
    void SymbolBegin(string s) {
        this->cBlock++;
    }
    //END
    void SymbolEnd(string s) {
        if (this->cBlock == 0)
            throw UnknownBlock();
        SymbolTable::removeBlock(this->cBlock);
        this->cBlock--;
    }
    //LOOKUP
    void SymbolLookup(string s) {
        string code, name, val1, val2;
        divideString(s, code, name, val1, val2);
        int gBlock = this->cBlock, slot = 0, pos = 0;
        while (gBlock >= 0) {
            if (contain(gBlock, name, slot, pos) == true)
                break;
            gBlock--;
        }
        if (gBlock < 0)
            throw Undeclared(s);
        cout << pos;
    }
    //PRINT
    void SymbolPrint(string s) {
        bool done = false;
        for (int i = 0; i < this->size; i++) {
            if (done == true)
                cout << ";";
            if (arr[i].state == State::available) {
                cout << i << " " << arr[i].id << "//" << arr[i].block;
                done = true;
            }
        }
    }
    void CheckClose() {
        if (this->cBlock != 0)
            throw UnclosedBlock(this->cBlock);
    }
    //******************************************************
    //DESTRUCTOR********************************************
    //******************************************************
    ~SymbolTable() {
        delete[] this->arr;
    }
    //////////////////

};
#endif