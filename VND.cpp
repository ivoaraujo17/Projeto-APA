#include <iostream>
#include <vector>
#include <algorithm>
#include "solucao.h"
#include "read_arquivo.h"
#include "VND.h"
#include "a_guloso.h"

using namespace std;

VND::VND(Solucao* solucao_gulosa, Read_Arquivo* dados, vector<vector<job>>* jobs_ordenados) {
    this->solucao_atual = *solucao_gulosa;
    this->dados = dados;
    this->jobs_ordenados = jobs_ordenados;
}

Solucao VND::executar() {
    int k = 1;
    while (k < 300000)
    {
        Solucao s = this->swap();
        if (s.custo < this->solucao_atual.custo){
            this->solucao_atual = s;
            cout << "Solucao atualizada: " << this->solucao_atual.custo << endl;
        }
        k++;
    }
    return this->solucao_atual;
}

Solucao VND::swap(){
    
    int n_jobs = this->dados->get_n_jobs();

    int novo_custo = this->solucao_atual.custo;
    vector<int> job1_alocacao(2);
    vector<int> job2_alocacao(2);
    vector<int> s1_ocupacao(2);
    vector<int> s2_ocupacao(2);

    // Combinação de jobs alocados em servidores diferentes
    for (int j1 = 0; j1 < n_jobs; j1++){
        for (int j2 = j1+1; j2 < n_jobs; j2++){
            // Se os jobs estão alocados em servidores diferentes
            if (this->solucao_atual.alocacao[j1] != this->solucao_atual.alocacao[j2]){
                // Troca os servidores
                int serv_job1 = this->solucao_atual.alocacao[j1];
                int serv_job2 = this->solucao_atual.alocacao[j2];

                int custo_atual = this->solucao_atual.custo;
               
                // Retira o custo da solucao dos dois jobs
                custo_atual -= this->dados->get_custo_job_servidor(j1, serv_job1);
                custo_atual -= this->dados->get_custo_job_servidor(j2, serv_job2);

                // retira o tempo de ocupacao dos servidores
                int t_job1 = this->dados->get_tempo_job_servidor(j1, serv_job1);
                int t_job2 = this->dados->get_tempo_job_servidor(j2, serv_job2);

                int ocupacao_serv_job1 = this->solucao_atual.ocupacao[serv_job1];
                ocupacao_serv_job1 -= t_job1;
                int ocupacao_serv_job2 = this->solucao_atual.ocupacao[serv_job2];
                ocupacao_serv_job2 -= t_job2;
                
                // verifica se a nova alocação é válida
                int t_job1_serv_job2 = this->dados->get_tempo_job_servidor(j1, serv_job2);
                int t_job2_serv_job1 = this->dados->get_tempo_job_servidor(j2, serv_job1);

                if (ocupacao_serv_job1 + t_job2_serv_job1 > this->dados->get_capacidade_servidor(serv_job1) ||
                    ocupacao_serv_job2 + t_job1_serv_job2 > this->dados->get_capacidade_servidor(serv_job2)){ 
                    // alocação inválida
                    continue;
                }

                custo_atual += this->dados->get_custo_job_servidor(j1, serv_job2);
                custo_atual += this->dados->get_custo_job_servidor(j2, serv_job1);
                
                if (custo_atual < novo_custo){
                    novo_custo = custo_atual;
                    job1_alocacao[0] = j1; job1_alocacao[1] = serv_job2;
                    job2_alocacao[0] = j2; job2_alocacao[1] = serv_job1;
                    s1_ocupacao[0] = serv_job1; s1_ocupacao[1] = ocupacao_serv_job1 + t_job2_serv_job1;
                    s2_ocupacao[0] = serv_job2; s2_ocupacao[1] = ocupacao_serv_job2 + t_job1_serv_job2;
                }
            }
        }
    }
    Solucao nova_solucao = this->solucao_atual;
    if (novo_custo < this->solucao_atual.custo){
        nova_solucao.custo = novo_custo;
        nova_solucao.alocacao[job1_alocacao[0]] = job1_alocacao[1];
        nova_solucao.alocacao[job2_alocacao[0]] = job2_alocacao[1];
        nova_solucao.ocupacao[s1_ocupacao[0]] = s1_ocupacao[1];
        nova_solucao.ocupacao[s2_ocupacao[0]] = s2_ocupacao[1];
    }
    return nova_solucao;
}