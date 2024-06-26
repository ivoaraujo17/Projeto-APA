#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "read_arquivo.h"

using namespace std;


Read_Arquivo::Read_Arquivo(string nome_arquivo){
    this->nome = nome_arquivo;
    arquivo.open(nome_arquivo);
    if (arquivo.is_open()){
        getline(arquivo, linha);
        this->n_jobs = stoi(linha);
        getline(arquivo, linha);
        this->n_servidores = stoi(linha);
        getline(arquivo, linha);
        this->custo_fixo = stoi(linha);
        // atualiza o tamanho da matriz jobs para o tamanho n_servidores x n_jobs
        this->jobs.resize(this->n_servidores, vector<job>(this->n_jobs));
        this->jobs_total.resize(this->n_jobs*this->n_servidores);
        this->capacidade_servidores.resize(this->n_servidores);

        getline(arquivo, linha); // pega a linha em branco

        // leitura do vetor de capacidade dos servidores
        getline(arquivo, linha);
        stringstream ss(linha);
        string aux;
        int pos_servidor = 0;
        while (ss >> aux) {
            this->capacidade_servidores[pos_servidor] = stoi(aux);
            pos_servidor++;
        }
        

        getline(arquivo, linha); // pega a linha em branco
        // leitura da matriz de tempo de processamento
        for (int i = 0; i < this->n_servidores; i++){
            getline(arquivo, linha);
            stringstream ss(linha);
            string aux;
            int coluna = 0;
            while (ss >> aux) {
                this->jobs[i][coluna].tempo = stoi(aux);
                this->jobs[i][coluna].servidor = i;
                this->jobs[i][coluna].id = coluna;
                coluna++;
            }
        }

        getline(arquivo, linha); // pega a linha em branco

        // leitura da matriz de custo
        for (int i = 0; i < this->n_servidores; i++){
            getline(arquivo, linha);
            stringstream ss(linha);
            string aux;
            int coluna = 0;
            while (ss >> aux) {
            this->jobs[i][coluna].custo = stoi(aux);
            this->jobs[i][coluna].ct = this->jobs[i][coluna].tempo * this->jobs[i][coluna].custo;
            this->jobs_total[i*this->n_jobs + coluna] = this->jobs[i][coluna];
            coluna++;
            }
        }
    }
    else{
        cout << "Erro ao abrir o arquivo" << endl;
        exit(1);
    }
}

int Read_Arquivo::get_n_jobs(){
    return this->n_jobs;
}

int Read_Arquivo::get_n_servidores(){
    return this->n_servidores;
}

int Read_Arquivo::get_capacidade_servidor(int i){
    return this->capacidade_servidores[i];
}

int Read_Arquivo::get_custo_fixo(){
    return this->custo_fixo;
}

int Read_Arquivo::get_tempo_job_servidor(int job, int servidor){
    if (servidor == -1){
        return -1;
    }
    return this->jobs[servidor][job].tempo;
}

int Read_Arquivo::get_custo_job_servidor(int job, int servidor){
    if (servidor == -1){
        return custo_fixo;
    }
    return this->jobs[servidor][job].custo;
}

vector<job> Read_Arquivo::get_info_job(int n_job){
    // complexidade O(s)
    vector<job> info_job(get_n_servidores());
    for (int s =0; s < this->get_n_servidores(); s++){
        info_job[s] = this->jobs[s][n_job];
    }
    return info_job;
}

void Read_Arquivo::print_matriz(){
    for (int s = 0; s < this->n_servidores; s++){
        for (int j = 0; j < this->n_jobs; j++){
            cout << "Servidor: " << jobs[s][j].servidor << " ";
            cout << "Job: " << jobs[s][j].id << " ";
            cout << "Tempo: " << jobs[s][j].tempo << " ";
            cout << "Custo: " << jobs[s][j].custo << endl;
        }
    }
    for (int s = 0; s < this->n_servidores; s++){
        cout << "Capacidade servidor " << s << ": " << this->capacidade_servidores[s] << endl;
    }
}

Read_Arquivo::~Read_Arquivo(){
    arquivo.close();
}
