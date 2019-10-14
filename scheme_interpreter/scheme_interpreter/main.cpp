#include <iostream>
#include <string>
#include "Mem.h"
#include "Hashtable.h"
#include "Stack.h"

using namespace std;

//#define MAX_TREE_SIZE 30
//#define HASH_SIZE 1031
#define LEFT_PAR -1
#define RIGHT_PAR -2
#define LAMBDA -18
#define DEFINE -36
#define PLUS -6
#define MINUS -29
#define PRODUCT -24
#define EQ -10
#define EQUAL -9
#define CAR -3
#define CDR -30
#define CONS -32
#define ELSE -34
#define COND -12
#define QUOTE -20
#define TRUE -16
#define FALSE -4
#define isNULL -14

int Eval(int);
string getNextToken(string);

static Mem memory[MAX_TREE_SIZE + 1];
static Hashtable hashtable[HASH_SIZE];
static Stack stack;
static bool flag[MAX_TREE_SIZE + 1];
static int i = 0, j = 0; // i는 현재 위치, j는 바로 전에 읽은 위치
static int freelist = 1;

int hashfunc(string key) {
    int value = 0;
    int len = key.size();
    for (int k = 0; k < len; ++k) {
        value += key[k] * key[k];
    }
    return value;
}

void hashtable_init() {
    hashtable[-TRUE].symbol = "#t";
    hashtable[-FALSE].symbol = "#f";
    hashtable[-LEFT_PAR].symbol = "(";
    hashtable[-RIGHT_PAR].symbol = ")";
    hashtable[-DEFINE].symbol = "define";
    hashtable[-PLUS].symbol = "+";
    hashtable[-MINUS].symbol = "-";
    hashtable[-PRODUCT].symbol = "*";
    hashtable[-LAMBDA].symbol = "lambda";
    hashtable[-CAR].symbol = "car";
    hashtable[-CDR].symbol = "cdr";
    hashtable[-CONS].symbol = "cons";
    hashtable[-COND].symbol = "cond";
    hashtable[-QUOTE].symbol = "quote";
    hashtable[-isNULL].symbol = "null?";
}

int getHashValue(string token) {
    /*hashtable[-LEFT_PAR].symbol = "(";
    hashtable[-RIGHT_PAR].symbol = ")";
    hashtable[-RIGHT_PAR].symbol = ")";
    */
    if (token == "(") {
        //hashtable[-LEFT_PAR].symbol = "(";
        return LEFT_PAR;
    }
    else if (token == ")") {
        //hashtable[-RIGHT_PAR].symbol = ")";
        return RIGHT_PAR;
    }
    else if (token == "define") {
        //hashtable[-DEFINE].symbol = "define";
        return DEFINE;
    }
    else if (token == "+") {
        //hashtable[-PLUS].symbol = "+";
        return PLUS;
    }
    else if (token == "-") {
        //hashtable[-MINUS].symbol = "-";
        return MINUS;
    }
    else if (token == "*") {
        //hashtable[-PRODUCT].symbol = "*";
        return PRODUCT;
    }
    else if (token == "lambda") {
        //hashtable[-LAMBDA].symbol = "lambda";
        return LAMBDA;
    }
    else if (token == "eq?") {
        //hashtable[-EQ].symbol = "eq?";
        return EQ;
    }
    else if (token == "equal?") {
        //hashtable[-EQUAL].symbol = "equal?";
        return EQUAL;
    }
    else if (token == "car") {
        //hashtable[-CAR].symbol = "car";
        return CAR;
    }
    else if (token == "cdr") {
        //hashtable[-CDR].symbol = "cdr";
        return CDR;
    }
    else if (token == "cons") {
        //hashtable[-CONS].symbol = "cons";
        return CONS;
    }
    else if (token == "else") {
        //hashtable[-ELSE].symbol = "else";
        return ELSE;
    }
    else if (token == "cond") {
        ///hashtable[-COND].symbol = "cond";
        return COND;
    }
    else if (token == "quote") {
        //hashtable[-QUOTE].symbol = "quote";
        return QUOTE;
    }
    else if (token == "null?") {
        //hashtable[-isNULL].symbol = "null?";
        return isNULL;
    }
    int tmp = hashfunc(token);
    int value = tmp % HASH_SIZE;
    if (hashtable[value].symbol != "0" && hashtable[value].symbol == token)
        return -value;
    else if (hashtable[value].symbol != "0") {
        cout << "collision!";
        exit(1);
    }
    else {
        hashtable[value].symbol = token;
    }
    return -value;
}


string Concatenate(string s1, string s2) {
    return s1 + " " + s2;
}

