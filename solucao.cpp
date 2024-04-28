#include <iostream>
#include <vector>
#include "solucao.h"
#include <fstream>

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

void Solucao::print_solucao(){
    cout << "Custo Total: " << this->custo << endl;
    cout << "Custo Nuvem: " << this->custo_nuvem << endl;
    cout << "Custo Local: " << this->custo_local << endl;
    cout << "Alocacao: ";
    for (int job = 0; job < int(this->alocacao.size()); job++){
        cout << this->alocacao[job] << " ";
    }
    cout << endl;
}


void Solucao::criar_arquivo(string nome_arquivo){
    int tam = ocupacao.size();
    vector<vector<int>> aloc_servs(tam+1);

    ofstream arq_saida;

    string nome_arq = nome_arquivo.substr(nome_arquivo.find_last_of("/") + 1);

    arq_saida.open("saida/" + nome_arq);

    arq_saida << this->custo << endl;
    arq_saida << this->custo_nuvem << endl;
    arq_saida << this->custo_local << endl;

    for (int job = 0; job < int(this->alocacao.size()); job++){
        if (this->alocacao[job] == -1){
            aloc_servs[tam].push_back(job +1);
        }
        else{
            aloc_servs[this->alocacao[job]].push_back(job +1);
        }
    }
    arq_saida << endl;
    for (int serv = 0; serv < tam+1; serv++){
        for (int job = 0; job < int(aloc_servs[serv].size()); job++){
            arq_saida << aloc_servs[serv][job] << " ";
        }
        arq_saida << endl;
    }
    arq_saida.close();
}
