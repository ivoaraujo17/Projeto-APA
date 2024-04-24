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

    // Ordena usando o sort atraves do job.tempo
    sort(dados->jobs_total.begin(), dados->jobs_total.end(), [](job a, job b){
        return a.tempo < b.tempo;
    });
    // O(j*s)
    int jobs_alocados = 0;
    int index = 0;
    this->solucao.custo = n_jobs * dados->get_custo_fixo();
    while (index < dados->jobs_total.size() && jobs_alocados < n_jobs){
        int id_job = dados->jobs_total[index].id;
        int id_servidor = dados->jobs_total[index].servidor;
        int tempo = dados->jobs_total[index].tempo;
        
        if (this->solucao.alocacao[id_job] != -1){
            index++;
            continue;
        }
        if (this->solucao.ocupacao[id_servidor] + tempo <= dados->get_capacidade_servidor(id_servidor)){
            this->solucao.ocupacao[id_servidor] += tempo;
            this->solucao.custo -= dados->get_custo_fixo();
            this->solucao.custo += dados->jobs_total[index].custo;
            this->solucao.alocacao[id_job] = id_servidor;
            jobs_alocados++;   
        }
        index++;
    } 
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