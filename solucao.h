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

        Solucao();
        Solucao(int n_jobs, int n_servidores);

};

#endif