#pragma once
#define MAX_TREE_SIZE 30
class Mem {
private:
    int n;
public:
    Mem() {
        this->n = MAX_TREE_SIZE;
    }
    int rchild = 0;
    int lchild = 0;
};
