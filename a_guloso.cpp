#include <iostream>
#include <vector>
#include <fstream>
#include "a_guloso.h"
#include "read_arquivo.h"
#include <algorithm>

using namespace std;

A_Guloso::A_Guloso(Read_Arquivo* dados){
    this->dados = dados;
}

void A_Guloso::executar(){

    int n_servidores = dados->get_n_servidores();
    int n_jobs = dados->get_n_jobs();
    this->c_s_ocupada.resize(n_servidores);
    this->alocacao.resize(n_jobs);
    this->jobs_ordenados.resize(n_jobs, vector<job>(n_servidores));

    // O(s) sem muito impacto
    for (int s=0; s < n_servidores; s++){
        this->c_s_ocupada[s] = 0;
    }
    // O(j) sem muito impacto
    for (int j=0; j < n_jobs; j++){
        alocacao[j] = -1;
    }

    // O(j*s)
    for (int j = 0; j < n_jobs; j++){
        vector<job> info_job = dados->get_info_job(j); // Complexidade O(s)
        
        // Ordena usando o sort atraves do job.tempo
        sort(info_job.begin(), info_job.end(), [](job a, job b){
            return a.tempo < b.tempo;
        });

        jobs_ordenados[j] = info_job;

        // aloca o job no servidor de menor custo O(s)
        for (int s = 0; s < n_servidores; s++){
            if (this->c_s_ocupada[s] + info_job[s].tempo <= dados->get_capacidade_servidor(s)){
                this->c_s_ocupada[s] += info_job[s].tempo;
                alocacao[j] = info_job[s].servidor;
                break; 
            }
        }
    }

    
    for (int j = 0; j < n_jobs; j++){
        cout << "job " << j << " alocado no servidor " << alocacao[j] << endl;
    }
    
    for (int s = 0; s < n_servidores; s++){
        cout << "servidor " << s << " ocupado: " << c_s_ocupada[s];
        cout << " total: " << dados->get_capacidade_servidor(s) << endl;
    }

    for (int j = 0; j < n_jobs; j++){
        cout << "job " << j << " tempos: ";
        for (int s = 0; s < n_servidores; s++){
            cout << jobs_ordenados[j][s].tempo << " " << jobs_ordenados[j][s].servidor << " | ";
        }
        cout << endl;
    }
}