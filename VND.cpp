#include <iostream>
#include <vector>
#include <algorithm>
#include "solucao.h"
#include "read_arquivo.h"
#include "VND.h"
#include "a_guloso.h"

using namespace std;

VND::VND(Solucao* solucao_gulosa, Read_Arquivo* dados) {
    this->solucao_atual = *solucao_gulosa;
    this->dados = dados;
}

Solucao VND::executar() {
    int k = 1;
    while (k < 4){
        if (k == 1){
            Solucao s = this->swap_local();
            if (s.custo < this->solucao_atual.custo){
                this->solucao_atual = s;
                k = 1;
                continue;
            }
            else{
                k++;
                continue;
            }
        }
        else if (k == 3){
            Solucao s = this->reinsertion();
            if (s.custo < this->solucao_atual.custo){
                this->solucao_atual = s;
                k = 1;
                continue;
            }
            else{
                k++;
                continue;
            }
        }
        else if (k == 2){
            Solucao s = this->swap();
            if (s.custo < this->solucao_atual.custo){
                this->solucao_atual = s;
                k = 1;
                continue;
            }
            else{
                k++;
                continue;
            }
        }
      
    }
    return this->solucao_atual;
}

Solucao VND::swap(){

    struct swap{
        int job1;
        int novo_server_j1;
        int ocup_server_j1;
        int job2;
        int novo_server_j2;
        int ocup_server_j2;
        int novo_custo;
        int custo_nuvem;
        int custo_local;
    };
    swap melhor_swap;
    melhor_swap.novo_custo = this->solucao_atual.custo;
    melhor_swap.custo_local = this->solucao_atual.custo_local;
    melhor_swap.custo_nuvem = this->solucao_atual.custo_nuvem;

    int n_jobs = this->dados->get_n_jobs();
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
            if (custo_atual < melhor_swap.novo_custo){
                melhor_swap.novo_custo = custo_atual;
                // Atualiza o novo servidor de j1 com a ocupação do servidor de destino (j2)
                melhor_swap.job1 = j1;
                melhor_swap.novo_server_j1 = serv_atual_j2;
                melhor_swap.ocup_server_j1 = ocup_serv_atual_j2 + t_j1_serv_j2;
                // Atualiza o novo servidor de j2 com a ocupação do servidor de destino (j1)
                melhor_swap.job2 = j2;
                melhor_swap.novo_server_j2 = serv_atual_j1;
                melhor_swap.ocup_server_j2 = ocup_serv_atual_j1 + t_j2_serv_j1;
                // Atualiza o custo local e nuvem
                melhor_swap.custo_local = this->solucao_atual.custo_local;
                melhor_swap.custo_nuvem = this->solucao_atual.custo_nuvem 
                                            - this->dados->get_custo_job_servidor(j1, serv_atual_j1) 
                                            - this->dados->get_custo_job_servidor(j2, serv_atual_j2) 
                                            + this->dados->get_custo_job_servidor(j1, serv_atual_j2) 
                                            + this->dados->get_custo_job_servidor(j2, serv_atual_j1);
            }
        }
    }
    Solucao nova_solucao = this->solucao_atual;
    if (melhor_swap.novo_custo < this->solucao_atual.custo){
        nova_solucao.custo = melhor_swap.novo_custo;
        nova_solucao.custo_nuvem = melhor_swap.custo_nuvem;
        nova_solucao.custo_local = melhor_swap.custo_local;
        nova_solucao.alocacao[melhor_swap.job1] = melhor_swap.novo_server_j1;
        nova_solucao.alocacao[melhor_swap.job2] = melhor_swap.novo_server_j2;
        nova_solucao.ocupacao[melhor_swap.novo_server_j1] = melhor_swap.ocup_server_j1;
        nova_solucao.ocupacao[melhor_swap.novo_server_j2] = melhor_swap.ocup_server_j2;
    }
    return nova_solucao;
}

