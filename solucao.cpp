#include <iostream>
#include <vector>
#include "solucao.h"

using namespace std;

Solucao::Solucao() {
    this->alocacao = vector<int>();
    this->ocupacao = vector<int>();
    this->custo = 0;
}

Solucao::Solucao(int n_jobs, int n_servidores) {
    this->alocacao = vector<int>(n_jobs, -1);
    this->ocupacao = vector<int>(n_servidores, 0);
    this->custo = 0;
}
