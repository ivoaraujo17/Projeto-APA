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
        this->n_servidores = stoi(linha) + 1;
        getline(arquivo, linha);
        this->custo_fixo = stoi(linha);
        // atualiza o tamanho da matriz jobs para o tamanho n_servidores x n_jobs
        this->jobs.resize(this->n_servidores, vector<job>(this->n_jobs));
        this->capacidade_servidores.resize(this->n_servidores);

        getline(arquivo, linha); // pega a linha em branco

        // leitura do vetor de capacidade dos servidores
        getline(arquivo, linha);
        string aux = "";
        int pos_servidor = 0;
        int maior_capacidade = 0;
        for (int c=0; c < int(linha.size()); c++){
            if (linha[c] == ' ' and aux != ""){
                this->capacidade_servidores[pos_servidor] = stoi(aux);
                pos_servidor++;
                if (stoi(aux) > maior_capacidade){
                    maior_capacidade = stoi(aux);
                }
                aux = "";
            }
            else if(linha[c] != ' '){
                aux += linha[c];
            }
        }
        
        this->capacidade_servidores[pos_servidor] = stoi(aux);
        

        getline(arquivo, linha); // pega a linha em branco
        // leitura da matriz de tempo de processamento
        int maior_tempo = 0;
        for (int i = 0; i < this->n_servidores; i++){
            if (i == this->n_servidores - 1){
                for (int j = 0; j < this->n_jobs; j++){
                    this->jobs[i][j].tempo = maior_tempo + 1;
                    this->jobs[i][j].servidor = i;
                    this->jobs[i][j].id = j;
                }
                this->capacidade_servidores[this->n_servidores - 1] = this->n_jobs*(maior_tempo+1);
                continue;
            }
            getline(arquivo, linha);
            string aux = "";
            int coluna = 0;
            for (int c=0; c < int(linha.size()); c++){
                if (linha[c] == ' ' and aux != ""){
                    this->jobs[i][coluna].tempo = stoi(aux);
                    this->jobs[i][coluna].servidor = i;
                    this->jobs[i][coluna].id = coluna;
                    coluna++;
                    if (stoi(aux) > maior_tempo){
                        maior_tempo = stoi(aux);
                    }
                    aux = "";
                    
                }
                else if(linha[c] != ' '){
                    aux += linha[c];
                }
            }
            this->jobs[i][coluna].tempo = stoi(aux); 
            this->jobs[i][coluna].servidor = i;
            this->jobs[i][coluna].id = coluna;
        }

        getline(arquivo, linha); // pega a linha em branco

        // leitura da matriz de custo
        for (int i = 0; i < this->n_servidores; i++){
            if (i == this->n_servidores - 1){
                for (int j = 0; j < this->n_jobs; j++){
                    this->jobs[i][j].custo = custo_fixo;
                }
                continue;
            }
            getline(arquivo, linha);
            string aux = "";
            int coluna = 0;
            for (int c=0; c < int(linha.size()); c++){
                if (linha[c] == ' '){
                    this->jobs[i][coluna].custo = stoi(aux);
                    coluna++;
                    aux = "";
                }
                else{
                    aux += linha[c];
                }
            }
            this->jobs[i][coluna].custo = stoi(aux);
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
    return this->jobs[servidor][job].tempo;
}

int Read_Arquivo::get_custo_job_servidor(int job, int servidor){
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