Solucao VND::swap_local(){

    struct swap{
        int job1;
        int novo_server_j1;
        int ocup_server_j1;
        int job2;
        int novo_server_j2;
        int ocup_server_j2;
        int novo_custo;
        int custo_nuvem;
        int custo_local;
    };

    swap melhor_swap;
    melhor_swap.novo_custo = this->solucao_atual.custo;
    melhor_swap.custo_local = this->solucao_atual.custo_local;
    melhor_swap.custo_nuvem = this->solucao_atual.custo_nuvem;
    int n_jobs = this->dados->get_n_jobs();

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
                int custo_j2_serv_j2 = this->dados->get_custo_job_servidor(j2, serv_atual_j2); // O(1)
                custo_atual -= custo_j2_serv_j2;
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
                int custo_j1_serv_j2 = this->dados->get_custo_job_servidor(j1, serv_atual_j2); // O(1)
                custo_atual += custo_j1_serv_j2;
                // não precisa adicionar o custo fixo pois o servidor j1 estava adicionando o custo fixo
                
                // Verifica se o custo é menor que o custo atual
                if (custo_atual < melhor_swap.novo_custo){
                    melhor_swap.novo_custo = custo_atual;
                    // Atualiza o novo servidor de j1 com a ocupação do servidor de destino (j2)
                    melhor_swap.job1 = j1;
                    melhor_swap.novo_server_j1 = serv_atual_j2;
                    melhor_swap.ocup_server_j1 = ocup_serv_atual_j2 + t_j1_serv_j2;
                    // Atualiza o novo servidor de j2 com a ocupação do servidor de destino (j1)
                    melhor_swap.job2 = j2;
                    melhor_swap.novo_server_j2 = -1;
                    melhor_swap.ocup_server_j2 = -1;
                    // Atualiza o custo local e nuvem
                    melhor_swap.custo_local = this->solucao_atual.custo_local;
                    melhor_swap.custo_nuvem = this->solucao_atual.custo - custo_j2_serv_j2 + custo_j1_serv_j2;
                }
            }
            else{
                // Obtem o custo atual da solução
                int custo_atual = this->solucao_atual.custo; // O(1)

                // Retira o custo da solucao dos dois jobs
                int custo_j1_serv_j1 = this->dados->get_custo_job_servidor(j1, serv_atual_j1); // O(1)
                custo_atual -= custo_j1_serv_j1;

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

                int custo_j2_serv_j1 = this->dados->get_custo_job_servidor(j2, serv_atual_j1); // O(1)
                custo_atual += custo_j2_serv_j1;
                
                // Verifica se o custo é menor que o custo atual
                if (custo_atual < melhor_swap.novo_custo){
                    melhor_swap.novo_custo = custo_atual;
                    // Atualiza o novo servidor de j1 com a ocupação do servidor de destino (j2)
                    melhor_swap.job1 = j1;
                    melhor_swap.novo_server_j1 = -1;
                    melhor_swap.ocup_server_j1 = -1;
                    // Atualiza o novo servidor de j2 com a ocupação do servidor de destino (j1)
                    melhor_swap.job2 = j2;
                    melhor_swap.novo_server_j2 = serv_atual_j1;
                    melhor_swap.ocup_server_j2 = ocup_serv_atual_j1 + t_j2_serv_j1;
                    // Atualiza o custo local e nuvem
                    melhor_swap.custo_local = this->solucao_atual.custo_local;
                    melhor_swap.custo_nuvem = this->solucao_atual.custo - custo_j1_serv_j1 + custo_j2_serv_j1;
                }
            }

            
        }
    }
    Solucao nova_solucao = this->solucao_atual;
    if (melhor_swap.novo_custo < this->solucao_atual.custo){
        nova_solucao.custo = melhor_swap.novo_custo;
        nova_solucao.custo_nuvem = melhor_swap.custo_nuvem;
        nova_solucao.custo_local = melhor_swap.custo_local;
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

    struct reinsertion{
        int job = -1;
        int servidor = -1;
        int novo_custo;
        int custo_nuvem;
        int custo_local;
    };

    reinsertion melhor_reinsertion;
    melhor_reinsertion.novo_custo = this->solucao_atual.custo;
    melhor_reinsertion.custo_local = this->solucao_atual.custo_local;
    melhor_reinsertion.custo_nuvem = this->solucao_atual.custo_nuvem;

    for (int j = 0; j < this->dados->get_n_jobs(); j++){
        // Itera sobre os servidores para encontrar o novo melhor servidor para o job j
        for (int s = 0; s<this->dados->get_n_servidores(); s++){
            // Desconsidera o servidor atual do job j  
            if (this->solucao_atual.alocacao[j] != s){
                
                int tempo_novo_servidor = this->dados->get_tempo_job_servidor(j, s);
                // Verifica se o servidor tem capacidade para alocar o job j no servidor s
                if (this->solucao_atual.ocupacao[s] + tempo_novo_servidor 
                    > this->dados->get_capacidade_servidor(s)){
                    continue;
                }
                // recupera o custo atual da solução
                int custo_atual = this->solucao_atual.custo;

                // Retira o custo do job j no servidor atual
                int servidor_atual = this->solucao_atual.alocacao[j];
                int custo_job_servidor_atual = this->dados->get_custo_job_servidor(j, servidor_atual);
                custo_atual -= custo_job_servidor_atual;

                // Adiciona o custo do job j no novo servidor
                int custo_job_servidor_novo = this->dados->get_custo_job_servidor(j, s);
                custo_atual += custo_job_servidor_novo;
                // Verifica se o custo_atual é menor que o novo custo dos reinsertion anteriores
                if (custo_atual < melhor_reinsertion.novo_custo){
                    // Atualiza o melhor custo, job e servidor para troca
                    melhor_reinsertion.novo_custo = custo_atual;
                    melhor_reinsertion.job = j;
                    melhor_reinsertion.servidor = s;
                    if (servidor_atual == -1){
                        // se for troca do local para a nuvem, retira o custo fixo do custo local e adiciona no custo da nuvem
                        // o custo do job no servidor novo
                        melhor_reinsertion.custo_local = this->solucao_atual.custo_local - custo_job_servidor_atual;
                        melhor_reinsertion.custo_nuvem = this->solucao_atual.custo_nuvem + custo_job_servidor_novo;
                    }
                    else{
                        melhor_reinsertion.custo_nuvem = this->solucao_atual.custo_nuvem - custo_job_servidor_atual 
                                                        + custo_job_servidor_novo;
                    }
                }
            }
        }
    }
    //cout << "finalizou busca\n";
    Solucao nova_solucao = this->solucao_atual;
    if (melhor_reinsertion.novo_custo < this->solucao_atual.custo){
        // reduz a capacidade do servidor antigo
        int job = melhor_reinsertion.job;
        int servidor = melhor_reinsertion.servidor;
        int servidor_antigo = nova_solucao.alocacao[job];
        if (servidor_antigo != -1){
            nova_solucao.ocupacao[servidor_antigo] -= this->dados->get_tempo_job_servidor(job, servidor_antigo);
        }
        // atualiza a nova solução
        nova_solucao.alocacao[job] = servidor;
        nova_solucao.ocupacao[servidor] += this->dados->get_tempo_job_servidor(job, servidor);
        // atualiza o custo
        nova_solucao.custo = melhor_reinsertion.novo_custo;
        nova_solucao.custo_nuvem = melhor_reinsertion.custo_nuvem;
        nova_solucao.custo_local = melhor_reinsertion.custo_local;
        return nova_solucao;
    }
    return this->solucao_atual;
}