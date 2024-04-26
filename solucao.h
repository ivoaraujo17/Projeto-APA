#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <string>
#include <vector>

using namespace std;

class Solucao{
    public:
        vector<int> alocacao;
        vector<int> ocupacao;
        int custo;
        int custo_nuvem;
        int custo_local;

        Solucao();
        Solucao(int n_jobs, int n_servidores);
        void print_solucao();
};

#endif