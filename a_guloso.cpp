#include <iostream>
#include <vector>
#include <fstream>
#include "a_guloso.h"
#include "read_arquivo.h"
#include <algorithm>
#include "solucao.h"

using namespace std;

A_Guloso::A_Guloso(Read_Arquivo* dados){
    this->dados = dados;
    this->solucao = Solucao(dados->get_n_jobs(), dados->get_n_servidores());    
}

void A_Guloso::executar(){

    int n_servidores = dados->get_n_servidores();
    int n_jobs = dados->get_n_jobs();
    this->jobs_ordenados.resize(n_jobs, vector<job>(n_servidores));
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
            if (this->solucao.ocupacao[s] + info_job[s].tempo <= dados->get_capacidade_servidor(s)){
                this->solucao.ocupacao[s] += info_job[s].tempo;
                this->solucao.custo += info_job[s].custo;
                this->solucao.alocacao[j] = s;
                break; 
            }
        }
    }
    // Verifica se algum job não foi alocado
    int custo_fixo = dados->get_custo_fixo();
    for (int j = 0; j < n_jobs; j++){
        if (this->solucao.alocacao[j] == -1){
            this->solucao.custo += custo_fixo;
        }
    }
    //this->print_guloso();
}

void A_Guloso::print_guloso(){
    for (int j = 0; j < this->dados->get_n_jobs(); j++){
        cout << "job " << j << " alocado no servidor " << solucao.alocacao[j] << endl;
    }
    
    for (int s = 0; s < this->dados->get_n_servidores(); s++){
        cout << "servidor " << s << " ocupado: " << solucao.ocupacao[s];
        cout << " total: " << dados->get_capacidade_servidor(s) << endl;
    }

    for (int j = 0; j < this->dados->get_n_jobs(); j++){
        cout << "job " << j << " tempos: ";
        for (int s = 0; s < this->dados->get_n_servidores(); s++){
            cout << jobs_ordenados[j][s].tempo << " " << jobs_ordenados[j][s].servidor << " | ";
        }
        cout << endl;
    }
}