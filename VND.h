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
    public:
        VND(Solucao* solucao, Read_Arquivo* dados);
        Solucao executar();
        bool swap();
        bool reinsertion();
        bool swap_local();
};


#endif