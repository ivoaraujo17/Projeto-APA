#ifndef READ_ARQUIVO_H
#define READ_ARQUIVO_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct job{
    int id;
    int tempo;
    int custo;
    int servidor;
    int ct;
};

class Read_Arquivo {
private:
    ifstream arquivo;
    string linha;
    int n_jobs;
    int n_servidores;
    int custo_fixo;
    vector<vector<job>> jobs;
    vector<int> capacidade_servidores;

public:
    vector<job> jobs_total;
    string nome;
    Read_Arquivo(string nome_arquivo);
    int get_n_jobs();
    int get_n_servidores();
    int get_capacidade_servidor(int i);
    int get_custo_fixo();
    int get_tempo_job_servidor(int job, int servidor);
    int get_custo_job_servidor(int job, int servidor);
    vector<job> get_info_job(int id_job);
    void print_matriz();
    ~Read_Arquivo();
};

#endif
