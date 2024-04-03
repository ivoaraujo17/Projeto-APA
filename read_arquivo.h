#ifndef READ_ARQUIVO_H
#define READ_ARQUIVO_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Read_Arquivo {
private:
    ifstream arquivo;
    string linha;
    int n_jobs;
    int n_servidores;
    int custo_fixo;
    vector<vector<int>> t_proces_job;
    vector<vector<int>> custo_job;
    vector<int> capacidade_servidores;

public:
    Read_Arquivo(string nome_arquivo);
    int get_n_jobs();
    int get_n_servidores();
    int get_capacidade_servidores(int i);
    int get_custo_fixo();
    vector<vector<int>> get_t_proces_job();
    vector<vector<int>> get_custo_job();
    ~Read_Arquivo();
};

#endif
