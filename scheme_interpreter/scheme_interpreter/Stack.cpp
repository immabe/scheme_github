#include "Stack.h"
#pragma once
#include <iostream>
using std::cout;
using std::pair;
Stack::Stack() {
    this->size = 50;
    this->arr = new pair<int,int>[size];
    this->index = -1;
}

void Stack::pop() {
    if (index == -1) {
        std::cout << "stack is empty" << std::endl;
        exit(1);
    }
    else {
        index--;
    }
}

pair<int,int> Stack::top() {
    if (index == -1) {
        std::cout << "stack is empty" << std::endl;
        exit(1);
    }
    else {
        return this->arr[index];
    }
}

Stack::~Stack() {
    delete[] arr;
}

void Stack::push(pair<int,int> p) {
    if (index == size) {
        std::cout << "stack is full" << std::endl;
        exit(1);
    }
    else {
        this->arr[++index] = p;
    }
}

void Stack::print() {
    cout << "stack::::  ";
    for (int i = 0; i <= index; ++i)
        std::cout << "(" << arr[i].first << " " << arr[i].second << ")" << " ";
    std::cout << std::endl;
}