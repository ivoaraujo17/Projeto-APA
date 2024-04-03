#include <iostream>
#include <vector>
#include <fstream>
#include "read_arquivo.h"

using namespace std;


Read_Arquivo::Read_Arquivo(string nome_arquivo){
    arquivo.open(nome_arquivo);
    if (arquivo.is_open()){
        getline(arquivo, linha);
        this->n_jobs = stoi(linha);
        getline(arquivo, linha);
        this->n_servidores = stoi(linha);
        getline(arquivo, linha);
        this->custo_fixo = stoi(linha);
        // atualiza o tamanho da matriz t process job para o tamanho n_jobs x n_servidores
        this->t_proces_job.resize(this->n_servidores, vector<int>(this->n_jobs));
        this->custo_job.resize(this->n_servidores, vector<int>(this->n_jobs));
        this->capacidade_servidores.resize(this->n_servidores);

        getline(arquivo, linha); // pega a linha em branco

        // leitura do vetor de capacidade dos servidores
        getline(arquivo, linha);
        string aux = "";
        int pos_servidor = 0;
        for (int c=0; c < int(linha.size()); c++){
            if (linha[c] == ' '){
                this->capacidade_servidores[pos_servidor] = stoi(aux);
                pos_servidor++;
                aux = "";
            }
            else{
                aux += linha[c];
            }
        }
        this->capacidade_servidores[pos_servidor] = stoi(aux);

        getline(arquivo, linha); // pega a linha em branco
        
        // leitura da matriz de tempo de processamento
        for (int i = 0; i < this->n_servidores; i++){
            getline(arquivo, linha);
            string aux = "";
            int coluna = 0;
            for (int c=0; c < int(linha.size()); c++){
                if (linha[c] == ' '){
                    this->t_proces_job[i][coluna] = stoi(aux);
                    coluna++;
                    aux = "";
                }
                else{
                    aux += linha[c];
                }
            }
            this->t_proces_job[i][coluna] = stoi(aux); 
        }

        getline(arquivo, linha); // pega a linha em branco

        // leitura da matriz de custo
        for (int i = 0; i < this->n_servidores; i++){
            getline(arquivo, linha);
            string aux = "";
            int coluna = 0;
            for (int c=0; c < int(linha.size()); c++){
                if (linha[c] == ' '){
                    this->custo_job[i][coluna] = stoi(aux);
                    coluna++;
                    aux = "";
                }
                else{
                    aux += linha[c];
                }
            }
            this->custo_job[i][coluna] = stoi(aux);
        }
    }
}

int Read_Arquivo::get_n_jobs(){
    return this->n_jobs;
}

int Read_Arquivo::get_n_servidores(){
    return this->n_servidores;
}

int Read_Arquivo::get_capacidade_servidores(int i){
    return this->capacidade_servidores[i];
}

int Read_Arquivo::get_custo_fixo(){
    return this->custo_fixo;
}

vector<vector<int>> Read_Arquivo::get_t_proces_job(){
    return this->t_proces_job;
}

vector<vector<int>> Read_Arquivo::get_custo_job(){
    return this->custo_job;
}

Read_Arquivo::~Read_Arquivo(){
    arquivo.close();
}
