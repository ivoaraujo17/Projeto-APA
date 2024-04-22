#ifndef VND_H
#define VND_H

#include <string>
#include <vector>
#include "solucao.h"
#include "read_arquivo.h"
#include "a_guloso.h"

using namespace std;

class VND {
    private:
        Solucao solucao_atual;
        Read_Arquivo* dados;
        vector<vector<job>>* jobs_ordenados;
    public:
        VND(Solucao* solucao, Read_Arquivo* dados, vector<vector<job>>* jobs_ordenados);
        Solucao executar();
        Solucao swap();
        Solucao reinsertion();
        //Solucao VND::two_opt();
};


#endif