string Preprocessing(string input) {
    string newcommand = "";
    string token;
    while ((token = getNextToken(input)) != "\0") {
        if (token == "define") {
            newcommand = Concatenate(newcommand, "define");
            token = getNextToken(input);
            if (token == "(") {
                token = getNextToken(input);
                newcommand = Concatenate(newcommand, token);
                newcommand = Concatenate(newcommand, "(lambda (");
                newcommand = Concatenate(newcommand, Preprocessing(input));
                newcommand = Concatenate(newcommand, ")");
            }
            else {
                i = j;
            }
        }
        else if (token == "'") {
            newcommand = Concatenate(newcommand, "(quote");
            int left = 0;
            do {
                token = getNextToken(input);
                newcommand = Concatenate(newcommand, token);
                if (token == "(")
                    left++;
                else if (token == ")")
                    left--;
            } while (left > 0);
            newcommand = Concatenate(newcommand, ")");
        }
        else newcommand = Concatenate(newcommand, token);
    }
    return newcommand;
}

string getNextToken(string input) {
    if (i == 0 && input[i] != '(' && input[i] != '\'') {
        i = input.size();
        return input;
    }
    else if (i == input.size()) {
        return "\0";
    }
    else {
        while (input[i] == ' ') ++i;
        if (i == input.size()) return "\0";
        else if (input[i] == '(' || input[i] == ')') {
            j = i;
            i++;
            return (input[j] == '(') ? "(" : ")";
        }
        else if (input[i] == '\'') {
            j = i;
            i++;
            return "'";
        }
        else {
            j = i;
            string tmp = "";
            while (input[i] != ' ' && input[i] != ')' && input[i] != '(') {
                tmp += input[i];
                ++i;
            }
            return tmp;
        }
    }

}

string getSymbol(int value) {
    return hashtable[-value].symbol;
}

int getLink(int value) {
    return hashtable[-value].link;
}

void Print(int index, bool startlist) {
    if (index == NULL)
        cout << "()";
    else if (index < 0)
        cout << getSymbol(index) << " ";
    else if (index > 0) {
        if (startlist == true)
            cout << "( ";
        Print(memory[index].lchild, true);
        if (memory[index].rchild != NULL)
            Print(memory[index].rchild, false);
        else
            cout << ")";
    }
}

int Read(string input) {
    int root = NULL;
    bool first = true;
    int value = getHashValue(getNextToken(input));
    int temp = NULL;
    if (value == LEFT_PAR) {
        while ((value = getHashValue(getNextToken(input))) != RIGHT_PAR) {
            if (first == true) {
                if (freelist == MAX_TREE_SIZE)
                    throw out_of_range("Tree is full");
                temp = freelist;
                freelist = memory[freelist].rchild;
                root = temp;
                first = false;
            }
            else {
                if (freelist == MAX_TREE_SIZE)
                    throw out_of_range("Tree is full");
                memory[temp].rchild = freelist;
                freelist = memory[freelist].rchild;
                temp = memory[temp].rchild;
            }
            if (value == LEFT_PAR) {
                i = j;
                //freelist--;
                memory[temp].lchild = Read(input);
            }
            else {
                memory[temp].lchild = value;
            }
        }
        memory[temp].rchild = NULL;
        return root;
    }
    else return value;
}

void init_mem(int root, int& freelist) {
    if (root == NULL)
        return;
    if (memory[root].lchild > 0)
        init_mem(memory[root].lchild, freelist);
    init_mem(memory[root].rchild, freelist);
    memory[root].lchild = NULL;
    memory[root].rchild = freelist;
    freelist = root;
    //cout << root << " ";
}

void lower(string& input) {
    for (int k = 0; k < input.size(); ++k) {
        if (input[k] >= 'A' && input[k] <= 'Z')
            input[k] += 32;
    }
}

void printTree() {
    cout << "Tree Array" << endl;
    cout << "node, left child, right child, flag" << endl;
    for (int k = 1; k <= MAX_TREE_SIZE; ++k) {
        cout << k << "\t" << memory[k].lchild << "\t" << memory[k].rchild << "\t" << flag[k] << endl;
    }
}

void setLink(int index, int link)
{
    if (index < 0)
        hashtable[-index].link = link;
}

int getArg1(int index) {
    return Eval(memory[memory[index].rchild].lchild);
}
int getArg2(int index) {
    return Eval(memory[memory[memory[index].rchild].rchild].lchild);
}

int bool_check(bool check) {
    if (check == true)
        return TRUE;
    else
        return FALSE;
}

