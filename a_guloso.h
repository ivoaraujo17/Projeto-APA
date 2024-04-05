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
        vector<vector<vector<int>>> tempo_job_ordenado;
        vector<int> c_s_ocupada;
        vector<int> alocacao;
        A_Guloso(Read_Arquivo* dados);
        void executar();
};


#endif