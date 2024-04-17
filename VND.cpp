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
    
    struct swap{
        int job1;
        int novo_server_j1;
        int ocup_server_j1;
        int job2;
        int novo_server_j2;
        int ocup_server_j2;
    };

    swap melhor_swap;
    // Combinação de jobs alocados em servidores diferentes
    for (int j1 = 0; j1 < n_jobs; j1++){
        for (int j2 = j1+1; j2 < n_jobs; j2++){
            // Se os jobs estão alocados em servidores diferentes
            if (this->solucao_atual.alocacao[j1] != this->solucao_atual.alocacao[j2]){
                // Troca os servidores
                int serv_atual_j1 = this->solucao_atual.alocacao[j1]; // O(1)
                int serv_atual_j2 = this->solucao_atual.alocacao[j2]; // O(1)

                int custo_atual = this->solucao_atual.custo; // O(1)

                // Retira o custo da solucao dos dois jobs
                custo_atual -= this->dados->get_custo_job_servidor(j1, serv_atual_j1); // O(1)
                custo_atual -= this->dados->get_custo_job_servidor(j2, serv_atual_j2); // O(1)
                // retira o tempo de ocupação dos jobs
                int ocup_serv_atual_j1 = this->solucao_atual.ocupacao[serv_atual_j1]   // O(1)
                                        - this->dados->get_tempo_job_servidor(j1, serv_atual_j1); // O(1)

                int ocup_serv_atual_j2 = this->solucao_atual.ocupacao[serv_atual_j2]  // O(1)
                                        - this->dados->get_tempo_job_servidor(j2, serv_atual_j2); // O(1)
                
                // recupera o tempo de rodar nos servidores trocados
                int t_j1_serv_j2 = this->dados->get_tempo_job_servidor(j1, serv_atual_j2); // O(1)
                int t_j2_serv_j1 = this->dados->get_tempo_job_servidor(j2, serv_atual_j1); // O(1)
                
                // verifica se é possivel alocar
                if (ocup_serv_atual_j1 + t_j2_serv_j1 > this->dados->get_capacidade_servidor(serv_atual_j1) ||
                    ocup_serv_atual_j2 + t_j1_serv_j2 > this->dados->get_capacidade_servidor(serv_atual_j2)){ 
                    // alocação inválida
                    continue;
                }

                custo_atual += this->dados->get_custo_job_servidor(j1, serv_atual_j2); // O(1)
                custo_atual += this->dados->get_custo_job_servidor(j2, serv_atual_j1); // O(1)
                
                if (custo_atual < novo_custo){
                    novo_custo = custo_atual;
                    // Atualiza o novo servidor de j1 com a ocupação do servidor de destino (j2)
                    melhor_swap.job1 = j1;
                    melhor_swap.novo_server_j1 = serv_atual_j2;
                    melhor_swap.ocup_server_j1 = ocup_serv_atual_j2 + t_j1_serv_j2;
                    // Atualiza o novo servidor de j2 com a ocupação do servidor de destino (j1)
                    melhor_swap.job2 = j2;
                    melhor_swap.novo_server_j2 = serv_atual_j1;
                    melhor_swap.ocup_server_j2 = ocup_serv_atual_j1 + t_j2_serv_j1;
                }
            }
        }
    }
    Solucao nova_solucao = this->solucao_atual;
    if (novo_custo < this->solucao_atual.custo){
        nova_solucao.custo = novo_custo;
        nova_solucao.alocacao[melhor_swap.job1] = melhor_swap.novo_server_j1;
        nova_solucao.alocacao[melhor_swap.job2] = melhor_swap.novo_server_j2;
        nova_solucao.ocupacao[melhor_swap.novo_server_j1] = melhor_swap.ocup_server_j1;
        nova_solucao.ocupacao[melhor_swap.novo_server_j2] = melhor_swap.ocup_server_j2;
    }
    return nova_solucao;
}