void pushToStack(int index) {
    //int func_node = getLink(memory[index].lchild);
    cout << "push called" << endl;
    int arg_node = memory[memory[Eval(memory[index].lchild)].rchild].lchild;
    int arg = memory[index].rchild;
    int arguments[100];
    int i = 0;
    /*
    arguments[i++] = Eval(memory[arg].lchild);
    arg = memory[arg].rchild;
    */
    while (arg != NULL) {
        arguments[i++] = Eval(memory[arg].lchild);
        arg = memory[arg].rchild;
    }

    i = 0;
    /*
    stack.push(getLink(memory[arg_node].lchild));
    setLink(memory[arg_node].lchild, arguments[i++]);
    arg_node = memory[arg_node].rchild;
    */
    while (arg_node != NULL) {
        stack.push(make_pair(memory[arg_node].lchild,getLink(memory[arg_node].lchild)));
        setLink(memory[arg_node].lchild, arguments[i++]);
        arg_node = memory[arg_node].rchild;
    }
    stack.print();
}

void popFromStack(int index) {
    cout << "pop called" << endl;
    //int func_node = getLink(memory[index].lchild);
    int arg_node = memory[memory[Eval(memory[index].lchild)].rchild].lchild;
    while (arg_node != NULL) {
        pair<int,int> oldlink = stack.top();
        stack.pop();
        setLink(oldlink.first, oldlink.second);
        arg_node = memory[arg_node].rchild;
    }
}

bool isNumber(int index) {
    return atoi(getSymbol(index).c_str()) != 0 || getSymbol(index).compare("0") == 0;
}

int Eval(int root) {
    if ((root < 0) && isNumber(root))
        return root;
    else if (root < 0)
        return getLink(root);
    switch (memory[root].lchild) {
    case DEFINE: {
         setLink(memory[memory[root].rchild].lchild, Eval(memory[memory[memory[root].rchild].rchild].lchild));
        return memory[memory[root].rchild].lchild;
    }
    case PLUS: {
        int arg1 = stoi(getSymbol(getArg1(root)));
        int arg2 = stoi(getSymbol(getArg2(root)));
        int sum = arg1 + arg2;
        return getHashValue(to_string(sum));
    }
    case MINUS: {
        int arg1 = stoi(getSymbol(getArg1(root)));
        int arg2 = stoi(getSymbol(getArg2(root)));
        int diff = arg1 - arg2;
        return getHashValue(to_string(diff));
    }
    case PRODUCT: {
        int arg1 = stoi(getSymbol(getArg1(root)));
        int arg2 = stoi(getSymbol(getArg2(root)));
        int prod = arg1 * arg2;
        return getHashValue(to_string(prod));
    }
    case LAMBDA:
        return root;
    case EQ: {
        return bool_check(Eval(memory[memory[root].rchild].lchild) == Eval(memory[memory[memory[root].rchild].rchild].lchild));
    }
    case CONS: {
        int newindex = freelist;
        freelist = memory[freelist].rchild;
        memory[newindex].lchild = Eval(memory[memory[root].rchild].lchild);
        memory[newindex].rchild = Eval(memory[memory[memory[root].rchild].rchild].lchild);
        return newindex;
    }
    case QUOTE: {
        return memory[memory[root].rchild].lchild;
    }
    case CAR: {
        return memory[Eval(memory[memory[root].rchild].lchild)].lchild;
    }
    case CDR: {
        return memory[Eval(memory[memory[root].rchild].lchild)].rchild;
    }
    case isNULL: {
        cout << "null?  " << memory[root].rchild << "    " << memory[Eval(memory[memory[root].rchild].lchild)].rchild << "    ";
        if (memory[root].rchild == NULL || Eval(memory[memory[root].rchild].lchild) == NULL) {
            cout << "true" << endl;
            return TRUE;
        }
        else {
            cout << "false" << endl;
            return FALSE;
        }
    }
    case COND: {
        while (memory[memory[root].rchild].rchild != NULL) {
            root = memory[root].rchild;
            if (Eval(memory[memory[root].lchild].lchild) == TRUE)
                return Eval(memory[memory[memory[root].lchild].rchild].lchild);
        }
        if (memory[memory[memory[root].rchild].lchild].lchild != ELSE) {
            cout << "There is no ELSE" << endl;
            exit(1);
        }
        return Eval(memory[memory[memory[memory[root].rchild].lchild].rchild].lchild);
    }
    default: {
        /*cout << "======================" << endl;
        cout << "--- Before push" << endl;
        cout << "list :  " << getLink(getHashValue("list")) << endl;
        stack.print();
        cout << "root : " << root << endl;*/
        pushToStack(root);
        
        /*cout << "--- After push" << endl;
        cout << "list :  " << getLink(getHashValue("list")) << endl;
        stack.print();*/
        int result = Eval(memory[memory[memory[Eval(memory[root].lchild)].rchild].rchild].lchild);
        /*cout << "--- After eval" << endl;
        cout << "Eval result: " << getSymbol(result) << " " << result << endl;
        cout << "--- Before pop" << endl;
        cout << "list :  " << getLink(getHashValue("list")) << endl;*/
        popFromStack(root);
        /*cout << "--- After pop" << endl;
        cout << "list :  " << getLink(getHashValue("list")) << endl;
        stack.print();
        cout << "------------------" << endl;*/
        return result;
    }
    }
    //return root;
}

