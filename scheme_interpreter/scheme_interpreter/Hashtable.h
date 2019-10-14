#pragma once
#define HASH_SIZE 1031
#include <string>
class Hashtable {
private:
    int n;
public:
    Hashtable() {
        this->n = HASH_SIZE;
    }
    std::string symbol;
    int link = NULL;
};

