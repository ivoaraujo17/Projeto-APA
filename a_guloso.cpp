#include <iostream>
#include <vector>
#include <fstream>
#include "a_guloso.h"
#include "read_arquivo.h"

using namespace std;

A_Guloso::A_Guloso(Read_Arquivo* dados){
    this->dados = dados;
}

void A_Guloso::executar(){

    int n_servidores = dados->get_n_servidores();
    int n_jobs = dados->get_n_jobs();
    this->c_s_ocupada.resize(n_servidores);
    this->alocacao.resize(n_jobs);
    tempo_job_ordenado.resize(n_jobs, vector<vector<int>>(n_servidores, vector<int>(2)));

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
        vector<vector<int>> tempo_job = dados->get_tempo_job(j); // Complexidade O(s)
        
        // Ordena do menor para o maior tempo - Complexidade O(s)
        for (int s = 0; s < n_servidores; s++){
            int index_menor_t = s;
            int i = s;
            while (i < n_servidores){
                if (tempo_job[i][0] < tempo_job[index_menor_t][0]){
                    index_menor_t = i;
                }
                i ++;
            }
            if (index_menor_t != i){
                vector<int> aux = tempo_job[index_menor_t];
                tempo_job[index_menor_t] = tempo_job[s];
                tempo_job[s] = aux;
            }
        }

        tempo_job_ordenado[j] = tempo_job;

        // aloca o job no servidor de menor custo O(s)
        for (int s = 0; s < n_servidores; s++){
            int tempo = tempo_job[s][0];
            int servidor = tempo_job[s][1];
            if (this->c_s_ocupada[s] + tempo <= dados->get_capacidade_servidor(s)){
                this->c_s_ocupada[s] += tempo;
                alocacao[j] = servidor;
                break; 
            }
        }
    }

    /*
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
            cout << tempo_job_ordenado[j][s][0] << " " << tempo_job_ordenado[j][s][1] << " | ";
        }
        cout << endl;
    }
    */
}