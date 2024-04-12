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
    Solucao nova_solucao = this->solucao_atual;
    int n_jobs = this->dados->get_n_jobs();

    // Combinação de jobs alocados em servidores diferentes
    for (int j1 = 0; j1 < n_jobs; j1++){
        for (int j2 = j1+1; j2 < n_jobs; j2++){
            // Se os jobs estão alocados em servidores diferentes
            if (this->solucao_atual.alocacao[j1] != this->solucao_atual.alocacao[j2]){
                // Troca os servidores
                int serv_job1 = this->solucao_atual.alocacao[j1];
                int serv_job2 = this->solucao_atual.alocacao[j2];

                int custo_atual = this->solucao_atual.custo;

                if (serv_job1 == -1){
                    // retira o custo e o tempo job2 no servidor atual
                    custo_atual -= this->dados->get_custo_job_servidor(j2, serv_job2);
                    int ocupacao_serv_job2 = this->solucao_atual.ocupacao[serv_job2];
                    ocupacao_serv_job2 -= this->dados->get_tempo_job_servidor(j2, serv_job2);
                    
                    // testa se a nova alocação é válida
                    int tempo_job1_serv2 = this->dados->get_tempo_job_servidor(j1, serv_job2);
                    if (ocupacao_serv_job2 + tempo_job1_serv2 <= this->dados->get_capacidade_servidor(serv_job2)){
                        // aumenta custo do job1 no servidor do job2 + custo fixo (job2 no servidor local)
                        custo_atual += this->dados->get_custo_job_servidor(j1, serv_job2);
                        if (custo_atual < nova_solucao.custo){
                            Solucao s = this->solucao_atual;
                            s.custo = custo_atual;
                            s.alocacao[j1] = serv_job2;
                            s.alocacao[j2] = -1;
                            s.ocupacao[serv_job2] += tempo_job1_serv2;
                            nova_solucao = s;
                        }
                    }
                }
                else if (serv_job2 == -1){
                    // retira o custo e o tempo job2 no servidor atual
                    custo_atual -= this->dados->get_custo_job_servidor(j1, serv_job1);
                    int ocupacao_serv_job1 = this->solucao_atual.ocupacao[serv_job1];
                    ocupacao_serv_job1 -= this->dados->get_tempo_job_servidor(j1, serv_job1);
                    
                    // testa se a nova alocação é válida
                    int tempo_job2_serv_job1 = this->dados->get_tempo_job_servidor(j2, serv_job1);
                    if (ocupacao_serv_job1 + tempo_job2_serv_job1 <= this->dados->get_capacidade_servidor(serv_job1)){
                        // aumenta custo do job1 no servidor do job2 + custo fixo (job2 no servidor local)
                        custo_atual += this->dados->get_custo_job_servidor(j2, serv_job1);
                        if (custo_atual < nova_solucao.custo){
                            Solucao s = this->solucao_atual;
                            s.custo = custo_atual;
                            s.alocacao[j1] = -1;
                            s.alocacao[j2] = serv_job1;
                            s.ocupacao[serv_job1] += tempo_job2_serv_job1;
                            nova_solucao = s;
                        }
                    }
                }
                else {
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
                    
                    if (custo_atual < nova_solucao.custo){
                        Solucao s = this->solucao_atual;
                        s.custo = custo_atual;
                        s.alocacao[j1] = serv_job2;
                        s.alocacao[j2] = serv_job1;
                        s.ocupacao[serv_job1] += t_job2_serv_job1;
                        s.ocupacao[serv_job2] += t_job1_serv_job2;
                        nova_solucao = s;
                    }
                }
            }
        }
    }
    return nova_solucao;
}