string& ltrim(string& str, const string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

void init_flag() {
    for (int i = 0; i <= MAX_TREE_SIZE; ++i) {
        flag[i] = true;
    }
}

void flag_true(int root) {
    if (root == NULL)
        return;
    if (memory[root].lchild > 0)
        flag_true(memory[root].lchild);
    flag_true(memory[root].rchild);
    flag[root] = true;
    //cout << root << " ";
}
void garbageCollector() {
    for (int i = 0; i < HASH_SIZE; ++i) {
        if (hashtable[i].link > 0) {
            flag_true(hashtable[i].link);
        }
    }
    freelist = MAX_TREE_SIZE;
    memory[MAX_TREE_SIZE].rchild = 0;
    int prev = MAX_TREE_SIZE;
    for (int i = MAX_TREE_SIZE - 1; i > 0; --i) {
        if (flag[i] == false) {
            freelist = i;
            memory[i].rchild = prev;
            memory[i].lchild = NULL;
            prev = i;
        }
    }
}

int main() {
    for (int k = 1; k < MAX_TREE_SIZE; ++k) {
        memory[k].lchild = NULL;
        memory[k].rchild = k + 1;
    }
    memory[MAX_TREE_SIZE].lchild = NULL;
    memory[MAX_TREE_SIZE].rchild = NULL;
    for (int k = 0; k < HASH_SIZE; ++k) {
        hashtable[k].symbol = "0";
    }
    hashtable_init();
    while (true) {
        cout << ">";
        string input;
        getline(cin, input);
        lower(input);
        if (input == "exit")
            exit(1);
        i = 0, j = 0;
        input = ltrim(input);
        input = Preprocessing(input);
        i = 0, j = 0;
        input = ltrim(input);
        int root = NULL;
        try {
            root = Read(input);
        }
        catch (out_of_range &e) {
            cout << "----- garbage collecting -----" << endl;
            garbageCollector();
            i = 0, j = 0;
            root = Read(input);
            int result = Eval(root);
            if (result < 0) {
                cout << "] " << getSymbol(result) << endl;
            }
            else if (result >= 0) {
                cout << "] ";
                Print(result, true);
                cout << endl;
            }
            cout << "Free list's root=" << freelist << endl;
            cout << "List's root=" << root << endl;
            printTree();
            cout << "Hash Table" << endl;
            cout << "hash value, symbol, link" << endl;
            for (int k = 0; k < HASH_SIZE; ++k) {
                if (hashtable[k].symbol != "0")
                    cout << -k << " " << hashtable[k].symbol << " " << hashtable[k].link << endl;
            }
            continue;
        }
        int result = Eval(root);
        if (result < 0) {
            cout << "] " << getSymbol(result) << endl;
        }else if (result >= 0) {
            cout << "] ";
            Print(result, true);
            cout << endl;
        }
        //garbageCollector();
        cout << "Free list's root=" << freelist << endl;
        cout << "List's root=" << root << endl;
        printTree();
        cout << "Hash Table" << endl;
        cout << "hash value, symbol, link" << endl;
        for (int k = 0; k < HASH_SIZE; ++k) {
            if (hashtable[k].symbol != "0")
                cout << -k << "\t" << hashtable[k].symbol << "\t" << hashtable[k].link << endl;
        }
        /*
        cout << "Free list's root=" << freelist << endl;
        cout << "List's root=" << root << endl;
        printTree();
        cout << "Hash Table" << endl;
        cout << "hash value, symbol, link" << endl;
        for (int k = 0; k < HASH_SIZE; ++k) {
            if (hashtable[k].symbol != "0")
                cout << -k << " " << hashtable[k].symbol << " " << hashtable[k].link << endl;
        }
        */
        //if (root > 0)
            //init_mem(root, freelist);
    }
    return 0;
}