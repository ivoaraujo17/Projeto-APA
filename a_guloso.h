#ifndef A_GULOSO_H
#define A_GULOSO_H

#include <string>
#include <vector>
#include <fstream>
#include "read_arquivo.h"

using namespace std;

class A_Guloso {
    private:
        Read_Arquivo* dados;
    public:
        A_Guloso(Read_Arquivo* dados);
        void executar();
};


#endif