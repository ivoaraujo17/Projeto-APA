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
    int n_jobs = dados->get_n_jobs();

    // Ordena usando o sort atraves do job.tempo
    sort(dados->jobs_total.begin(), dados->jobs_total.end(), [](job a, job b){
        return a.tempo < b.tempo;
    });
    // O(j*s)
    int jobs_alocados = 0;
    int index = 0;
    this->solucao.custo = n_jobs * dados->get_custo_fixo();
    this->solucao.custo_local = n_jobs * dados->get_custo_fixo();
    this->solucao.custo_nuvem = 0;
    while (index < int(dados->jobs_total.size()) && jobs_alocados < n_jobs){
        int id_job = dados->jobs_total[index].id;
        int id_servidor = dados->jobs_total[index].servidor;
        int tempo = dados->jobs_total[index].tempo;
        
        if (this->solucao.alocacao[id_job] != -1){
            index++;
            continue;
        }
        if (this->solucao.ocupacao[id_servidor] + tempo <= dados->get_capacidade_servidor(id_servidor)){
            // Remove o custo fixo do custo total por ter alocado mais um job na nuvem
            int custo_fixo = dados->get_custo_fixo();
            this->solucao.custo -= custo_fixo;
            this->solucao.custo_local -= custo_fixo;
            // Adiciona o custo do job alocado
            int custo = dados->jobs_total[index].custo;
            this->solucao.custo += custo;
            this->solucao.custo_nuvem += custo;
            // Atualiza alocação e ocupação do novo servidor
            this->solucao.alocacao[id_job] = id_servidor;
            this->solucao.ocupacao[id_servidor] += tempo;
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
}