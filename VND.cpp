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
    while (k <= 3){   
        if (k == 1){
            Solucao s = this->swap();
            if (s.custo < this->solucao_atual.custo){
                this->solucao_atual = s;
                k = 1;
            }
            else{
                k++;
            }
        }
        else if (k == 3){
            Solucao s = this->swap_local();
            if (s.custo < this->solucao_atual.custo){
                this->solucao_atual = s;
                k = 1;
            }
            else{
                k++;
            }
        }
        else if (k == 2){
            Solucao s = this->reinsertion();
            if (s.custo < this->solucao_atual.custo){
                this->solucao_atual = s;
                k = 1;
            }
            else{
                k++;
            }
        }
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
            int serv_atual_j1 = this->solucao_atual.alocacao[j1]; // O(1)
            int serv_atual_j2 = this->solucao_atual.alocacao[j2]; // O(1)

            if (serv_atual_j1 == -1 || serv_atual_j2 == -1){
                continue;
            }

            if (serv_atual_j1 == serv_atual_j2){
                continue;
            }

            // Obtem o custo atual da solução
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
            
            // Verifica se o custo é menor que o custo atual
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

Solucao VND::swap_local(){
    
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
            int serv_atual_j1 = this->solucao_atual.alocacao[j1]; // O(1)
            int serv_atual_j2 = this->solucao_atual.alocacao[j2]; // O(1)

            if (serv_atual_j1 != -1 && serv_atual_j2 != -1){
                continue;
            }

            if (serv_atual_j1 == serv_atual_j2){
                continue;
            }

            if (serv_atual_j1 == -1){
                // Obtem o custo atual da solução
                int custo_atual = this->solucao_atual.custo; // O(1)

                // Retira o custo da solucao dos dois jobs
                custo_atual -= this->dados->get_custo_job_servidor(j2, serv_atual_j2); // O(1)
                // retira o tempo de ocupação dos jobs
                int ocup_serv_atual_j2 = this->solucao_atual.ocupacao[serv_atual_j2]  // O(1)
                                        - this->dados->get_tempo_job_servidor(j2, serv_atual_j2); // O(1)
                
                // recupera o tempo de rodar nos servidores trocados
                int t_j1_serv_j2 = this->dados->get_tempo_job_servidor(j1, serv_atual_j2); // O(1)
                
                // verifica se é possivel alocar
                if (ocup_serv_atual_j2 + t_j1_serv_j2 > this->dados->get_capacidade_servidor(serv_atual_j2)){ 
                    // alocação inválida
                    continue;
                }

                custo_atual += this->dados->get_custo_job_servidor(j1, serv_atual_j2); // O(1)
                // não precisa adicionar o custo fixo pois o servidor j1 estava adicionando o custo fixo
                
                // Verifica se o custo é menor que o custo atual
                if (custo_atual < novo_custo){
                    novo_custo = custo_atual;
                    // Atualiza o novo servidor de j1 com a ocupação do servidor de destino (j2)
                    melhor_swap.job1 = j1;
                    melhor_swap.novo_server_j1 = serv_atual_j2;
                    melhor_swap.ocup_server_j1 = ocup_serv_atual_j2 + t_j1_serv_j2;
                    // Atualiza o novo servidor de j2 com a ocupação do servidor de destino (j1)
                    melhor_swap.job2 = j2;
                    melhor_swap.novo_server_j2 = -1;
                    melhor_swap.ocup_server_j2 = -1;
                }
            }
            else{
                // Obtem o custo atual da solução
                int custo_atual = this->solucao_atual.custo; // O(1)

                // Retira o custo da solucao dos dois jobs
                custo_atual -= this->dados->get_custo_job_servidor(j1, serv_atual_j1); // O(1)

                // retira o tempo de ocupação dos jobs
                int ocup_serv_atual_j1 = this->solucao_atual.ocupacao[serv_atual_j1]   // O(1)
                                        - this->dados->get_tempo_job_servidor(j1, serv_atual_j1); // O(1)
                
                // recupera o tempo de rodar nos servidores trocados
                int t_j2_serv_j1 = this->dados->get_tempo_job_servidor(j2, serv_atual_j1); // O(1)
                
                // verifica se é possivel alocar
                if (ocup_serv_atual_j1 + t_j2_serv_j1 > this->dados->get_capacidade_servidor(serv_atual_j1)){ 
                    // alocação inválida
                    continue;
                }

                custo_atual += this->dados->get_custo_job_servidor(j2, serv_atual_j1); // O(1)
                
                // Verifica se o custo é menor que o custo atual
                if (custo_atual < novo_custo){
                    novo_custo = custo_atual;
                    // Atualiza o novo servidor de j1 com a ocupação do servidor de destino (j2)
                    melhor_swap.job1 = j1;
                    melhor_swap.novo_server_j1 = -1;
                    melhor_swap.ocup_server_j1 = -1;
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
        if (melhor_swap.novo_server_j1 != -1){
            nova_solucao.ocupacao[melhor_swap.novo_server_j1] = melhor_swap.ocup_server_j1;
        }
        if (melhor_swap.novo_server_j2 != -1){
            nova_solucao.ocupacao[melhor_swap.novo_server_j2] = melhor_swap.ocup_server_j2;
        }
    }
    return nova_solucao;
}

Solucao VND::reinsertion(){
    int job = -1;
    int servidor = -1;
    int melhor_custo = this->solucao_atual.custo;
    for (int j = 0; j < this->dados->get_n_jobs(); j++){
        // Itera sobre os servidores para encontrar o novo melhor servidor para o job j
        for (int s = 0; s<this->dados->get_n_servidores(); s++){
            // Desconsidera o servidor atual do job j
            if (this->solucao_atual.alocacao[j] != s){
                int melhor_servidor = (*this->jobs_ordenados)[j][s].servidor;
                int tempo_melhor_servidor = (*this->jobs_ordenados)[j][s].tempo;

                // Verifica se o servidor tem capacidade para alocar o job j no servidor s
                if (this->solucao_atual.ocupacao[melhor_servidor] + tempo_melhor_servidor 
                    > this->dados->get_capacidade_servidor(melhor_servidor)){
                    continue;
                }

                // Calcula o novo custo no caso realocação
                int custo_atual = this->solucao_atual.custo;
                custo_atual -= this->dados->get_custo_job_servidor(j, this->solucao_atual.alocacao[j]);
                custo_atual += this->dados->get_custo_job_servidor(j, melhor_servidor);

                // Verifica se o custo é menor que o custo atual
                if (custo_atual < melhor_custo){
                    // Atualiza o melhor custo, job e servidor para troca
                    melhor_custo = custo_atual;
                    job = j;
                    servidor = melhor_servidor;
                }
            }
        }
    }
    Solucao nova_solucao = this->solucao_atual;
    if (melhor_custo < this->solucao_atual.custo){
        // reduz a capacidade do servidor antigo
        int servidor_antigo = nova_solucao.alocacao[job];
        nova_solucao.ocupacao[servidor_antigo] -= this->dados->get_tempo_job_servidor(job, servidor_antigo);
        // atualiza a nova solução
        nova_solucao.alocacao[job] = servidor;
        nova_solucao.ocupacao[servidor] += this->dados->get_tempo_job_servidor(job, servidor);
        // atualiza o custo
        nova_solucao.custo = melhor_custo;

        return nova_solucao;
    }
    return this->solucao_atual;
}