#pragma once
#include <utility>
using std::pair;
class Stack {
private:
    int size;
    pair<int,int>* arr;
public:
    int index;
    Stack();
    void pop();
    pair<int,int> top();
    ~Stack();
    void push(pair<int,int> p);
    void print();
};
