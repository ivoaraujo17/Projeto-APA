#ifndef A_GULOSO_H
#define A_GULOSO_H

#include <string>
#include <vector>
#include <fstream>
#include "read_arquivo.h"
#include "solucao.h"

using namespace std;

class A_Guloso {
    private:
        Read_Arquivo* dados;
    public:
        Solucao solucao;
        A_Guloso(Read_Arquivo* dados);
        void executar();
        void print_guloso();
};


#endif